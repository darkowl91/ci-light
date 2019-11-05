/*
 CI Build Traffic Light Controller based on LoLin new NodeMcu v3(ESP-12E) board
 Board Manager: http://arduino.esp8266.com/stable/package_esp8266com_index.json
 Upload speed: 115200
 CPU frequency: 80MHz
 Platfrom: esp8266 v2.4.2
 */
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <FastLED.h>
#include "Secrets.h"

// PIN
#define DATA_PIN 1 // D1

// LED
#define NUM_LEDS 150
CRGB leds[NUM_LEDS];

// WIFI
char ssid[] = SSID;
char password[] = PASSWORD;

// TGM
char token[] = TOKEN;

WiFiClientSecure client;
UniversalTelegramBot bot(token, client);

int Bot_mtbs = 1000; // time between scan messages
long Bot_lasttime;   // last time messages scan has been done

void setup()
{
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println("Start excecuting setup");

  //setup leds
  FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
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
  // message format: #38338 SUCCESS
  if (text != NULL && text.startsWith("#") && text.indexOf(" ") > 0)
  {
    String buildStatus = text.substring(text.indexOf(" ") + 1, text.length());
    Serial.println("build status: " + buildStatus);

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
  for (int i = 0; i < NUM_LEDS; i = i + 1)
  {
    leds[i] = CRGB::Red;
    FastLED.show();
    delay(5);
  }
}

void setLightOrange()
{
  for (int i = 0; i < NUM_LEDS; i = i + 1)
  {
    leds[i] = CRGB::Yellow;
    FastLED.show();
    delay(5);
  }
}

void setLightRed()
{
  for (int i = 0; i < NUM_LEDS; i = i + 1)
  {
    leds[i] = CRGB::Green;
    FastLED.show();
    delay(5);
  }
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
    Serial.print(".");
    delay(300);
  }
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  Serial.print("mac address: ");
  Serial.print(WiFi.macAddress());
}

void blink()
{
  for (int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed = whiteLed + 1)
  {
    // Turn our current led on to white, then show the leds
    leds[whiteLed] = CRGB::White;

    // Show the leds (only one of which is set to white, from above)
    FastLED.show();

    // Wait a little bit
    delay(5);

    // Turn our current led back to black for the next loop around
    leds[whiteLed] = CRGB::Black;
  }
}
