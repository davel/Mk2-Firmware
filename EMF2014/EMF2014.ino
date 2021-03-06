/*
 TiLDA Mk2

 EMF2014
 This is the entry point for the badge firmware.
 Here we deal with the setup of all the badge features
 This includes:
 Initial Pin States
 Detecting the reset condition (power cycle, button wake from deep sleep, IMU wake from deep sleep)
 Recovering of the config or hibernation file from the Flash
 Restoring context as needed
 Starting the main TiLDATask and the FreeRTOS scheduler


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

// Reference all libraries that are used here, otherwise Arduino won't include them :(
#include <FreeRTOS_ARM.h>
#include <Sha1.h>
#include <DueFlashStorage.h>
#include <TinyPacks.h>
#include <rtc_clock.h>
#include <uECC.h>
#include <Arduino.h>

// These are the includes actually needed for this file:
#include "EMF2014Config.h"
#include "DebugTask.h"
#include "RGBTask.h"
#include "ButtonTask.h"
#include "RadioTask.h"
#include "MessageCheckTask.h"
#include "AppOpenerTask.h"
#include "AppManager.h"
#include "FlashLightApp.h"
#include "HomeScreenApp.h"
#include "TiLDAButtonInterrupts.h"
#include "Tilda.h"
#include "SettingsStore.h"

/*
 * Setup is the main entry point for an Arduino sketch.
 * Here is where we will do a lot of work in getting the system running
 * and in FreeRTOS we will start the scheduler
 */

RTC_clock realTimeClock(RC);
SettingsStore settingsStore;
AppManager appManager;

DebugTask debugTask;
RGBTask rgbTask;
ButtonTask buttonTask;
MessageCheckTask messageCheckTask;
RadioTask radioTask(messageCheckTask, realTimeClock);
AppOpenerTask appOpenerTask(appManager);

FlashLightApp flashLightApp;
HomeScreenApp homeScreenApp;

void setup() {
    debug::setup();
    Tilda::setupTasks(&appManager, &rgbTask, &realTimeClock);

    // Uncomment this if you want to see serial output during startup
    // This will require you to send a character over serial before unblocking
    // the startup

    debug::waitForKey();

    realTimeClock.init();

    tildaButtonSetup();
    tildaButtonAttachInterrupts();
    tildaButtonInterruptPriority();

    // Background tasks
    debugTask.start();
    rgbTask.start();
    buttonTask.start();
    messageCheckTask.start();
    radioTask.start();
    appOpenerTask.start();

    // Applications
    appManager.add(homeScreenApp);
    appManager.add(flashLightApp);

    // Boot into home screen
    Tilda::openApp("HomeScreen");

    // Start scheduler
    debug::log("Start Scheduler");
    vTaskStartScheduler();

    debug::log("Insufficient RAM");
    while(1);
}

/*
 * In this Arduino implmentation of FreeRTOS the loop is used as the IDLE_HOOK
 * This runs on each iteration of the idle task, this will only run if all other task are blocked.
 */
void loop() {

}