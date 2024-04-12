#pragma once

#include <Arduino.h>
#include "Publisher.h"

typedef float (*THandler)(Publisher *publish);

class HandlerContainer
{
private:
    String **handlersName;
    THandler *handlers;
    unsigned int handlersCount = 0;

public:
    THandler handler;
    HandlerContainer(/* args */);
    ~HandlerContainer();
    void addHandler(String *handlerName, THandler handler);
    bool getHandler(String *handlerName);
};

void HandlerContainer::addHandler(String *handlerName, THandler handler)
{
    this->handlers = (THandler *)realloc(this->handlers, (this->handlersCount + 1) * sizeof(THandler));
    this->handlers[this->handlersCount] = handler;
    this->handlersName = (String **)realloc(this->handlersName, (this->handlersCount + 1) * sizeof(String *));
    this->handlersName[this->handlersCount] = handlerName;
    this->handlersCount++;
}

bool HandlerContainer::getHandler(String *handlerName)
{
    Serial.println(this->handlersCount);

    for (unsigned int i = 0; i < this->handlersCount; i++)
    {
        Serial.print("handler in container: ");
        delay(1000);
        const char *tmp = this->handlersName[i]->c_str();
        Serial.println(tmp);
        Serial.print("Search: ");
        const char *tmp1 = handlerName->c_str();
        Serial.println(tmp1);
        delay(1000);

        if (this->handlersName[i]->equals(handlerName->c_str()))
        {
            Serial.print("handler found: ");
            Serial.println(handlerName->c_str());
            delay(1000);
            this->handler = this->handlers[i];

            return true;
        }
    }

    return false;
}

HandlerContainer::HandlerContainer(/* args */)
{
}

HandlerContainer::~HandlerContainer()
{
}
