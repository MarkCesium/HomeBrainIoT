#pragma once

#include <Arduino.h>
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
	char stringBuffer[8] = "";
	String out = String();
	out.concat("{");
	out.concat("\"data\":");
	out.concat("[");

	for (int i = 0; i < this->publisherCount; i++)
	{
		if (i > 0)
		{
			out.concat(",");
		}

		out.concat("{");
		out.concat("\"");
		out.concat("id");
		out.concat("\"");
		out.concat(":");
		out.concat(itoa(this->publishers[i]->getId(), stringBuffer, 10));
		out.concat(",");
		out.concat("\"");
		out.concat("value");
		out.concat("\"");
		out.concat(":");
		out.concat(itoa(this->publishers[i]->getData(), stringBuffer, 10));
		out.concat("}");
	}
	out.concat("]");
	out.concat("}");

	return out;
}