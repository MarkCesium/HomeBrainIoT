#pragma once

#include <Arduino.h>

class Publisher
{
private:
	unsigned int id;
	unsigned int type;
	unsigned int pinInput;
	unsigned int pinOutput = 0;
	unsigned int devicePinOutput;
	unsigned int requestDelay = 500;
	unsigned int deviceRuntime = 1000;
	float (*handler)(Publisher *publish);
	unsigned long int lastCall;
	float data;

public:
	Publisher(
		unsigned int id,
		unsigned int type,
		unsigned int pinInput,
		unsigned int requestDelay,
		float (*handler)(Publisher *publish));

	Publisher(
		unsigned int id,
		unsigned int devicePinOutput,
		unsigned int deviceRuntime,
		float (*handler)(Publisher *publish));

	~Publisher();

	unsigned int getRequestDelay();

	float getData();

	unsigned int getType();

	void setData(float data);

	unsigned long int getLastCall();

	void setLastCall(unsigned long int lastCall);

	float callHandler();

	void setHandler(float (*handler)(Publisher *publish));

	unsigned int getId();

	unsigned int getPinOutput();

	unsigned int getPinInput();

	void setPinInput(unsigned int pinOutput);

	unsigned int getDevicePinOutput();

	void setDevicePinOutput(unsigned int devicePinOutput);

	void setDeviceRuntime(unsigned int deviceRuntime);

	unsigned int getDeviceRuntime();
};

Publisher::Publisher(
	unsigned int id,
	unsigned int type,
	unsigned int pinInput,
	unsigned int requestDelay,
	float (*handler)(Publisher *publish))
{
	this->id = id;
	this->type = type;
	this->pinInput = pinInput;
	this->requestDelay = requestDelay;
	this->handler = handler;
}

Publisher::Publisher(
	unsigned int id,
	unsigned int devicePinOutput,
	unsigned int deviceRuntime,
	float (*handler)(Publisher *publish))
{
	this->type = 1;
	this->id = id;
	this->devicePinOutput = devicePinOutput;
	this->deviceRuntime = deviceRuntime;
	this->handler = handler;
}

Publisher::~Publisher()
{
}

unsigned int Publisher::getId()
{
	return this->id;
}

unsigned long int Publisher::getLastCall()
{
	return this->lastCall;
}

unsigned int Publisher::getRequestDelay()
{
	return this->requestDelay;
}

float Publisher::getData()
{
	return this->data;
}

void Publisher::setData(float data)
{
	this->data = data;
}

unsigned int Publisher::getType()
{
	return this->type;
}

void Publisher::setLastCall(unsigned long int lastCall)
{
	this->lastCall = lastCall;
}

float Publisher::callHandler()
{
	return this->handler(this);
}

void Publisher::setHandler(float (*handler)(Publisher *publish))
{
	this->handler = handler;
}

unsigned int Publisher::getPinOutput()
{
	return this->pinOutput;
}

unsigned int Publisher::getPinInput()
{
	return this->pinInput;
}

void Publisher::setPinInput(unsigned int pinOutput)
{
	this->pinOutput = pinOutput;
}

void Publisher::setDeviceRuntime(unsigned int deviceRuntime)
{
	this->deviceRuntime = deviceRuntime;
}

unsigned int Publisher::getDeviceRuntime()
{
	return this->deviceRuntime;
}

unsigned int Publisher::getDevicePinOutput()
{
	return this->devicePinOutput;
}

void Publisher::setDevicePinOutput(unsigned int devicePinOutput)
{
	this->devicePinOutput = devicePinOutput;
}
