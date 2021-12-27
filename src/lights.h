#pragma once
#include <FastLED.h>

// https://github.com/FastLED/FastLED/blob/master/examples/ColorPalette/ColorPalette.ino
// https://github.com/FastLED/FastLED/blob/master/examples/Pride2015/Pride2015.ino

// How many leds in your pixels?
#define NUM_LEDS 68
#define DATA_PIN 33
#define BRIGHTNESS 125

#define UPDATES_PER_SECOND 20

CRGB leds[NUM_LEDS];
uint8_t colourIndex[NUM_LEDS];

uint8_t hue;

void lightSetups()
{
    // FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection(0xFFB0F0);
    FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
    FastLED.setBrightness(BRIGHTNESS);
    FastLED.setCorrection(TypicalLEDStrip);
    // FastLED.setTemperature(FullSpectrumFluorescent);
    hue = random8(198, 250);
}

void pride()
{
    // tweaked from official FastLED example - some code may no longer be needed
    static uint16_t sPseudotime = 0;
    static uint16_t sLastMillis = 0;
    static uint16_t sHue16 = 0;

    uint8_t sat8 = beatsin88(87, 220, 250);
    uint8_t brightdepth = beatsin88(341, 96, 224);
    uint16_t brightnessthetainc16 = beatsin88(203, (25 * 256), (40 * 256));
    uint8_t msmultiplier = beatsin88(147, 23, 60);

    uint16_t hue16 = sHue16; // gHue * 256;
    uint16_t hueinc16 = beatsin88(113, 198, 250);

    uint16_t ms = millis();
    uint16_t deltams = ms - sLastMillis;
    sLastMillis = ms;
    sPseudotime += deltams * msmultiplier;
    sHue16 += deltams * beatsin88(400, 5, 9);
    uint16_t brightnesstheta16 = sPseudotime;

    for (uint16_t i = 0; i < NUM_LEDS; i++)
    {
        hue16 += hueinc16;
        uint8_t hue8 = hueinc16;

        brightnesstheta16 += brightnessthetainc16;
        uint16_t b16 = sin16(brightnesstheta16) + 32768;

        uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
        uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
        bri8 += (255 - brightdepth);

        CRGB newcolor = CHSV(hue8, sat8, bri8);

        uint16_t pixelnumber = i;
        pixelnumber = (NUM_LEDS - 1) - pixelnumber;

        nblend(leds[pixelnumber], newcolor, 64);
    }
}

void lighter(bool show)
{
    if (show)
    {
        pride();
        FastLED.show();
    }
    else
    {
        FastLED.clear();
        FastLED.show();
    }
}
