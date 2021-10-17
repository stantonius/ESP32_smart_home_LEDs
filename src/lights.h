#pragma once
#include <FastLED.h>

// How many leds in your strip?
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
    FastLED.setCorrection(TypicalPixelString);
    FastLED.setTemperature(FullSpectrumFluorescent);
}

void lighter(bool show)
{
    if (show)
    {
        // fill_palette(leds, NUM_LEDS, paletteIndex, 255 / NUM_LEDS, myPal, 255, LINEARBLEND);
        // EVERY_N_MILLISECONDS(10)
        // {
        //     paletteIndex++;
        // }
        // fill_solid(leds, NUM_LEDS, CRGB::White);

        // FastLED.show();

        // // FastLED.setCorrection(UncorrectedColor);
        // FastLED.setTemperature
        // leds[0] = CRGB::Red;
        // FastLED.show();
        // delay(1000);

        // FastLED.setCorrection(TypicalLEDStrip);
        // leds[0] = CRGB::Green;
        // FastLED.show();
        // delay(1000);

        // FastLED.setCorrection(TypicalPixelString);
        // leds[0] = CRGB::Blue;
        // FastLED.show();
        // delay(1000);

        EVERY_N_MILLISECONDS(50)
        {

            // Create a new HSV color for led[0]
            leds[0] = CRGB::DarkOrange;
            // Copy each pixel to the next one, starting at the far end
            // thereby 'moving' the pattern along the strip
            for (int i = NUM_LEDS - 1; i > 0; i--)
            {
                leds[i] = leds[i - 1];
            }
        }
        FastLED.show();
    }
    else
    {
        FastLED.clear();
        FastLED.show();
    }
}