 #include "Wire.h"

#include <Adafruit_NeoPixel.h>

// PIN connected to the led strip
#define LEDPIN           6

// Number of neopixel LEDs attached
#define NUMPIXELS     32

Adafruit_NeoPixel LEDStrip = Adafruit_NeoPixel(NUMPIXELS, LEDPIN, NEO_GRB + NEO_KHZ800);


struct Time {
    int hour;
    int minute;
    int second;
};

struct PIXEL {
  bool state;
  uint32_t color;
};

 PIXEL *pixels = new  PIXEL[ NUMPIXELS ];

// different color modes for led to cycle through
int colormode=0;

// things to be done on power up
void setup(){
  Wire.begin();
  Serial.begin(9600);
  LEDStrip.begin(); //Begin Neopixel string
}





// loop this every 1 second
void loop(){

  initilizePixelArray(pixels);

  struct Time currentTime = getTime();;

  //currentTime = getTime();

  setLEDTime(pixels,currentTime);
  setLEDColor(pixels,colormode);

  // for(int i=0;i<NUMPIXELS;i++){
  //   Serial.println("Pixel "+String(i)+"is " + String(pixels[i].state) +" and color is "+ String(pixels[i].color));
  // }


  Serial.println("Hour "+ String(currentTime.hour));
 Serial.println("Minute " + String(currentTime.minute));
 Serial.println("Second " + String(currentTime.second));

  for(int i=0;i<NUMPIXELS;i++){
    if(pixels[i].state){
      LEDStrip.setPixelColor(i,pixels[i].color);
    }
    else{
      LEDStrip.setPixelColor(i,LEDStrip.Color(0,0,0));
    }

  }
  LEDStrip.show(); //Display Neopixel color
  delay(1000);
}
