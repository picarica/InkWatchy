#ifndef DEFINES_H
#define DEFINES_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <GxEPD2_BW.h>
#include <SmallRTC.h>
#include "Fonts/FreeSansBold9pt7b.h"
#include <WiFi.h>
#include <WiFiMulti.h>
#include <NTPClient.h>
#include <OpenWeatherOneCall.h>

#define PARTIAL_UPDATE true
#define FULL_UPDATE false

#include "config.h" // Needs to be first!
#include "confidential.h"
#include "macros.h"
#include "images.h"

#include "../hardware/hardware.h"
#include "../hardware/battery.h"
#include "../hardware/RTC.h"
#include "../hardware/sleep.h"
#include "../hardware/buttons.h"
#include "../hardware/display.h"
#include "../functions.h"
#include "../ui/functionsUi.h"
#include "../ui/debug.h"
#include "../ui/batteryDebug.h"
#include "../ui/wifiDebug.h"
#include "../network/wifi.h"
#include "../network/ntp.h"
#include "../network/weather.h"

#endif
