// ============================================================
// colors.h — Named color palette with gamma correction for NeoPixels
// + gradient and section helpers
// ============================================================

#pragma once
#include <Adafruit_NeoPixel.h>

// ---------- Color structure ----------
struct NamedColor {
    const char* name;
    uint8_t r, g, b;
};

// ---------- Color palette ----------
static const NamedColor PALETTE[] = {
  {"Black",        0,   0,   0},     // 0
  {"Brown",        150, 75,  0},     // 1
  {"Red",          255, 0,   0},     // 2
  {"Orange",       255, 80,  0},     // 3
  {"Yellow",       255, 180, 0},     // 4
  {"Green",        0,   255, 0},     // 5
  {"Blue",         0,   0,   255},   // 6
  {"Violet",       180, 0,   255},   // 7
  {"Grey",         130, 130, 130},   // 8
  {"White",        255, 255, 255},   // 9
  {"Amber",        255, 120, 0},     // 10
  {"Lime",         170, 255, 0},     // 11
  {"SpringGreen",  0,   255, 120},   // 12
  {"Cyan",         0,   200, 255},   // 13
  {"DeepSkyBlue",  0,   150, 255},   // 14
  {"RoyalBlue",    0,   70,  255},   // 15
  {"Indigo",       90,  0,   200},   // 16
  {"Magenta",      255, 0,   180},   // 17
  {"Pink",         255, 80,  160},   // 18
  {"WarmWhite",    255, 220, 170},   // 19
  {"ColdWhite",    200, 220, 255}    // 20
};

static const uint8_t PALETTE_SIZE = sizeof(PALETTE) / sizeof(PALETTE[0]);

// ---------- Color index definitions ----------
#define COLOR_BLACK        0
#define COLOR_BROWN        1
#define COLOR_RED          2
#define COLOR_ORANGE       3
#define COLOR_YELLOW       4
#define COLOR_GREEN        5
#define COLOR_BLUE         6
#define COLOR_VIOLET       7
#define COLOR_GREY         8
#define COLOR_WHITE        9
#define COLOR_AMBER        10
#define COLOR_LIME         11
#define COLOR_SPRINGGREEN  12
#define COLOR_CYAN         13
#define COLOR_DEEPSKYBLUE  14
#define COLOR_ROYALBLUE    15
#define COLOR_INDIGO       16
#define COLOR_MAGENTA      17
#define COLOR_PINK         18
#define COLOR_WARMWHITE    19
#define COLOR_COLDWHITE    20

// ---------- Section structure ----------
struct LedsNumbers {
    uint8_t startLed;
    uint8_t endLed;
};

// ---------- Utility: convert palette index -> gamma-corrected color ----------
static inline uint32_t paletteColor(Adafruit_NeoPixel& strip, uint8_t idx) {
    const NamedColor& c = PALETTE[idx % PALETTE_SIZE];
    return strip.gamma32(strip.Color(c.r, c.g, c.b));
}

// ---------- Set a single pixel color by palette index ----------
static inline void setLedColor(Adafruit_NeoPixel& strip, uint8_t led, uint8_t colorIndex) {
    strip.setPixelColor(led, paletteColor(strip, colorIndex));
}

// ---------- Fill section with a solid color ----------
static inline void fillSectionPalette(Adafruit_NeoPixel& strip,
    const LedsNumbers& section,
    uint8_t colorIndex) {
    uint32_t c = paletteColor(strip, colorIndex);
    for (uint8_t i = section.startLed; i <= section.endLed; i++)
        strip.setPixelColor(i, c);
}

// ---------- Fill section with gradient between two palette colors ----------
static inline void fillSectionGradient(Adafruit_NeoPixel& strip,
    const LedsNumbers& section,
    uint8_t colorStart,
    uint8_t colorEnd) {
    const NamedColor& a = PALETTE[colorStart % PALETTE_SIZE];
    const NamedColor& b = PALETTE[colorEnd % PALETTE_SIZE];

    uint8_t count = section.endLed - section.startLed + 1;
    for (uint8_t i = 0; i < count; i++) {
        float t = (float)i / (float)(count - 1); // 0.0 → 1.0
        uint8_t r = a.r + (b.r - a.r) * t;
        uint8_t g = a.g + (b.g - a.g) * t;
        uint8_t bcol = a.b + (b.b - a.b) * t;
        uint32_t color = strip.gamma32(strip.Color(r, g, bcol));
        strip.setPixelColor(section.startLed + i, color);
    }
}
