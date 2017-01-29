# 1 "/tmp/tmpfhYkPi"
#include <Arduino.h>
# 1 "/home/pranav/Arduino/wordclock/arduino/src/main.ino"

 #include "Wire.h"

#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>


#define LEDPIN 6


#define NUMPIXELS 32


#define MAX_BRIGHTNESS 100

int BRIGHTNESS = 100;


int COLORMODE=0;


int SOLID_red = 255;
int SOLID_green = 255;
int SOLID_blue=255;


int DELAY = 1000;



Adafruit_NeoPixel LEDStrip = Adafruit_NeoPixel(NUMPIXELS, LEDPIN, NEO_GRB + NEO_KHZ800);

SoftwareSerial BTSerial(10, 11);


struct Time {
        int hour;
        int minute;
        int second;
};

struct PIXEL {
        bool state;
        uint32_t color;
};

PIXEL *pixels = new PIXEL[ NUMPIXELS ];
void setup();
void loop();
byte decToBcd(byte val);
byte bcdToDec(byte val);
struct Time getRTCTime();
void setRTCTime(struct Time time);
void bluetoothcmd(String command);
String splitAndGetValue(String data, char separator, int index);
void changeTime(String timestr);
void changeBrightness(String value);
void changeColorMode(String value);
void changeColor(String value);
void initilizePixelArray(struct PIXEL *pixels);
uint32_t convertColor(uint8_t r, uint8_t g, uint8_t b);
uint32_t setBrightness(uint32_t color, uint8_t brightness );
void setLEDTime(struct PIXEL *individualPixels,struct Time time);
uint32_t Wheel(byte WheelPos);
void setLEDColor(struct PIXEL *individualPixels,int colormode);
void setLEDSolidCOlor(PIXEL *individualPixels);
#line 52 "/home/pranav/Arduino/wordclock/arduino/src/main.ino"
void setup(){
        Wire.begin();
        Serial.begin(9600);
        LEDStrip.begin();

        BTSerial.begin(9600);
}



void loop(){

        initilizePixelArray(pixels);

        struct Time currentTime = getRTCTime();;
        setLEDTime(pixels,currentTime);
        setLEDColor(pixels,COLORMODE);


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


        for(int i=0; i<NUMPIXELS; i++) {
                if(pixels[i].state) {
                        LEDStrip.setPixelColor(i,pixels[i].color);
                }
                else{
                        LEDStrip.setPixelColor(i,LEDStrip.Color(0,0,0));
                }
        }
        LEDStrip.show();
        delay(DELAY);
}
# 1 "/home/pranav/Arduino/wordclock/arduino/src/RTC.ino"
#include "Wire.h"
#define DS1307_ADDRESS 0x68
byte zero = 0x00;


byte decToBcd(byte val){

        return ( (val/10*16) + (val%10) );
}

byte bcdToDec(byte val) {

        return ( (val/16*10) + (val%16) );
}

struct Time getRTCTime(){
        struct Time currentTime = {0,0,0};

        Wire.beginTransmission(DS1307_ADDRESS);
        Wire.write(zero);
        Wire.endTransmission();

        Wire.requestFrom(DS1307_ADDRESS, 7);

        int second = bcdToDec(Wire.read());
        int minute = bcdToDec(Wire.read());
        int hour = bcdToDec(Wire.read() & 0b111111);
        if(hour>12)
                hour = hour-12;
        int weekDay = bcdToDec(Wire.read());
        int monthDay = bcdToDec(Wire.read());
        int month = bcdToDec(Wire.read());
        int year = bcdToDec(Wire.read());

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


void setRTCTime(struct Time time){

        byte second = time.second;
        byte minute = time.minute;
        byte hour = time.hour;

        byte weekDay = 7;
        byte monthDay = 14;
        byte month = 1;
        byte year = 16;
        Wire.beginTransmission(DS1307_ADDRESS);
        Wire.write(zero);
        Wire.write(decToBcd(second));
        Wire.write(decToBcd(minute));
        Wire.write(decToBcd(hour));
        Wire.write(decToBcd(weekDay));
        Wire.write(decToBcd(monthDay));
        Wire.write(decToBcd(month));
        Wire.write(decToBcd(year));
        Wire.write(zero);
        Wire.endTransmission();
}
# 1 "/home/pranav/Arduino/wordclock/arduino/src/bluetooth.ino"







String splitAndGetValue(String data, char separator, int index);
void changeTime(String timestr);
void changeColorMode(String value);
void changeBrightness(String value);
void changecColor(String value);




void bluetoothcmd(String command){
        if (command.startsWith("time:"))
                changeTime(command.substring(5));
        if (command.startsWith("brightness:"))
                changeBrightness(command.substring(11));
        if (command.startsWith("colormode:"))
                changeColorMode(command.substring(10));
        if (command.startsWith("color:"))
                changeColor(command.substring(6));
}


String splitAndGetValue(String data, char separator, int index)
{
        int found = 0;
        int strIndex[] = { 0, -1 };
        int maxIndex = data.length() - 1;
        for (int i = 0; i <= maxIndex && found <= index; i++) {
                if (data.charAt(i) == separator || i == maxIndex) {
                        found++;
                        strIndex[0] = strIndex[1] + 1;
                        strIndex[1] = (i == maxIndex) ? i+1 : i;
                }
        }
        return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void changeTime(String timestr){
        struct Time time;
        time.hour = splitAndGetValue(timestr, ':', 0).toInt();
        time.minute = splitAndGetValue(timestr, ':', 1).toInt();
        time.second = splitAndGetValue(timestr, ':', 2).toInt();
        Serial.println("Updating RTC time to:"+String(time.hour)+":"+String(time.minute)+":"+String(time.second));
        setRTCTime(time);
}

void changeBrightness(String value){
        Serial.println("Updating brightness to: " + value);
        BRIGHTNESS=value.toInt();
}

void changeColorMode(String value){
        Serial.println("Setting color mode to: " + value);
        COLORMODE = value.toInt();
}


void changeColor(String value){


        Serial.println("Setting solid color to: " + value);
        SOLID_red = splitAndGetValue(value, ',', 0).toInt();
        SOLID_blue = splitAndGetValue(value, ',', 1).toInt();
        SOLID_green = splitAndGetValue(value, ',', 2).toInt();
}
# 1 "/home/pranav/Arduino/wordclock/arduino/src/led.ino"
# 9 "/home/pranav/Arduino/wordclock/arduino/src/led.ino"
void initilizePixelArray(struct PIXEL *pixels){
        for(int i=0; i<NUMPIXELS; i++) {
                pixels[i]={false,0};
        }
}


uint32_t convertColor(uint8_t r, uint8_t g, uint8_t b) {
        return ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
}


uint32_t setBrightness(uint32_t color, uint8_t brightness ) {
        uint8_t r = (uint8_t)(color >> 16) * BRIGHTNESS/MAX_BRIGHTNESS;
        uint8_t g = (uint8_t)(color >> 8) * BRIGHTNESS/MAX_BRIGHTNESS;
        uint8_t b = (uint8_t)(color >> 0) * BRIGHTNESS/MAX_BRIGHTNESS;
        return ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
}





void setLEDTime(struct PIXEL *individualPixels,struct Time time){

        individualPixels[28].state=true;
        individualPixels[29].state=true;






        if ((time.minute>=0 && time.minute<5)) {
                individualPixels[0].state=true;
                individualPixels[1].state=true;
        }


        if ((time.minute>=5 && time.minute<10) || (time.minute>=55 && time.minute<60)) {
                individualPixels[21].state=true;
                individualPixels[22].state=true;
                individualPixels[23].state=true;
        }


        if ((time.minute>=10 && time.minute<15) || (time.minute>=50 && time.minute<55)) {
                individualPixels[30].state=true;
                individualPixels[22].state=true;
                individualPixels[23].state=true;
        }


        if ((time.minute>=15 && time.minute<20) || (time.minute>=45 && time.minute<50)) {
                individualPixels[27].state=true;
                individualPixels[26].state=true;
        }


        if ((time.minute>=20 && time.minute<25) || (time.minute>=40 && time.minute<45)) {
                individualPixels[24].state=true;
                individualPixels[25].state=true;
                individualPixels[22].state=true;
                individualPixels[23].state=true;
        }


        if ((time.minute>=25 && time.minute<30) || (time.minute>=35 && time.minute<40)) {
                individualPixels[24].state=true;
                individualPixels[25].state=true;
                individualPixels[22].state=true;
                individualPixels[23].state=true;
                individualPixels[21].state=true;
        }


        if ((time.minute>=30 && time.minute<35)) {
                individualPixels[31].state=true;
        }


        if ((time.minute>=5) && (time.minute<35)) {
                individualPixels[20].state=true;
        }


        if (time.minute>=35) {
                individualPixels[19].state=true;
                time.hour++;

                if (time.hour==0) {
                        time.hour=12;
                }

                if (time.hour==13) {
                        time.hour=1;
                }
        }


        if (time.hour==1) {
                individualPixels[18].state=true;
        }


        if (time.hour==2) {
                individualPixels[17].state=true;
        }


        if (time.hour==3) {
                individualPixels[13].state=true;
                individualPixels[14].state=true;
        }


        if (time.hour==4) {
                individualPixels[15].state=true;
        }


        if (time.hour==5) {
                individualPixels[16].state=true;
        }


        if (time.hour==6) {
                individualPixels[12].state=true;
        }


        if (time.hour==7) {
                individualPixels[10].state=true;
                individualPixels[11].state=true;
        }



        if (time.hour==8) {
                individualPixels[9].state=true;
                individualPixels[8].state=true;
        }


        if (time.hour==9) {
                individualPixels[4].state=true;
        }


        if (time.hour==10) {
                individualPixels[5].state=true;
        }


        if (time.hour==11) {
                individualPixels[6].state=true;
                individualPixels[7].state=true;
        }


        if (time.hour==12) {
                individualPixels[3].state=true;
                individualPixels[2].state=true;
        }
}




uint32_t Wheel(byte WheelPos) {
        WheelPos = 255 - WheelPos;
        if(WheelPos < 85) {
                return LEDStrip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
        }
        if(WheelPos < 170) {
                WheelPos -= 85;
                return LEDStrip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
        }
        WheelPos -= 170;
        return LEDStrip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}




void setLEDColor(struct PIXEL *individualPixels,int colormode){
        switch (colormode) {
        case 0:
                setLEDSolidCOlor(individualPixels);
                break;
        default:
                setLEDSolidCOlor(individualPixels);
                break;
        }

        for(int i=0; i<NUMPIXELS; i++) {

                individualPixels[i].color=setBrightness(individualPixels[i].color, BRIGHTNESS);
        }
}




void setLEDSolidCOlor(PIXEL *individualPixels){
        for(int i=0; i<NUMPIXELS; i++) {
                individualPixels[i].color=convertColor(SOLID_red,SOLID_green, SOLID_blue);
        }
}