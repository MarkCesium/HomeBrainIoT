#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFiMulti.h>
#include <ArduinoJson.h>
#include <DHT.h>
#include "Publisher.h"
#include "HandlerContainer.h"
#include "PublisherContainer.h"

HTTPClient http;
ESP8266WiFiMulti wifiMulti;
WiFiClient client;
HandlerContainer handlerContainer;
PublisherContainer app;

#include <config.h>