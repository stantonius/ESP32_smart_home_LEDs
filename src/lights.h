#pragma once
#include <FastLED.h>

// How many leds in your pixels?
#define NUM_LEDS 68
#define DATA_PIN 33
#define BRIGHTNESS 50

CRGB leds[NUM_LEDS];
uint8_t paletteIndex = 0;

uint8_t hue = 200;

// Gradient palette "bhw1_01_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw1/tn/bhw1_01.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 12 bytes of program space.

DEFINE_GRADIENT_PALETTE(bhw1_01_gp){
    0, 150, 55, 214,
    227, 253, 29, 29,
    255, 150, 55, 214};

CRGBPalette16 myPal = bhw1_01_gp;

void lightSetups()
{
    FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection(0xFFB0F0);
    FastLED.setBrightness(BRIGHTNESS);
    FastLED.setCorrection(TypicalLEDStrip);
    // FastLED.setTemperature(FullSpectrumFluorescent);
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

    uint16_t hue16 = sHue16; //gHue * 256;
    uint16_t hueinc16 = beatsin88(113, 1, 3000);

    uint16_t ms = millis();
    uint16_t deltams = ms - sLastMillis;
    sLastMillis = ms;
    sPseudotime += deltams * msmultiplier;
    sHue16 += deltams * beatsin88(400, 5, 9);
    uint16_t brightnesstheta16 = sPseudotime;

    for (uint16_t i = 0; i < NUM_LEDS; i++)
    {
        hue16 += hueinc16;
        uint8_t hue8 = hue16 / 256;

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
