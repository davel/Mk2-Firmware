/*
 TiLDA Mke

 Task to emulate buttons over the USB serial port

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

#include <FreeRTOS_ARM.h>
#include "EMF2014Config.h"
#include "SerialButtonsTask.h"

void handleButtonPress(void *, uint32_t);

String SerialButtonsTask::getName() const {
	return "SerialButtons";
}

void SerialButtonsTask::task() {
	while (true) {
		while (SerialUSB.available()) {
			switch (SerialUSB.read()) {
			case ',':
				handleButtonPress(0, (uint32_t) A);
				break;
			case '.':
				handleButtonPress(0, (uint32_t) B);
				break;
			case 'a':
				handleButtonPress(0, (uint32_t) LEFT);
				break;
			case 'd':
				handleButtonPress(0, (uint32_t) RIGHT);
				break;
			case 'w':
				handleButtonPress(0, (uint32_t) UP);
				break;
			case 'x':
				handleButtonPress(0, (uint32_t) DOWN);
				break;
			case 's':
				handleButtonPress(0, (uint32_t) CENTER);
				break;
			}
		}

		vTaskDelay(portTICK_PERIOD_MS);
	}
}

void SerialButtonsTask::afterSuspension() {
}

void SerialButtonsTask::beforeResume() {
}
