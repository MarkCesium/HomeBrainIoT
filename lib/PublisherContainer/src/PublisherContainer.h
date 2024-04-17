#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include "Publisher.h"

class PublisherContainer
{
private:
	bool state = true;
	Publisher **publishers;
	unsigned int currentSensor = 0;
	unsigned int publisherCount = 0;
	/* data */
public:
	PublisherContainer(/* args */);
	~PublisherContainer();
	void callSensor();
	String report();
	void addPublisher(Publisher *publisher);
	Publisher *getPublisher(int number, Publisher *publisher);
};

PublisherContainer::PublisherContainer(/* args */)
{
}

PublisherContainer::~PublisherContainer()
{
}

void PublisherContainer::callSensor()
{
	if (!this->state)
	{
		return;
	}

	unsigned long int curentMillis = millis();
	Publisher *sensor = this->publishers[this->currentSensor];
	if ((curentMillis - sensor->getLastCall()) >= sensor->getRequestDelay())
	{
		float data = sensor->callHandler();
		sensor->setData(data);
		sensor->setLastCall(curentMillis);
	}

	if (this->currentSensor == this->publisherCount - 1)
	{
		this->currentSensor = 0;
	}
	else
	{
		this->currentSensor++;
	}
}

void PublisherContainer::addPublisher(Publisher *publisher)
{
	this->publishers = (Publisher **)realloc(this->publishers, (this->publisherCount + 1) * sizeof(Publisher *));
	this->publishers[this->publisherCount] = publisher;
	this->publisherCount++;
}

String PublisherContainer::report()
{
	JsonDocument outJSON;
	for (unsigned int i = 0; i < this->publisherCount; i++)
	{
		outJSON["data"][i]["id"] = this->publishers[i]->getId();
		outJSON["data"][i]["value"] = this->publishers[i]->getData();
	}
	char output[256];
	serializeJson(outJSON, output);

	return output;
}