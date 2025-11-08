// Visual Micro is in vMicro>General>Tutorial Mode
// 
/*
	Name:       Neopixel_plane_lights.ino
	Created:	10/31/2025 11:39:15 PM
	Author:     MARCIN_DELL_G16\sp5io
*/
#define VERSION "Neopixel_plane_lights v1.0.0 20251108"
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

// Global palette shift index used by both snakeColorForPos() and gradientSnakeShow()
static uint8_t paletteIndexShift = 0;


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
		setLedColor(stripLeft, i, COLOR_WHITE);   // front left white
	for (uint8_t i = frontRight.startLed; i <= frontRight.endLed; i++)
		setLedColor(stripRight, i, COLOR_WHITE);   // front Right white
	stripLeft.show();
	stripRight.show();
}

void positionLampsOn()
{
	// Down side lamps
	for (uint8_t i = positionLeftDown.startLed; i <= positionLeftDown.endLed; i++)
		setLedColor(stripLeft, i, COLOR_RED);
	for (uint8_t i = positionRightDown.startLed; i <= positionRightDown.endLed; i++)
		setLedColor(stripRight, i, COLOR_GREEN);
	// Up side lamps
	for (uint8_t i = positionLeftUp.startLed; i <= positionLeftUp.endLed; i++)
		setLedColor(stripLeft, i, COLOR_RED);
	for (uint8_t i = positionRightUp.startLed; i <= positionRightUp.endLed; i++)
		setLedColor(stripRight, i, COLOR_GREEN);
	// back lamps
	setLedColor(stripBack, positionLeftBack.startLed, COLOR_RED);       // left back red
	setLedColor(stripBack, positionCenterBack.startLed, COLOR_BLACK);  // center back off this section is for blibnking lamps
	setLedColor(stripBack, positionRightBack.startLed, COLOR_GREEN);    // right back
	stripLeft.show();
	stripRight.show();
	stripBack.show();
}

// ============================================================
// Non-blocking blinking lamps (synchronized back & wings)
// Pattern: BLUE 100 ms → OFF 1000 ms → WHITE 100 ms → OFF 1000 ms
// ============================================================

void blinkingLamps() {
	static uint8_t phase = 0;
	static unsigned long lastChange = 0;
	unsigned long now = millis();

	const unsigned long shortOn = 100;
	const unsigned long longOff = 1000;

	switch (phase) {
		// ---------------------------------------------------------
		// 0: BLUE ON 100 ms  (wings + back)
	case 0:
		for (uint8_t i = blinkLeftDown.startLed; i <= blinkLeftDown.endLed; i++) setLedColor(stripLeft, i, COLOR_BLUE);
		for (uint8_t i = blinkRightDown.startLed; i <= blinkRightDown.endLed; i++) setLedColor(stripRight, i, COLOR_BLUE);
		for (uint8_t i = positionCenterBack.startLed; i <= positionCenterBack.endLed; i++) setLedColor(stripBack, i, COLOR_BLUE);
		stripLeft.show(); stripRight.show(); stripBack.show();
		lastChange = now;
		phase = 1;
		break;

		// ---------------------------------------------------------
		// 1: OFF after BLUE
	case 1:
		if (now - lastChange >= shortOn) {
			for (uint8_t i = blinkLeftDown.startLed; i <= blinkLeftDown.endLed; i++) setLedColor(stripLeft, i, COLOR_BLACK);
			for (uint8_t i = blinkRightDown.startLed; i <= blinkRightDown.endLed; i++) setLedColor(stripRight, i, COLOR_BLACK);
			for (uint8_t i = positionCenterBack.startLed; i <= positionCenterBack.endLed; i++) setLedColor(stripBack, i, COLOR_BLACK);
			stripLeft.show(); stripRight.show(); stripBack.show();
			lastChange = now;
			phase = 2;
		}
		break;

		// ---------------------------------------------------------
		// 2: wait 1000 ms before WHITE flash
	case 2:
		if (now - lastChange >= longOff) {
			for (uint8_t i = blinkLeftUp.startLed; i <= blinkLeftUp.endLed; i++) setLedColor(stripLeft, i, COLOR_WHITE);
			for (uint8_t i = blinkRightUp.startLed; i <= blinkRightUp.endLed; i++) setLedColor(stripRight, i, COLOR_WHITE);
			for (uint8_t i = positionCenterBack.startLed; i <= positionCenterBack.endLed; i++) setLedColor(stripBack, i, COLOR_WHITE);
			stripLeft.show(); stripRight.show(); stripBack.show();
			lastChange = now;
			phase = 3;
		}
		break;

		// ---------------------------------------------------------
		// 3: OFF after WHITE
	case 3:
		if (now - lastChange >= shortOn) {
			for (uint8_t i = blinkLeftUp.startLed; i <= blinkLeftUp.endLed; i++) setLedColor(stripLeft, i, COLOR_BLACK);
			for (uint8_t i = blinkRightUp.startLed; i <= blinkRightUp.endLed; i++) setLedColor(stripRight, i, COLOR_BLACK);
			for (uint8_t i = positionCenterBack.startLed; i <= positionCenterBack.endLed; i++) setLedColor(stripBack, i, COLOR_BLACK);
			stripLeft.show(); stripRight.show(); stripBack.show();
			lastChange = now;
			phase = 4;
		}
		break;

		// ---------------------------------------------------------
		// 4: wait 1000 ms before next BLUE flash (loop)
	case 4:
		if (now - lastChange >= longOff) {
			phase = 0;
		}
		break;
	}
}


#define SNAKE_COLOR         0
#define SNAKE_GRADIENT      1
#define SNAKE_GRADIENT_FLOW 2

// Color for a given LED position (solid, static gradient, or palette-flow gradient)
static inline uint32_t snakeColorForPos(Adafruit_NeoPixel& strip,
	const LedsNumbers& sec,
	int16_t pos,
	uint8_t mode,
	uint8_t colorStart,
	uint8_t colorEnd,
	uint16_t hueShift)
{
	switch (mode) {
	case SNAKE_COLOR:
		return paletteColor(strip, colorStart);

	case SNAKE_GRADIENT: {
		const uint8_t count = sec.endLed - sec.startLed + 1;
		const float t = (float)(pos - sec.startLed) / (float)(count - 1);
		const NamedColor& a = PALETTE[colorStart % PALETTE_SIZE];
		const NamedColor& b = PALETTE[colorEnd % PALETTE_SIZE];
		const uint8_t r = a.r + (int)((b.r - a.r) * t);
		const uint8_t g = a.g + (int)((b.g - a.g) * t);
		const uint8_t bb = a.b + (int)((b.b - a.b) * t);
		return strip.gamma32(strip.Color(r, g, bb));
	}

	case SNAKE_GRADIENT_FLOW: {
		// Discrete palette-based flowing gradient (no HSV rainbow)
		const uint8_t n = sec.endLed - sec.startLed + 1;
		const uint8_t i = (uint8_t)(pos - sec.startLed);  // local index 0..n-1
		const uint8_t paletteSize = PALETTE_SIZE;

		// Use the file-scope paletteIndexShift to advance the flow over time
		const uint8_t shift = paletteIndexShift % paletteSize;

		// Adjacent palette indices for a smooth step between named colors
		const uint8_t indexA = (shift + i) % paletteSize;
		const uint8_t indexB = (indexA + 1) % paletteSize;

		const NamedColor& a = PALETTE[indexA];
		const NamedColor& b = PALETTE[indexB];

		// Mix between A and B (0.0 = sharp step, 0.5 = mid-mix)
		const float t = 0.5f;

		const uint8_t r = a.r + (int)((b.r - a.r) * t);
		const uint8_t g = a.g + (int)((b.g - a.g) * t);
		const uint8_t bb = a.b + (int)((b.b - a.b) * t);
		return strip.gamma32(strip.Color(r, g, bb));
	}
	}
	// Fallback
	return paletteColor(strip, colorStart);
}

void gradientSnakeShow(uint8_t mode = SNAKE_COLOR,
	uint8_t colorStart = COLOR_BLUE,
	uint8_t colorEnd = COLOR_VIOLET,
	uint16_t stepTime = 100)
{
	enum Phase : uint8_t { BUILD_UP, BUILD_DOWN, BUILD_BACK, RETURN_UP, RETURN_DOWN, SOLIDIFY };

	static int16_t iBackL = -1; // left back build index
	static int16_t iBackR = -1; // right back build index


	static Phase         phase = BUILD_UP;
	static int16_t       iUp = -1;     // upper section build index
	static int16_t       iDown = -1;     // lower section build / return index
	static uint16_t      hueShift = 0;      // (kept for other modes if needed)
	static unsigned long lastStep = 0;

	const unsigned long now = millis();
	if (now - lastStep < stepTime) return;
	lastStep = now;

	// Advance palette flow (used by SNAKE_GRADIENT_FLOW in snakeColorForPos)
	paletteIndexShift = (paletteIndexShift + 1) % PALETTE_SIZE;

	// ===== BUILD_UP: upper sections fill (no clearing) =====
	if (phase == BUILD_UP) {
		if (iUp < 0) iUp = positionLeftUp.startLed;

		// LEFT upper: forward direction
		for (int16_t p = positionLeftUp.startLed; p <= iUp; ++p) {
			stripLeft.setPixelColor(
				p, snakeColorForPos(stripLeft, positionLeftUp, p, mode, colorStart, colorEnd, hueShift)
			);
		}

		// RIGHT upper: reversed direction (mirror local index)
		for (int16_t p = positionRightUp.startLed; p <= iUp && p <= positionRightUp.endLed; ++p) {
			const int16_t mirrored =
				positionRightUp.endLed - (p - positionRightUp.startLed);
			stripRight.setPixelColor(
				p, snakeColorForPos(stripRight, positionRightUp, mirrored, mode, colorStart, colorEnd, hueShift)
			);
		}

		stripLeft.show();
		stripRight.show();

		if (iUp < positionLeftUp.endLed) { ++iUp; return; }

		// Upper done → init lower
		phase = BUILD_DOWN;
		iDown = positionLeftDown.startLed;

		return;
	}

   // ===== BUILD_DOWN: lower sections fill (no clearing) =====
	if (phase == BUILD_DOWN) {
		for (int16_t p = positionLeftDown.startLed; p <= iDown; ++p) {
			stripLeft.setPixelColor(p, snakeColorForPos(stripLeft, positionLeftDown, p, mode, colorStart, colorEnd, hueShift));
			stripRight.setPixelColor(p, snakeColorForPos(stripRight, positionRightDown, p, mode, colorStart, colorEnd, hueShift));
		}
		stripLeft.show(); stripRight.show();

		if (iDown < positionLeftDown.endLed) { ++iDown; return; }

		// Lower sections done → now build BACK as last position part
		phase = BUILD_BACK;
		iBackL = positionLeftBack.startLed;
		iBackR = positionRightBack.endLed;
		return;
	}

	// ===== BUILD_BACK: back sections as last part of position, opposite directions =====
	if (phase == BUILD_BACK) {
		bool leftDone = false;
		bool rightDone = false;

		// LEFT back: forward direction
		if (iBackL <= positionLeftBack.endLed) {
			for (int16_t p = positionLeftBack.startLed; p <= iBackL; ++p) {
				stripBack.setPixelColor(
					p, snakeColorForPos(stripBack, positionLeftBack, p, mode, colorStart, colorEnd, hueShift)
				);
			}
			++iBackL;
		}
		else {
			leftDone = true;
		}

		// RIGHT back: reversed direction
		if (iBackR >= positionRightBack.startLed) {
			for (int16_t p = positionRightBack.endLed; p >= iBackR; --p) {
				const int16_t mirrored =
					positionRightBack.startLed + (positionRightBack.endLed - p);
				stripBack.setPixelColor(
					p, snakeColorForPos(stripBack, positionRightBack, mirrored, mode, colorStart, colorEnd, hueShift)
				);
			}
			--iBackR;
		}
		else {
			rightDone = true;
		}

		stripBack.show();

		if (!(leftDone && rightDone)) return;

		// Back sections done → now go to RETURN_UP
		phase = RETURN_UP;
		iUp = positionLeftUp.startLed;
		return;
	}


	// ===== RETURN_UP: progressive swap to position colors (no clearing) =====
	if (phase == RETURN_UP) {
		stripLeft.setPixelColor(iUp, paletteColor(stripLeft, COLOR_RED));
		stripRight.setPixelColor(iUp, paletteColor(stripRight, COLOR_GREEN));
		stripLeft.show(); stripRight.show();

		if (iUp < positionLeftUp.endLed) { ++iUp; return; }

		// Upper done → move to lower and set back to position colors
		phase = RETURN_DOWN;
		iDown = positionLeftDown.startLed;

		fillSolid(stripBack, positionLeftBack, COLOR_RED);
		fillSolid(stripBack, positionRightBack, COLOR_GREEN);
		stripBack.show();
		return;
	}

	// ===== RETURN_DOWN: progressive swap to position colors (no clearing) =====
	if (phase == RETURN_DOWN) {
		stripLeft.setPixelColor(iDown, paletteColor(stripLeft, COLOR_RED));
		stripRight.setPixelColor(iDown, paletteColor(stripRight, COLOR_GREEN));
		stripLeft.show(); stripRight.show();

		if (iDown < positionLeftDown.endLed) { ++iDown; return; }

		// Lower done → finalize and ensure full solid position colors
		phase = SOLIDIFY;
		return;
	}

	// ===== SOLIDIFY: enforce full position lights and restart (or keep if you want) =====
	if (phase == SOLIDIFY) {
		fillSolid(stripLeft, positionLeftUp, COLOR_RED);
		fillSolid(stripLeft, positionLeftDown, COLOR_RED);
		fillSolid(stripRight, positionRightUp, COLOR_GREEN);
		fillSolid(stripRight, positionRightDown, COLOR_GREEN);
		fillSolid(stripBack, positionLeftBack, COLOR_RED);
		fillSolid(stripBack, positionRightBack, COLOR_GREEN);
		stripLeft.show(); stripRight.show(); stripBack.show();

		// Restart cycle; comment out next lines if you want to stop on positions
		phase = BUILD_UP;
		iUp = -1;
		iDown = -1;
		return;
	}
}

// fill section with solid color from palette (gamma)
static inline void fillSolid(Adafruit_NeoPixel& strip,
	const LedsNumbers& sec,
	uint8_t colorIndex)
{
	const uint32_t c = paletteColor(strip, colorIndex);
	for (uint8_t p = sec.startLed; p <= sec.endLed; ++p) strip.setPixelColor(p, c);
}

void lightsControl(int controlPin) {
	pinMode(controlPin, INPUT);
	long pulseDuration = pulseIn(controlPin, HIGH);
#ifdef DEBUG_SERIAL
	Serial.print(F("Pulse duration: "));
	Serial.println(pulseDuration);
#endif
	//clearAllLeds();

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
		clearAllLeds();
		frontLampsOn();
	}
	else if (pulseDuration < pulses[3]) {
		// front lights on
		//clearAllLeds();
#ifdef DEBUG_SERIAL
		Serial.println(F("positionLampsOn"));
#endif
		clearAllLeds();
		frontLampsOn();
		positionLampsOn();
	}
	else if (pulseDuration < pulses[4]) {
		// front lights on
#ifdef DEBUG_SERIAL
		Serial.println(F("blinkingLamps"));
#endif
		clearAllLeds();
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
		gradientSnakeShow(SNAKE_COLOR, COLOR_BLUE, COLOR_BLUE, 100);
		blinkingLamps();
	}
	else if (pulseDuration < pulses[6]) {
		// all lights on + blinking + rainbow
#ifdef DEBUG_SERIAL
		Serial.println(F("button 6"));
#endif
		frontLampsOn();
		gradientSnakeShow(SNAKE_GRADIENT_FLOW, COLOR_ORANGE, COLOR_PINK, 100);
		blinkingLamps();
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

