#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

const int PIN = 26;
const int NUM_PXL = 3;
const int DELAY = 500;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_PXL, PIN, NEO_GRB + NEO_KHZ800);

// Some functions of our own for creating animated effects -----------------

// Fill pixels pixels one after another with a color. pixels is NOT cleared
// first; anything there will be covered pixel by pixel. Pass in color
// (as a single 'packed' 32-bit value, which you can get by calling
// pixels.Color(red, green, blue) as shown in the loop() function above),
// and a delay time (in milliseconds) between pixels.
void colorWipe(uint32_t color, int wait)
{
  for (int i = 0; i < pixels.numPixels(); i++)
  {                                 // For each pixel in pixels...
    pixels.setPixelColor(i, color); //  Set pixel's color (in RAM)
    pixels.show();                  //  Update pixels to match
    delay(wait);                    //  Pause for a moment
  }
}

// Theater-marquee-style chasing lights. Pass in a color (32-bit value,
// a la pixels.Color(r,g,b) as mentioned above), and a delay time (in ms)
// between frames
void theaterChase(uint32_t color, int wait)
{
  for (int a = 0; a < 10; a++)
  { // Repeat 10 times...
    for (int b = 0; b < 3; b++)
    {                 //  'b' counts from 0 to 2...
      pixels.clear(); //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of pixels in steps of 3...
      for (int c = b; c < pixels.numPixels(); c += 3)
      {
        pixels.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      pixels.show(); // Update pixels with new contents
      delay(wait);   // Pause for a moment
    }
  }
}

// Rainbow cycle along whole pixels. Pass delay time (in ms) between frames.
void rainbow(int wait)
{
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this outer loop:
  for (long firstPixelHue = 0; firstPixelHue < 5 * 65536; firstPixelHue += 256)
  {
    for (int i = 0; i < pixels.numPixels(); i++)
    { // For each pixel in pixels...
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the pixels
      // (pixels.numPixels() steps):
      int pixelHue = firstPixelHue + (i * 65536L / pixels.numPixels());
      // pixels.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the single-argument hue variant. The result
      // is passed through pixels.gamma32() to provide 'truer' colors
      // before assigning to each pixel:
      pixels.setPixelColor(i, pixels.gamma32(pixels.ColorHSV(pixelHue)));
    }
    pixels.show(); // Update pixels with new contents
    delay(wait);   // Pause for a moment
  }
}

void mellow(uint32_t colour)
{
  for (int i = 0; i < pixels.numPixels(); i++)
  {
  }
}

// Rainbow-enhanced theater marquee. Pass delay time (in ms) between frames.
void theaterChaseRainbow(int wait)
{
  int firstPixelHue = 0; // First pixel starts at red (hue 0)
  for (int a = 0; a < 30; a++)
  { // Repeat 30 times...
    for (int b = 0; b < 3; b++)
    {                 //  'b' counts from 0 to 2...
      pixels.clear(); //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of pixels in increments of 3...
      for (int c = b; c < pixels.numPixels(); c += 3)
      {
        // hue of pixel 'c' is offset by an amount to make one full
        // revolution of the color wheel (range 65536) along the length
        // of the pixels (pixels.numPixels() steps):
        int hue = firstPixelHue + c * 65536L / pixels.numPixels();
        uint32_t color = pixels.gamma32(pixels.ColorHSV(hue)); // hue -> RGB
        pixels.setPixelColor(c, color);                        // Set pixel 'c' to value 'color'
      }
      pixels.show();               // Update pixels with new contents
      delay(wait);                 // Pause for a moment
      firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
    }
  }
}

void setup()
{
  pixels.begin();
  pixels.setBrightness(50);
}

void loop()
{
  // Fill along the length of the pixels in various colors...
  // colorWipe(pixels.Color(255, 0, 0), 50); // Red
  // colorWipe(pixels.Color(0, 255, 0), 50); // Green
  // colorWipe(pixels.Color(0, 0, 255), 50); // Blue

  // Do a theater marquee effect in various colors...
  // theaterChase(pixels.Color(127, 127, 127), 50); // White, half brightness
  // theaterChase(pixels.Color(127, 0, 0), 50);     // Red, half brightness
  // theaterChase(pixels.Color(0, 0, 127), 50);     // Blue, half brightness

  rainbow(10); // Flowing rainbow cycle along the whole pixels
  // theaterChaseRainbow(50); // Rainbow-enhanced theaterChase variant
}
