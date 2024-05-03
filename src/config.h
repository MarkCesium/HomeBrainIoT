// BLOCK server info
// Do not change this block!

const String HOMEBRAIN = "192.168.100.2";
const String ROOT_PORT = "80";
const String WS_SENSOR_PORT = "3003";
const String AUTH = "/api/api-authentication";
const String GET_SENSORS = "/api/sensors/get";
const String GET_DEVICES = "/api/devices/get";

// ENDBLOCK server info

// ----------------------------------------------------

// BLOCK user's classes, constants and params

#define DHTType DHT22

// ENDBLOCK user's classes, constants and params

// ----------------------------------------------------

// BLOCK user's info
/*
	You can change this block.
	You must to write here valid info
	about your Wi-Fi connection, plates
	username and password.
*/

// Type here all your avaible WiFi points.
// Ex: {(char *)"<Your-WiFi-Name>", (char *)"<Your-WiFi-Pass>"}
char *AccessPoints[][2] = {
	{(char *)"Keenetic-2206", (char *)"PDY5A8E2"},
	{(char *)"HONOR 20", (char *)"87654321"},
	{(char *)"Redmi 5+", (char *)"12345678"}
	// {(char *)"TechnoPark", (char *)"test@TEST"}
};

// Type here username and password of that plate
/*
	THE DATA MUST BE VALID WITH THE
	DATA FROM HOMEBRAIN!
*/
const String USERNAME = "wemos";
const String PASSWORD = "12345678";

// ENDBLOCK user's info

// ----------------------------------------------------

// BLOCK user's handlers

// Out of the box, you already have three handlers:
//                                                 getAnalogData()         - for sensors
//                                                 getDigitalData()        - for sensors
//                                                 activateDigitalDevice() - for devices

/*
	If you use external libraries
	you should initialize the corresponding
	classes either in the function body or
	in the global scope of the file (BLOCK
	user's classes, constants and params).
*/

float getAnalogData(Publisher *publisher)
{
	return analogRead(publisher->getPinOutput());
}

float getDigitalData(Publisher *publisher)
{
	return digitalRead(publisher->getPinOutput()); // TODO: Input!!!
}

// Type here your own handlers for sensors.
// Ex: <type> <name>(Publisher *publisher) { <do some stuff> }
/*
	Don't forget to set pinMode.
*/

// float getHumidity(Publisher *publisher)
// {
// 	DHT dht(publisher->getPinInput(), DHTType);
//  dht.begin();
// 	Hum = dht.readHumidity(); // Gets the values of the humidity
// 	Serial.printf("Humidity: %f\n", Hum);
// 	return Hum;
// }

// The addSensorsHandlers() function appends your handlers for sensors.
// To add a handler, you need to do the following:
/* Ex:
  String <handler>Name = "<handler>";
  THandler <handler>Link = &<handler>;
  handlerContainer.addHandler(&<handler>Name, <handler>Link);
*/

void addSensorsHandlers()
{
	String getAnalogDataHandlerName = "getAnalogData";
	THandler getAnalogDataHandlerLink = &getAnalogData;
	handlerContainer.addHandler(&getAnalogDataHandlerName, getAnalogDataHandlerLink);

	String getDigitalDataHandlerName = "getDigitalData";
	THandler getDigitalDataHandlerLink = &getDigitalData;
	handlerContainer.addHandler(&getDigitalDataHandlerName, getDigitalDataHandlerLink);

	// String getHumidityHandlerName = "getHumidity";
	// THandler getHumidityHandlerLink = &getHumidity;
	// handlerContainer.addHandler(&getHumidityHandlerName, getHumidityHandlerLink);
}

// ----------------------------------------------------

float activateDigitalDevice(Publisher *publisher) // TODO: OUTput!!!
{
	pinMode(publisher->getDevicePinOutput(), OUTPUT);
	digitalWrite(publisher->getDevicePinOutput(), HIGH);
	delay(publisher->getDeviceRuntime());
	digitalWrite(publisher->getDevicePinOutput(), LOW);

	return true;
}

// Type here your own handlers for sensors.
// Ex: <type> <name>(Publisher *publisher) { <do some stuff> }

// The addDevicesHandlers() function appends your handlers for sensors.
// To add a handler, you need to do the following:
/* Ex:
  String <handler>Name = "<handler>";
  THandler <handler>Link = &<handler>;
  handlerContainer.addHandler(&<handler>Name, <handler>Link);
*/

void addDevicesHandlers()
{
	String activateDigitalDeviceHandlerName = "activateDigitalDevice";
	THandler activateDigitalDeviceHandlerLink = &activateDigitalDevice;
	handlerContainer.addHandler(&activateDigitalDeviceHandlerName, activateDigitalDeviceHandlerLink);
}

// ENDBLOCK user's handlers