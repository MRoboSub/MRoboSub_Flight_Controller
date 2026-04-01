#pragma once
#include <Arduino.h>
#include <WebServer.h>
#include "hal.hpp"

void setupWebServer(HAL *hal);
void updateWebServer();
