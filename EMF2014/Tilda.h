/*
 TiLDA Mk2

 Tilda

 This file exposes a static, easy to use interface onto the TiLDA API. It's designed
 to make it easier for others to write Apps.

 The MIT License (MIT)

 Copyright (c) 2014 Electromagnetic Field LTD

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#pragma once

#include <Arduino.h>
#include <FreeRTOS_ARM.h>
#include <rtc_clock.h>
#include "EMF2014Config.h"
#include "ButtonSubscription.h"
#include "RGBTask.h"
#include "AppManager.h"


class Tilda {
public:
    static ButtonSubscription createButtonSubscription(uint16_t buttons);
    static void log(String text);
    static void delay(uint16_t durationInMs);
    static void setLedColor(RGBLed led, RGBColor color);
    static void setLedColor(RGBColor color);
    static void openApp(String name);
    static RTC_clock* getClock();

    // This is not part of the actual API
    static void setupTasks(AppManager* appManager, RGBTask* rgbTask, RTC_clock* realTimeClock);
private:
    Tilda();

    static RGBTask* _rgbTask;
    static AppManager* _appManager;
    static RTC_clock* _realTimeClock;
};