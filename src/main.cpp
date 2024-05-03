#include <main.h>
#include <ArduinoWebsockets.h>
#define MOTION_SPEED 921600

int statusCode;
unsigned long prevCall = 0;
JsonDocument token;
JsonDocument sensor;
JsonDocument device;

using namespace websockets;
WebsocketsClient wsclient;

float getHumidity(Publisher *publisher)
{
	DHT dhtHum(publisher->getPinInput(), DHTType);
	dhtHum.begin();
	float Hum = dhtHum.readHumidity(); // Gets the values of the humidity
	Serial.printf("Humidity: %f\n", Hum);
	return Hum;
}

float getTemp(Publisher *publisher)
{
	DHT dhtTemp(publisher->getPinInput(), DHTType);
	dhtTemp.begin();
	float Temp = dhtTemp.readTemperature(); // Gets the values of the temperature
	Serial.printf("Temperature: %f\n", Temp);
	return Temp;
}

float ledSwitch(Publisher *publisher)
{
	Serial.println("___________________LED_SWITCH__________________");
	pinMode(publisher->getDevicePinOutput(), OUTPUT);
	if (ledSwitchValue)
	{
		digitalWrite(publisher->getDevicePinOutput(), LOW);
		ledSwitchValue = false;
		return 0.0;
	}
	else
	{
		digitalWrite(publisher->getDevicePinOutput(), HIGH);
		ledSwitchValue = true;
		return 1.0;
	}
}

void printAvaibleAP()
{
	int n = WiFi.scanNetworks();
	Serial.printf("%i network(s) found\n", n);
	Serial.println("--------------------------");
	for (int i = 0; i < n; i++)
	{
		Serial.println(WiFi.SSID(i));
	}
	Serial.println("--------------------------");
}

void addAP()
{
	uint listSize = sizeof(AccessPoints);
	uint elementSize = sizeof(AccessPoints[0]);
	uint elementsInArray = listSize / elementSize;
	Serial.println("Adding access points!");
	printAvaibleAP();
	for (uint i = 0; i < elementsInArray; i++)
	{
		wifiMulti.addAP(AccessPoints[i][0], AccessPoints[i][1]);
	}
}

void connect()
{
	Serial.println("Connecting");
	while (wifiMulti.run() != WL_CONNECTED)
	{
		delay(500);
		Serial.print(".");
	}
	Serial.println();
	Serial.println("Connected!");
	Serial.print("IP-address: ");
	Serial.println(WiFi.localIP());
	Serial.println("AP: " + WiFi.SSID());
	Serial.println("--------------------------");
}

void onMessageCallback(WebsocketsMessage message)
{
	Serial.print("Got Message: ");
	Serial.println(message.data());
	JsonDocument msgIn;
	deserializeJson(msgIn, message.data());
	unsigned int id = msgIn["msg"]["device"];
	Serial.println(id);
	msgIn.clear();

	Publisher *publisher = app.getPublisher(id);
	float result = publisher->callHandler();

	JsonDocument msgOut;
	msgOut["type"] = "sendDevice";
	msgOut["data"]["id"] = id;
	msgOut["data"]["value"] = result;
	char output[256];
	serializeJson(msgOut, output);
	msgOut.clear();

	wsclient.send(output);
}

void onEventsCallback(WebsocketsEvent event, String data)
{
	if (event == WebsocketsEvent::ConnectionOpened)
	{
		Serial.println("Connnection Opened");
	}
	else if (event == WebsocketsEvent::ConnectionClosed)
	{
		Serial.println("Connnection Closed");
	}
	else if (event == WebsocketsEvent::GotPing)
	{
		Serial.println("Got a Ping!");
	}
	else if (event == WebsocketsEvent::GotPong)
	{
		Serial.println("Got a Pong!");
	}
}

void connectWS()
{
	wsclient.onMessage(onMessageCallback);
	wsclient.onEvent(onEventsCallback);
	wsclient.connect("ws://" + HOMEBRAIN + ":" + WS_SENSOR_PORT);
}

void hello_test()
{
	while (true)
	{
		http.begin(client, "http://192.168.100.2/api/api-hello");

		int code = http.GET();
		if (code > 0)
		{
			String payload = http.getString();
			Serial.print("Data is: ");
			Serial.println(payload);
			Serial.printf("Code is: %i\n", code);
			break;
		}
		else
		{
			Serial.println("Failed");
			Serial.printf("Code is: %i\n", code);
		}
		delay(500);
	}
}

int auth(String url, String name, String pass)
{
	Serial.println("Authorization");
	JsonDocument out;
	out["_username"] = name;
	out["_password"] = pass;

	String data;
	serializeJson(out, data);
	Serial.println(data);
	Serial.println(url);
	// client.setInsecure();

	// http.setTimeout(8000);
	// client.setTimeout(8000);
	http.begin(client, url);
	http.addHeader("Content-Type", "application/json");
	int httpCode = http.POST(data);
	// Serial.println(http.getString());
	Serial.printf("--- (POST) Response code: %i\n", httpCode);
	if (httpCode > 0)
	{
		deserializeJson(token, http.getString());
	}
	http.end();

	return httpCode;
}

int loadSensors(String url)
{
	http.begin(client, url);
	http.addHeader("Content-Type", "application/json");
	http.addHeader("jwt-auth", token["token"].as<const char *>());
	int httpCode = http.GET();
	Serial.printf("--- (GET) Response code: %i\n", httpCode);
	if (httpCode > 0)
	{
		deserializeJson(sensor, http.getString());
	}
	http.end();

	return httpCode;
}

int loadDevices(String url)
{
	http.begin(client, url);
	http.addHeader("Content-Type", "application/json");
	http.addHeader("jwt-auth", token["token"].as<const char *>());
	int httpCode = http.GET();
	Serial.printf("--- (GET) Response code: %i\n", httpCode);
	if (httpCode > 0)
	{
		deserializeJson(device, http.getString());
	}
	http.end();

	return httpCode;
}

void setup()
{
	Serial.begin(921600);
	Serial.println();
	Serial.println("Start!");

	// set plate in client mode
	WiFi.mode(WIFI_STA);

	// adding Access Points to connect
	addAP();

	// connect
	connect();

	hello_test();

	// authorization
	statusCode = auth("http://" + HOMEBRAIN + AUTH, USERNAME, PASSWORD);
	if (statusCode != 200)
	{
		Serial.println("ERROR! Reboot...");
		delay(2000);
		ESP.reset();
	}

	// load sensors from HomeBrain
	statusCode = loadSensors("http://" + HOMEBRAIN + GET_SENSORS);
	if (statusCode != 200)
	{
		Serial.println("ERROR! Reboot...");
		delay(2000);
		ESP.reset();
	}

	Serial.println(sensor["data"].as<String>());
	int sensorCount = sensor["data"].size();

	// adding handlers
	// addSensorsHandlers();
	String getAnalogDataHandlerName = "getAnalogData";
	THandler getAnalogDataHandlerLink = &getAnalogData;
	handlerContainer.addHandler(&getAnalogDataHandlerName, getAnalogDataHandlerLink);

	String getDigitalDataHandlerName = "getDigitalData";
	THandler getDigitalDataHandlerLink = &getDigitalData;
	handlerContainer.addHandler(&getDigitalDataHandlerName, getDigitalDataHandlerLink);

	String getHumidityHandlerName = "getHumidity";
	THandler getHumidityHandlerLink = &getHumidity;
	handlerContainer.addHandler(&getHumidityHandlerName, getHumidityHandlerLink);

	String getTempHandlerName = "getTemp";
	THandler getTempHandlerLink = &getTemp;
	handlerContainer.addHandler(&getTempHandlerName, getTempHandlerLink);

	for (int i = 0; i < sensorCount; i++)
	{
		String tmp = sensor["data"][i]["publisherDescriptions"]["handler"].as<String>();
		Serial.print("In Array: ");
		Serial.println(tmp);
		if (!handlerContainer.getHandler(&tmp))
		{
			Serial.println("Handler not found in HandlerContainer. Sensor skipped");
			continue;
		}
		THandler handler = handlerContainer.handler;
		Serial.printf("Sensor ID: %i\n", sensor["data"][i]["id"].as<int>());
		Publisher *s = new Publisher(
			sensor["data"][i]["id"].as<int>(),
			2,
			sensor["data"][i]["publisherDescriptions"]["pin_i"].as<int>(),
			sensor["data"][i]["publisherDescriptions"]["srd"].as<int>(),
			handler);

		app.addPublisher(s);
	}

	Serial.println("Load devices");
	// load devices from HomeBrain
	statusCode = loadDevices("http://" + HOMEBRAIN + GET_DEVICES);
	if (statusCode != 200)
	{
		Serial.println("ERROR! Reboot...");
		delay(2000);
		ESP.reset();
	}

	Serial.println(device["data"].as<String>());
	int deviceCount = device["data"].size();

	// adding handlers
	// addDevicesHandlers();
	String getLedHandlerName = "ledSwitch";
	THandler getLedHandlerLink = &ledSwitch;
	handlerContainer.addHandler(&getLedHandlerName, getLedHandlerLink);

	for (int i = 0; i < deviceCount; i++)
	{
		String tmp = device["data"][i]["publisherDescriptions"]["handler"].as<String>();
		Serial.print("In Array: ");
		Serial.println(tmp);
		if (!handlerContainer.getHandler(&tmp))
		{
			Serial.println("Handler not found in HandlerContainer. Device skipped");
			continue;
		}
		THandler handler = handlerContainer.handler;
		Publisher *s = new Publisher(
			device["data"][i]["id"].as<int>(),
			device["data"][i]["publisherDescriptions"]["d_pin_o"].as<int>(),
			device["data"][i]["publisherDescriptions"]["d_rt"].as<int>(),
			handler);

		app.addPublisher(s);
	}
	connectWS();
}

void loop()
{
	if (WiFi.status() == WL_CONNECTED)
	{
		if (wsclient.available())
		{
			wsclient.poll();
		}
		else
		{
			Serial.println("WS connection is closed. Reconnecting");
			wsclient.connect("ws://" + HOMEBRAIN + ":" + WS_SENSOR_PORT);
			delay(100);
			return;
		}
		app.callSensor();
		unsigned long currentTime = millis();
		if (currentTime >= prevCall + 100)
		{
			prevCall = currentTime;
			String msg = app.report();
			wsclient.send(msg);
			Serial.print("WS send message: ");
			Serial.println(msg);
		}
	}
	else
	{
		Serial.println("Something went wrong! Reconnection to another point");
		connect();
	}
}