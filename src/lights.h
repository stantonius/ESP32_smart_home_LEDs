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

void simple()
{
    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds[i] = CHSV(hue, 255, 100);
    }

    EVERY_N_SECONDS(2)
    {
        hue = random8(198, 250);
    }

    FastLED.show();
}

void pride()
{
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

void blurry()
{
    uint8_t sinBeat = beatsin8(30, 0, NUM_LEDS - 1, 0, 0);
    uint8_t sinBeat2 = beatsin8(30, 0, NUM_LEDS - 1, 0, 85);
    uint8_t sinBeat3 = beatsin8(30, 0, NUM_LEDS - 1, 0, 170);

    leds[sinBeat] = CHSV(random8(198, 250), 255, 100);
    leds[sinBeat2] = CHSV(random8(198, 250), 255, 100);
    leds[sinBeat3] = CHSV(random8(198, 250), 255, 100);

    EVERY_N_MILLISECONDS(25)
    {
        for (int i = 0; i < 4; i++)
        {
            blur1d(leds, NUM_LEDS, 50);
        }
    }

    // fadeToBlackBy(leds, NUM_LEDS, 10);

    FastLED.show();
}

void lighter(bool show)
{
    if (show)
    {

        // simple();
        // blurry();

        pride();
        FastLED.show();
    }
    else
    {
        FastLED.clear();
        FastLED.show();
    }
}
