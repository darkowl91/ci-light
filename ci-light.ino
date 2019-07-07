/*
 CI Build Traffic Light Controller based on LoLin new NodeMcu v3(ESP-12E) board
 Board Manager: http://arduino.esp8266.com/stable/package_esp8266com_index.json
 Upload speed: 115200
 CPU frequency: 80MHz
 */
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include "Secrets.h"

// PIN
const int pinRed = 16;   //D0
const int pinOrange = 5; //D1
const int pinGreen = 4;  //D2

// WIFI
char ssid[] = SSID;
char password[] = PASSWORD;

// TGM
char token[] = TOKEN;

WiFiClientSecure client;
UniversalTelegramBot bot(token, client);

int Bot_mtbs = 1000; // time between scan messages
long Bot_lasttime;   // last time messages' scan has been done

void setup()
{
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println("Start excecuting setup");

  //setup pins
  pinMode(pinRed, OUTPUT);
  pinMode(pinGreen, OUTPUT);
  pinMode(pinOrange, OUTPUT);
  blink();

  //setup wifi
  connectWiFi();
}

void loop()
{
  if (millis() > Bot_lasttime + Bot_mtbs)
  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    while (numNewMessages)
    {
      Serial.println("got response");
      for (int i = 0; i < numNewMessages; i++)
      {
        if (bot.messages[i].type == "channel_post")
        {
          handleMessage(bot.messages[i].text);
        }
      }
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    Bot_lasttime = millis();
  }
}

void handleMessage(String text)
{
  // message fromat: #38338 SUCCESS
  if (text != NULL && text.startsWith("#") && text.indexOf(" ") > 0)
  {
    String buildStatus = text.substring(text.indexOf(" ") + 1, text.length());

    if (buildStatus == "SUCCESS" || buildStatus == "FIXED")
    {
      setLightGreen();
    }
    else if (buildStatus == "UNSTABLE")
    {
      setLightOrange();
    }
    else if (buildStatus == "FAILURE")
    {
      setLightRed();
    }
    else
    {
      Serial.println("unknown build status " + text);
    }
  }
}

void setLightGreen()
{
  digitalWrite(pinGreen, HIGH);
  digitalWrite(pinRed, LOW);
  digitalWrite(pinOrange, LOW);
}

void setLightOrange()
{
  digitalWrite(pinOrange, HIGH);
  digitalWrite(pinRed, LOW);
  digitalWrite(pinGreen, LOW);
}

void setLightRed()
{
  digitalWrite(pinRed, HIGH);
  digitalWrite(pinOrange, LOW);
  digitalWrite(pinGreen, LOW);
}

void connectWiFi()
{
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  Serial.print("Connecting to ");
  Serial.print(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    digitalWrite(pinOrange, HIGH);
    Serial.print(".");
    delay(500);
    digitalWrite(pinOrange, LOW);
  }
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
}

void blink()
{
  digitalWrite(pinRed, HIGH);
  delay(300);
  digitalWrite(pinRed, LOW);

  digitalWrite(pinOrange, HIGH);
  delay(300);
  digitalWrite(pinOrange, LOW);

  digitalWrite(pinGreen, HIGH);
  delay(300);
  digitalWrite(pinGreen, LOW);
}
