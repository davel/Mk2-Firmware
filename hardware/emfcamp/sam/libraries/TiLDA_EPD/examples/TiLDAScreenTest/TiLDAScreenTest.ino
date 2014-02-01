/*
 TiLDA Mk2 epaper screen test display
 
 Switch two images on button press
 Includes Due debunce
 
 
 The MIT License (MIT)
 
 Copyright (c) 2014 Electromagnetic  Field LTD
 
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

#include <SPI.h>
#include <TiLDA_EPD.h>
#include <Images.h>

#include <cat_2_7.xbm>
#include <saturn_2_7.xbm>

#define IMAGE_A 1
#define IMAGE_A_BITS cat_2_7_bits
#define IMAGE_B 2
#define IMAGE_B_BITS saturn_2_7_bits

EPD_Class EPD(EPD_2_7, EPD_PANEL_ON, EPD_BORDER_CONTROL, EPD_DISCHARGE, EPD_PWM, EPD_RESET, EPD_BUSY, 52);

int currentImage = 0;
int setImage = 0;

void setup() {
    Serial.begin(115200);
    Serial.println("Pin setup");
    // PMIC to CHARGE
    pinMode(PMIC_ENOTG, OUTPUT);
    digitalWrite(PMIC_ENOTG, LOW);
    EPD.pinSetup();
    
    pinMode(BUTTON_A, INPUT);
    pinMode(BUTTON_B, INPUT);
    
    digitalWrite(BUTTON_A, HIGH);
    digitalWrite(BUTTON_B, HIGH);
    
    attachInterrupt(BUTTON_A, showImageA, FALLING);
    setDebounce(BUTTON_A, 200000);
    
    attachInterrupt(BUTTON_B, showImageB, FALLING);
    setDebounce(BUTTON_B, 200000);
    
    Serial.println("Clear Screen");
    // clear the screen
    int temperature = 24;   // fake it unitil we have either MPU or SAM3X temp working
    EPD.begin();
    EPD.setFactor(temperature);
    EPD.clear();
    EPD.end();
    delay(100);
    Serial.println("Clear Screen");
    // clear the screen
    EPD.begin();
    EPD.setFactor(temperature);
    EPD.clear();
    EPD.end();
    
    Serial.println("Start Loop");
}

void loop() {
    
    if (setImage != currentImage){
        Serial.print("Button Pressed: ");
        // lets update the screen
        int temperature = 24;   // fake it unitil we have either MPU or SAM3X temp working
        EPD.begin(); // power up the EPD panel
        EPD.setFactor(temperature); // adjust for current temperature
        
        switch (setImage) {
            case IMAGE_A:
            Serial.println("A");
            if (currentImage)
            EPD.image(IMAGE_A_BITS, IMAGE_B_BITS);
            else
            EPD.image(IMAGE_A_BITS);
            currentImage = IMAGE_A;
            break;
            case IMAGE_B:
            Serial.println("B");
            if (currentImage)
            EPD.image(IMAGE_B_BITS, IMAGE_A_BITS);
            else
            EPD.image(IMAGE_B_BITS);
            currentImage = IMAGE_B;
            break;
            
            default:
            break;
        }
        
        EPD.end();   // power down the EPD panel
    }
    
}

void setDebounce(int pin, int usecs){  // reject spikes shorter than usecs on pin
    if(usecs){
        g_APinDescription[pin].pPort -> PIO_IFER = g_APinDescription[pin].ulPin;
        g_APinDescription[pin].pPort -> PIO_DIFSR |= g_APinDescription[pin].ulPin;
    }
    else {
        g_APinDescription[pin].pPort -> PIO_IFDR = g_APinDescription[pin].ulPin;
        g_APinDescription[pin].pPort -> PIO_DIFSR &=~ g_APinDescription[pin].ulPin;
        return;
    }
    
    
    int div=(usecs/31)-1; if(div<0)div=0; if(div > 16383) div=16383;
    g_APinDescription[pin].pPort -> PIO_SCDR = div;
    
}


void showImageA(){
    setImage = IMAGE_A;
}

void showImageB(){
    setImage = IMAGE_B;
}
