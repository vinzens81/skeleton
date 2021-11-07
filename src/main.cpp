#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include "SPIFFS.h"


WebServer server(80);
const int led = 2;
char ssid[30];
String localIP;
boolean connected;
boolean wifiConnected = false;

#include "filehandle.h"
#include "http_answer.h"
#include "handleLoop.h"


void setup(void)
{
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);

  String passtmp;
  String ssidtmp;

  if (!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
  }
  else
  {
    delay(1000);
    Serial.println("Mounted SPIFFS successfully");
    passtmp = readFile("password");
    ssidtmp = readFile("ssid");
  }

  if (passtmp.length() > 0 && ssidtmp.length() > 0)
  {
    Serial.println("Using stored SSID and pass");

    char pass[30];

    passtmp.toCharArray(pass, passtmp.length() + 1);
    ssidtmp.toCharArray(ssid, ssidtmp.length() + 1);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pass);
    Serial.println("");
    Serial.print("Connecting to SSID: ");
    Serial.print(ssid);
    // Wait for connection
    wifiConnected = true;
    int connectCounter = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
      delay(500);
      Serial.print(".");
      connectCounter++;
      if (connectCounter > 50)
      {
        Serial.println("Connection took to long, aborting and start in Standalone Mode");
        wifiConnected = false;
        break;
      }
    }
  }

  if (wifiConnected)
  {
    localIP = WiFi.localIP().toString();
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(localIP);

    if (MDNS.begin("esp32"))
    {
      Serial.println("MDNS responder started");
    }

    server.on("/", handleRoot);
    server.on("/setParams", setParams);
    server.on("/getParams", getParams);
    server.on("/status", getStatus);
    server.on("/resetWifi", resetWifi);
    server.onNotFound(handleNotFound);
    server.begin();
    Serial.println("HTTP server started");

    connected = true;
  }
  else
  {
    connected = false;
    WiFi.softAP("ESP-Standalone", "password");
    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);
    delay(200);
    server.on("/", handleRoot);
    server.on("/setup", standaloneAP);
    server.on("/setParams", setParams);
    server.on("/getParams", getParams);
    server.on("/resetWifi", resetWifi);
    server.on("/setParams", setParams);
    server.onNotFound(handleNotFound);
    server.begin();
    Serial.println("HTTP server started");
  }
}

void loop(void)
{
  if (connected)
  {
    handleConnectedLoop(led);
  }
  else
  {
    handleStandaloneLoop(led);
  }
}