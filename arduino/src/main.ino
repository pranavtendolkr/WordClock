
 #include "Wire.h"

#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>

// PIN connected to the led strip
#define LEDPIN           6

// Number of neopixel LEDs attached
#define NUMPIXELS     32

// max value for brightness
#define MAX_BRIGHTNESS 100
// brightness [0-100]
int BRIGHTNESS = 100;

// different color modes for led to cycle through
int COLORMODE=0;

// rgb variables for setting solid colors
int SOLID_red = 255;
int SOLID_green = 255;
int SOLID_blue=255;



Adafruit_NeoPixel LEDStrip = Adafruit_NeoPixel(NUMPIXELS, LEDPIN, NEO_GRB + NEO_KHZ800);

SoftwareSerial BTSerial(10, 11); // RX | TX


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



// things to be done on power up
void setup(){
  Wire.begin();
  Serial.begin(9600);
  LEDStrip.begin(); //Begin Neopixel string
  //9600,19200,38400,57600,115200,230400,460800
  BTSerial.begin(9600);  // HC-05 default speed in AT command more
}


// loop this every 1 second
void loop(){

  initilizePixelArray(pixels);

  struct Time currentTime = getRTCTime();;

  setLEDTime(pixels,currentTime);
  setLEDColor(pixels,COLORMODE);

  // for(int i=0;i<NUMPIXELS;i++){
  //   Serial.println("Pixel "+String(i)+"is " + String(pixels[i].state) +" and color is "+ String(pixels[i].color));
  // }

  // read anything over bluetooth serial port and send to bluetooth functions
   if (BTSerial.available()) {
     String s = "";
     char c;
     while((c = BTSerial.read()) != -1) {
       s += c;
       delay(5);
     }
     Serial.println("Received bluetooth cmd: " + s);
     bluetoothcmd(s);
   }

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
