#include <Arduino.h>
#include <tinyNeoPixel.h>

#define LED_PIN PIN_PA5
#define LED_COUNT 24

tinyNeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

uint16_t pixelNumber = LED_COUNT; // Total Number of Pixels

void setup()
{
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)
}

// loop() function -- runs repeatedly as long as board is on ---------------
void loop()
{
  true;
}
