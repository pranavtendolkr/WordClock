 #include "Wire.h"

#include <Adafruit_NeoPixel.h>

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN           6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS     32

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#define DS1307_ADDRESS 0x68
byte zero = 0x00; //workaround for issue #527

struct Time {
    int hour;
    int minute;
    int second;
};

struct PIXEL {
  bool state;
  uint32_t color;
};

//
int colormode=0 // all white
//colormode=1 each word random color
//colormode=2 all one color, slowly cycle
//colormode=3 values equal to R,G,B


// RGB color variables
int red=255;
int green=255;
int blue=255;
int color_position=0;


void setup(){
  Wire.begin();
  Serial.begin(9600);
  pixels.begin(); //Begin Neopixel string

  // setDateTime(); //MUST CONFIGURE IN FUNCTION

Serial.println("loopstart");
}




// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}



byte decToBcd(byte val){
// Convert normal decimal numbers to binary coded decimal
  return ( (val/10*16) + (val%10) );
}

byte bcdToDec(byte val)  {
// Convert binary coded decimal to normal decimal numbers
  return ( (val/16*10) + (val%16) );
}

struct Time getTime(){
  struct Time currentTime = {0,0,0};
  // Reset the register pointer
  Wire.beginTransmission(DS1307_ADDRESS);
  Wire.write(zero);
  Wire.endTransmission();

  Wire.requestFrom(DS1307_ADDRESS, 7);

  int second = bcdToDec(Wire.read());
  int minute = bcdToDec(Wire.read());
  //int hour = bcdToDec(Wire.read() & 0b111111); //24 hour time
  int hour = bcdToDec(Wire.read() & 0b111111); //24 hour time
  if(hour>12)
    hour = hour-12;
  int weekDay = bcdToDec(Wire.read()); //0-6 -> sunday - Saturday
  int monthDay = bcdToDec(Wire.read());
  int month = bcdToDec(Wire.read());
  int year = bcdToDec(Wire.read());
  //print the date EG   3/1/11 23:59:59
  Serial.print(month);
  Serial.print("/");
  Serial.print(monthDay);
  Serial.print("/");
  Serial.print(year);
  Serial.print("     ");
  Serial.print(hour);
  Serial.print(":");
  Serial.print(minute);
  Serial.print(":");
  Serial.println(second);
  currentTime.hour=hour;
  currentTime.minute=minute;
  currentTime.second=second;
  return currentTime;
}



void loop(){
  int individualPixels[NUMPIXELS]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  struct Time currentTime = {0, 0, 0 };

  currentTime = getTime();
  Serial.println("Hour "+ String(currentTime.hour));
 Serial.println("Minute " + String(currentTime.minute));
 Serial.println("Second " + String(currentTime.second));
 int h = currentTime.hour;
 int m = currentTime.minute;



 /* Parse time values to light corresponding pixels */
  individualPixels[28]=1; //Light "IT"
  individualPixels[29]=1; //Light "IS"

  /* Minutes between 0-5 - Light "O CLOCK" */
  if ((m>=0 && m<5)){
    individualPixels[0]=1;
    individualPixels[1]=1;
  }

  /* Minutes between 5-10 or 55-60 - Light "FIVE," "MINUTES" */
  if ((m>=5 && m<10) || (m>=55 && m<60)){
    individualPixels[21]=1;
    individualPixels[22]=1;
    individualPixels[23]=1;
  }

  /* Minutes between 10-15 or 50-55 - Light "TEN," "MINUTES" */
  if ((m>=10 && m<15) || (m>=50 && m<55)){
    individualPixels[30]=1;
    individualPixels[22]=1;
    individualPixels[23]=1;
  }

  /* Minutes between 15-20 or 45-50 - Light "QUARTER" */
  if ((m>=15 && m<20) || (m>=45 && m<50)){
    individualPixels[27]=1;
    individualPixels[26]=1;
  }

  /* Minutes between 20-25 or 40-45 - Light "TWENTY," "MINUTES" */
  if ((m>=20 && m<25) || (m>=40 && m<45)){
    individualPixels[24]=1;
    individualPixels[25]=1;
    individualPixels[26]=1;
    individualPixels[27]=1;
  }

  /* Minutes between 25-30 or 35-40 - Light "TWENTY," "FIVE," "MINUTES" */
  if ((m>=25 && m<30) || (m>=35 && m<40)){
    individualPixels[24]=1;
    individualPixels[25]=1;
    individualPixels[22]=1;
    individualPixels[23]=1;
    individualPixels[21]=1;
  }

  /* Minutes between 30-35 - Light "HALF" */
  if ((m>=30 && m<35)){
    individualPixels[31]=1;
  }

  /* Minutes between 5-35 - Light "PAST" */
  if ((m>=5) && (m<35)){
    individualPixels[20]=1;
  }

  /* Minutes between 35-60 - Light "TO" & MODIFY CURRENT HOUR VALUE */
  if (m>=35){
    individualPixels[19]=1;
    h++; //Add 1 from current hour
    /*Set time to twelve for hour around midnight, noon */
    if (h==0){
      h=12;
    }
    /*Corner case for 12:35-12:59 */
    if (h==13){
      h=1;
    }
  }

  /* Hour=1 - Light "ONE" */
  if (h==1){
    individualPixels[18]=1;
  }

  /* Hour=2 - Light "TWO" */
  if (h==2){
    individualPixels[17]=1;
  }

  /* Hour=3 - Light "THREE" */
  if (h==3){
    individualPixels[13]=1;
    individualPixels[14]=1;
  }

  /* Hour=4 - Light "FOUR" */
  if (h==4){
    individualPixels[15]=1;
  }

  /* Hour=5 - Light "FIVE" */
  if (h==5){
    individualPixels[16]=1;
  }

  /* Hour=6 - Light "SIX" */
  if (h==6){
    individualPixels[12]=1;
  }

  /* Hour=7 - Light "SEVEN" */
  if (h==7){
    individualPixels[10]=1;
    individualPixels[11]=1;
  }


  /* Hour=8 - Light "EIGHT" */
  if (h==8){
    individualPixels[9]=1;
    individualPixels[8]=1;
  }

  /* Hour=9 - Light "NINE" */
  if (h==9){
    individualPixels[4]=1;
  }

  /* Hour=10 - Light "TEN" */
  if (h==10){
    individualPixels[5]=1;
  }

  /* Hour=11 - Light "ELEVEN" */
  if (h==11){
    individualPixels[6]=1;
    individualPixels[7]=1;
  }

  /* Hour=12 - Light "TWELVE" */
  if (h==12){
    individualPixels[3]=1;
    individualPixels[2]=1;
  }


 if(color_position++ > 255)
    color_position=0;
  /* Light pixels corresponding to current time */
  for (int i=0; i<sizeof(individualPixels); i++){
    if (individualPixels[i]==1){
      //pixels.setPixelColor(i, pixels.Color(red,green,blue)); //Set Neopixel color
      pixels.setPixelColor(i, Wheel(color_position)); //Set Neopixel color

    }
    else{
      pixels.setPixelColor(i,pixels.Color(0,0,0));
    }
  }
  pixels.show(); //Display Neopixel color
  delay(1000);
}
