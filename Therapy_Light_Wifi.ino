//System Variables
const int sensorPin = 2;  // Change this pin to your input pin
volatile unsigned long pulseStart = 0;
volatile bool signalDetected = false;
volatile bool Startrecord = false;
volatile unsigned long lastActivityTime = 0;

char receivedData[26];  // 25 bits + '\0'


unsigned long Start_Millis();
int Duration = 60;  //minutes
String Hex_Color1 = "#147500";
String Hex_Color2 = "#007a78";
String Hex_Color3 = "#5a007a";
String Hex_Color4 = "#850061";
String Password = "Positivity!";
bool Wrong_Password;
bool Enable_LED = false;
volatile unsigned long LastButtonPress = 0;
volatile unsigned long Webpage_Timer = 0;

//LED pins
#define LED_Pin 16

float LED_Rate = 0.1;

//Wifi Stuff
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <DNSServer.h>
const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 2, 1);
DNSServer dnsServer;
WebServer server(80);  //Server on port 80

//File System Stuff
#define ARDUINOJSON_USE_LONG_LONG 1
#include <ArduinoJson.h>  //Version 5
#include "FS.h"
//#include "SPIFFS.h"
#include <LittleFS.h>
#define SPIFFS LittleFS
//#define FORMAT_SPIFFS_IF_FAILED true
#define FORMAT_LITTLEFS_IF_FAILED true
bool File_Avaliable;
char Clear_Text_Data[512];
#include "file_handler.h";

#include "webpages.h"
#include "webpage_handler.h"

#include "Color_Selector.h"

void setup() {
  pinMode(sensorPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(sensorPin), handleInterrupt, CHANGE);
  Serial.begin(115200);
  SetupLED();
  SetupFile();
  Start_Web_Services();
  SetupLED();
  Webpage_Timer = millis();
  LED_Rate = 1;
  Ramp_LED(128, 128, 128);
  delay(1000);
  Ramp_LED(0, 0, 0);
}

void loop() {
  if((millis() - Webpage_Timer) < 600000) {
    dnsServer.processNextRequest();
    server.handleClient();  //Handle client requests
    yield();
  }else  WiFi.mode(WIFI_OFF);

  Color_Timer(Enable_LED);

  if (signalDetected) {
    if (strcmp(receivedData, "0100010101101111001001101") == 0) {
      if ((millis() - LastButtonPress) > 500) {
        Serial.println("Clicked");
        Enable_LED = !Enable_LED;
      }
      LastButtonPress = millis();
    }
    memset(receivedData, 0, sizeof(receivedData));  // Reset receivedData array
    signalDetected = false;
  }
}


void handleInterrupt() {
  static int dataIndex = 0;
  if (digitalRead(sensorPin) == HIGH) {
    pulseStart = micros();
  } else {
    unsigned long pulseDuration = micros() - pulseStart;

    if (pulseDuration >= 950 && pulseDuration <= 1050) {
      Startrecord = true;
      receivedData[dataIndex++] = '1';
      lastActivityTime = millis();
    } else if ((pulseDuration >= 320 && pulseDuration <= 400) && Startrecord) {
      receivedData[dataIndex++] = '0';
      lastActivityTime = millis();
    }

    if (dataIndex >= 25) {
      Startrecord = false;
      receivedData[dataIndex] = '\0';  // Null-terminate the string
      signalDetected = true;
      dataIndex = 0;  // Reset dataIndex for the next sequence
    }
  }
  if (((millis() - lastActivityTime) > 7) && (dataIndex > 0)) {
    dataIndex = 0;  // Reset dataIndex after 7ms of inactivity
    Startrecord = false;
  }
}
