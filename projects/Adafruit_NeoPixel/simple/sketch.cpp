// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            BUILD_OPT_PIN        // Value supplied by the build script!

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      BUILD_OPT_NUM_PIXELS // Value supplied by the build script!

// Type NeoPixel devices
#define NEO_TYPE       BUILD_OPT_NEO_TYPE   // Value supplied by the build script!

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_TYPE + NEO_KHZ800);

int delayval = 200; // delay for half a second

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code

  pixels.begin(); // This initializes the NeoPixel library.
  pixels.show();
    

}

void cycle(uint8_t r, uint8_t g, uint8_t b, uint8_t w )
{
  // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.
  for(int i=0;i<NUMPIXELS;i++){

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(r,g,b,w)); // Moderately bright green color.

    pixels.show(); // This sends the updated pixel color to the hardware.

    delay(delayval); // Delay for a period of time (in milliseconds).

  }
}
void loop() {

    cycle(64,0,0,0);       // Something about RED messes up my NeoPixel 16 Ring
    cycle(0,0,0,0);
    cycle(0,64,0,0);
    cycle(0,0,0,0);
    cycle(0,0,64,0);
    cycle(0,0,0,0);
    cycle(0,0,0,64);
    cycle(0,0,0,0);
  
}
