#include <Arduino.h>
#include <WiFi.h>
#include "webserver.hpp"
#include "hal.hpp"

const char *WIFI_NAME = "MROBOSUB_TEST_CONTROL";
const char *WIFI_PASSWORD = "robo rocks";

HAL hal;

void setup()
{
  Serial.begin(115200);

  Serial.println("Power on");
  Serial.println("Initalizing ESCs");

  delay(7000); // delay to allow the ESC to recognize the stopped signal

  Serial.println("ESCs initialized");

  // Wi-Fi
  WiFi.softAP(WIFI_NAME, WIFI_PASSWORD);
  Serial.println("AP started");
  Serial.println(WiFi.softAPIP());

  // Webserver
  setupWebServer(&hal);
}

void loop()
{
  if (Serial.available())
  {
    String input = Serial.readStringUntil('\n'); // read one line
    input.trim();                                // remove whitespace

    int colonIndex = input.indexOf(':');
    int commaIndex = input.indexOf(',');

    if (colonIndex > 0)
    {
      String command = input.substring(0, colonIndex);
      if (command == "HEARTBEAT")
      {
        // TODO
      }
      else if (command == "POWER")
      {
        int index = input.substring(colonIndex + 1, commaIndex).toInt();
        int value = input.substring(commaIndex + 1).toInt();

        hal.applyPower(index, value);

        Serial.println("Updated ESC " + String(index) + " to " + String(value));
      }
      else if (command == "ENABLE")
      {
        String value = input.substring(colonIndex + 1);
        if (value == "TRUE")
        {
          if (!hal.enable())
          {
            Serial.println("Flight Controller is in emergency stop");
          }
          else
          {
            Serial.println("Flight Controller Enabled");
          }
        }
        else if (value == "FALSE")
        {
          hal.disable();
          Serial.println("Flight Controller Disabled");
        }
        else
        {
          Serial.println("usage: ENABLE:{TRUE,FALSE}");
        }
      }
      else if (command == "ESTOP")
      {
        hal.estop();
        Serial.println("Flight Controller Emergency Stopped");
      }
    }
  }

  hal.update(0);

  updateWebServer();
}
