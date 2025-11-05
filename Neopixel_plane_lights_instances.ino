// Visual Micro is in vMicro>General>Tutorial Mode
// 
/*
	Name:       Neopixel_plane_lights.ino
	Created:	10/31/2025 11:39:15 PM
	Author:     MARCIN_DELL_G16\sp5io
*/
#define VERSION "Neopixel_plane_lights v1.0.0"
#include <Adafruit_NeoPixel.h>
#include "colors.h"

//#ifdef __AVR__
//#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
//#endif

//#define DEBUG_SERIAL

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN_LEFT     11
#define LED_PIN_RIGHT	 12
#define LED_PIN_BACK	 13
#define LED_COUNT_LEFT 18
#define LED_COUNT_RIGHT 18

#define CONTROL_PIN0 9
#define CONTROL_PIN1 10
//const uint16_t pulsesOriginal[] = { 982, 1042,1116,1188,1260,1332, 2000 }; //1-6 pot pulses durations
const uint16_t pulses[] = { 1000, 1030,1106,1178,1250,1322, 2000 }; //1-6 pot pulses durations
//struct LedsNumbers {
//	uint8_t startLed;
//	uint8_t endLed;
//};

const LedsNumbers frontLeft = { 0, 2 };
const LedsNumbers frontRight = { 0, 2 };
const LedsNumbers positionLeftDown = { 3, 11 };
const LedsNumbers positionRightDown = { 3, 11 };
const LedsNumbers blinkLeftDown = { 12, 12 };
const LedsNumbers blinkRightDown = { 12, 12 };
const LedsNumbers blinkLeftUp = { 13, 13 };
const LedsNumbers blinkRightUp = { 13, 13 };
const LedsNumbers positionLeftUp = { 14, 17 };
const LedsNumbers positionRightUp = { 14, 17 };
const LedsNumbers positionLeftBack = { 0, 0 };
const LedsNumbers positionCenterBack = { 1, 1 };
const LedsNumbers positionRightBack = { 2, 2 };


// How many NeoPixels are attached to the Arduino?
#define LED_COUNT_LEFT  18
#define LED_COUNT_RIGHT  18
#define LED_COUNT_BACK  3

// NeoPixel brightness, 0 (min) to 255 (max)
//#define BRIGHTNESS 50 // Set BRIGHTNESS to about 1/5 (max = 255)
#define BRIGHTNESS 255 // Set BRIGHTNESS to about 1/2 (max = 255)

// Declare our NeoPixel strip object:
//For WS2812
//Adafruit_NeoPixel stripLeft(LED_COUNT_LEFT, LED_PIN_LEFT, NEO_GRB + NEO_KHZ800);
//Adafruit_NeoPixel stripRight(LED_COUNT_RIGHT, LED_PIN_RIGHT, NEO_GRB + NEO_KHZ800);
//For WS2811
Adafruit_NeoPixel stripLeft(LED_COUNT_LEFT, LED_PIN_LEFT, NEO_BRG + NEO_KHZ800);
Adafruit_NeoPixel stripRight(LED_COUNT_RIGHT, LED_PIN_RIGHT, NEO_BRG + NEO_KHZ800);
Adafruit_NeoPixel stripBack(LED_COUNT_BACK, LED_PIN_BACK, NEO_BRG + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:


void lightsControl(int controlPin) {
	pinMode(controlPin, INPUT);
	long pulseDuration = pulseIn(controlPin, HIGH);
#ifdef DEBUG_SERIAL
	Serial.print(F("Pulse duration: "));
	Serial.println(pulseDuration);
#endif
	clearAllLeds();

	if (pulseDuration < pulses[1]) {
		// turn off all lights
#ifdef DEBUG_SERIAL
		Serial.println(F("clearAllLeds"));
#endif
		clearAllLeds();
	}
	else if (pulseDuration < pulses[2]) {
		// front lights on
		//clearAllLeds();
#ifdef DEBUG_SERIAL
		Serial.println(F("frontLampsOn"));
#endif
		frontLampsOn();
	}
	else if (pulseDuration < pulses[3]) {
		// front lights on
		//clearAllLeds();
#ifdef DEBUG_SERIAL
		Serial.println(F("positionLampsOn"));
#endif
		frontLampsOn();
		positionLampsOn();
	}
	else if (pulseDuration < pulses[4]) {
		// front lights on
#ifdef DEBUG_SERIAL
		Serial.println(F("blinkingLamps"));
#endif
		frontLampsOn();
		positionLampsOn();
		blinkingLamps();
	}
	else if (pulseDuration < pulses[5]) {
		// all lights on + blinking no rainbow
#ifdef DEBUG_SERIAL
		Serial.println(F("snakeShow"));
#endif
		frontLampsOn();
		snakeShow();
		blinkingLamps();
	}
	else if (pulseDuration < pulses[6]) {
		// all lights on + blinking + rainbow
#ifdef DEBUG_SERIAL
		Serial.println(F("button 6"));
#endif
		frontLampsOn();
		snakeShow();
		blinkingLamps();
	}
}

static void setLedRGB(Adafruit_NeoPixel& strip, uint8_t led, uint8_t r, uint8_t g, uint8_t b)
{
	strip.setPixelColor(led, strip.Color(r, g, b));
}


void clearAllLeds()
{
	stripLeft.clear();
	stripLeft.show();
	stripRight.clear();
	stripRight.show();
	stripBack.clear();
	stripBack.show();
}

void frontLampsOn()
{
	// front lamps
	for (uint8_t i = frontLeft.startLed; i <= frontLeft.endLed; i++)
		setLedRGB(stripLeft, i, 255, 255, 255);   // front left white
	for (uint8_t i = frontRight.startLed; i <= frontRight.endLed; i++)
		setLedRGB(stripRight, i, 255, 255, 255);   // front Right white
	stripLeft.show();
	stripRight.show();
}

void positionLampsOn()
{
	// front lamps
	for (uint8_t i = positionLeftDown.startLed; i <= positionLeftDown.endLed; i++)
		setLedColor(stripLeft, i, COLOR_RED);
	for (uint8_t i = positionRightDown.startLed; i <= positionRightDown.endLed; i++)
		setLedColor(stripRight, i, COLOR_GREEN);
	for (uint8_t i = positionLeftUp.startLed; i <= positionLeftUp.endLed; i++)
		setLedColor(stripLeft, i, COLOR_RED);
	for (uint8_t i = positionRightUp.startLed; i <= positionRightUp.endLed; i++)
		setLedColor(stripRight, i, COLOR_GREEN);
	// back lamps
	setLedColor(stripBack, positionLeftBack.startLed, COLOR_RED);       // left back red
	setLedColor(stripBack, positionCenterBack.startLed, COLOR_BLUE);  // center back blue
	setLedColor(stripBack, positionRightBack.startLed, COLOR_GREEN);    // right back
	stripLeft.show();
	stripRight.show();
	stripBack.show();
}

void blinkingLamps() {
	//Blinking lamps
	for (uint8_t i = blinkLeftDown.startLed; i <= blinkLeftDown.endLed; i++)
		setLedColor(stripLeft, i, COLOR_BLUE);
	for (uint8_t i = blinkRightDown.startLed; i <= blinkRightDown.endLed; i++)
		setLedColor(stripRight, i, COLOR_BLUE);
	for (uint8_t i = positionCenterBack.startLed; i <= positionCenterBack.endLed; i++)
		setLedColor(stripBack, positionCenterBack.startLed, COLOR_BLUE);
	stripLeft.show();
	stripRight.show();
	stripBack.show();
	delay(100);
	for (uint8_t i = blinkLeftDown.startLed; i <= blinkLeftDown.endLed; i++)
		setLedColor(stripLeft, i, COLOR_BLACK);
	for (uint8_t i = blinkRightDown.startLed; i <= blinkRightDown.endLed; i++)
		setLedColor(stripRight, i, COLOR_BLACK); // right upper black (Off)
	for (uint8_t i = positionCenterBack.startLed; i <= positionCenterBack.endLed; i++)
		setLedColor(stripBack, positionCenterBack.startLed, COLOR_BLACK); //back center off
	stripLeft.show();
	stripRight.show();
	stripBack.show();
	delay(1000);

	for (uint8_t i = blinkLeftUp.startLed; i <= blinkLeftUp.endLed; i++)
		setLedColor(stripLeft, i, COLOR_WHITE);  // blinkLeft Up white
	for (uint8_t i = blinkRightUp.startLed; i <= blinkRightUp.endLed; i++)
		setLedColor(stripRight, i, COLOR_WHITE);  // blinkRight Up white
	for (uint8_t i = positionCenterBack.startLed; i <= positionCenterBack.endLed; i++)
		setLedColor(stripBack, positionCenterBack.startLed, COLOR_WHITE);   // blinkRight Up white
	stripLeft.show();
	stripRight.show();
	stripBack.show();
	delay(100);
	for (uint8_t i = blinkLeftUp.startLed; i <= blinkLeftUp.endLed; i++)
		setLedColor(stripLeft, i, COLOR_BLACK); // blink Left Up off
	for (uint8_t i = blinkRightUp.startLed; i <= blinkRightUp.endLed; i++)
		setLedColor(stripRight, i, COLOR_BLACK); // blink Right Up off
	for (uint8_t i = positionCenterBack.startLed; i <= positionCenterBack.endLed; i++)
		setLedColor(stripBack, positionCenterBack.startLed, COLOR_BLACK);
	stripLeft.show();
	stripRight.show();
	stripBack.show();
	delay(1000);
}

void snakeShow()
{

	for (uint8_t i = positionLeftUp.endLed; i >= positionLeftUp.startLed; i--) {
		setLedColor(stripLeft, i, COLOR_BLUE);	 // positio left up blue
		setLedColor(stripRight, i, COLOR_BLUE);	 // positio right up blue
		if (i == positionLeftUp.endLed) {
			setLedColor(stripBack, positionRightBack.startLed, COLOR_BLUE); // right back blue
			setLedColor(stripBack, positionLeftBack.startLed, COLOR_BLUE); // right back blue
			stripBack.show();
		}
		stripLeft.show();
		stripRight.show();
		stripBack.show();
		delay(100);
	}
	for (uint8_t i = positionLeftDown.endLed; i >= positionLeftDown.startLed; i--) {
		setLedColor(stripLeft, i, COLOR_BLUE);	 // position left down blue
		setLedColor(stripRight, i, COLOR_BLUE);	 // front right down blue
		stripLeft.show();
		stripRight.show();
		delay(100);
	}
	for (uint8_t i = positionLeftDown.startLed; i <= positionLeftDown.endLed; i++) {
		setLedColor(stripLeft, i, COLOR_RED);   // position left down red
		if (i == positionLeftDown.startLed) {
			setLedColor(stripBack, positionLeftBack.startLed, COLOR_RED); // left back red
			setLedColor(stripBack, positionRightBack.startLed, COLOR_GREEN); // right back green
			stripBack.show();
		}
		stripLeft.show();
		stripRight.show();
		delay(100);
	}
	for (uint8_t i = positionLeftUp.startLed; i <= positionLeftUp.endLed; i++) {
		setLedColor(stripLeft, i, COLOR_RED);   // front left up red
		setLedColor(stripRight, i, COLOR_GREEN);   // front right up green
		setLedColor(stripRight, i, COLOR_GREEN);   // position right down green
		stripLeft.show();
		stripRight.show();
		delay(100);
	}
}


// The setup() function runs once each time the micro-controller starts
void setup()
{
#ifdef DEBUG_SERIAL
	Serial.begin(115200);
	Serial.println(VERSION);
#endif

	stripLeft.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
	stripLeft.show();            // Turn OFF all pixels ASAP
	stripLeft.setBrightness(BRIGHTNESS);

	stripRight.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
	stripRight.show();            // Turn OFF all pixels ASAP
	stripRight.setBrightness(BRIGHTNESS);

	stripBack.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
	stripBack.show();            // Turn OFF all pixels ASAP
	stripBack.setBrightness(BRIGHTNESS);
}

// Add the main program code into the continuous loop() function
void loop()
{
	lightsControl(CONTROL_PIN0);

}

