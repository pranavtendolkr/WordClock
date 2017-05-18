// colormodes:
// 0: all one solid color;
// 1: each word individual color
// 2: random colors for each letter
// 3: fast random transition


// initilizes all pixels to off state and zero value for color
void initilizePixelArray(struct PIXEL *pixels){
        for(int i=0; i<NUMPIXELS; i++) {
                pixels[i]={false,0};
        }
}

// Convert separate R,G,B into packed 32-bit RGB color.
uint32_t convertColor(uint8_t r, uint8_t g, uint8_t b) {
        return ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;
}

// applies brightness adjustment to the color
uint32_t setBrightness(uint32_t color, uint8_t brightness ) {
        uint8_t r =  (uint8_t)(color >> 16) * BRIGHTNESS/MAX_BRIGHTNESS;
        uint8_t g = (uint8_t)(color >>  8) * BRIGHTNESS/MAX_BRIGHTNESS;
        uint8_t b = (uint8_t)(color >>  0) * BRIGHTNESS/MAX_BRIGHTNESS;
        return ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;
}



// Warning, if else ladder. :X
// Toggles words based on time.
void setLEDTime(struct PIXEL *individualPixels,struct Time time){
        /* Parse time values to light corresponding pixels */
        individualPixels[28].state=true; //Light "IT"
        individualPixels[29].state=true; //Light "IS"

        // //Light "O CLOCK"
        // individualPixels[0].state=true;
        // individualPixels[1].state=true;

        /* Minutes between 0-5 - Light "O CLOCK" */
        if ((time.minute>=0 && time.minute<5)) {
                individualPixels[0].state=true;
                individualPixels[1].state=true;
        }

        /* Minutes between 5-10 or 55-60 - Light "FIVE," "MINUTES" */
        if ((time.minute>=5 && time.minute<10) || (time.minute>=55 && time.minute<60)) {
                individualPixels[21].state=true;
                individualPixels[22].state=true;
                individualPixels[23].state=true;
        }

        /* Minutes between 10-15 or 50-55 - Light "TEN," "MINUTES" */
        if ((time.minute>=10 && time.minute<15) || (time.minute>=50 && time.minute<55)) {
                individualPixels[30].state=true;
                individualPixels[22].state=true;
                individualPixels[23].state=true;
        }

        /* Minutes between 15-20 or 45-50 - Light "QUARTER" */
        if ((time.minute>=15 && time.minute<20)  || (time.minute>=45 && time.minute<50)) {
                individualPixels[27].state=true;
                individualPixels[26].state=true;
        }

        /* Minutes between 20-25 or 40-45 - Light "TWENTY," "MINUTES" */
        if ((time.minute>=20 && time.minute<25)  || (time.minute>=40 && time.minute<45)) {
                individualPixels[24].state=true;
                individualPixels[25].state=true;
                individualPixels[22].state=true;
                individualPixels[23].state=true;
        }

        /* Minutes between 25-30 or 35-40 - Light "TWENTY," "FIVE," "MINUTES" */
        if ((time.minute>=25 && time.minute<30) || (time.minute>=35 && time.minute<40)) {
                individualPixels[24].state=true;
                individualPixels[25].state=true;
                individualPixels[22].state=true;
                individualPixels[23].state=true;
                individualPixels[21].state=true;
        }

        /* Minutes between 30-35 - Light "HALF" */
        if ((time.minute>=30 && time.minute<35)) {
                individualPixels[31].state=true;
        }

        /* Minutes between 5-35 - Light "PAST" */
        if ((time.minute>=5) && (time.minute<35)) {
                individualPixels[20].state=true;
        }

        /* Minutes between 35-60 - Light "TO" & MODIFY CURRENT HOUR VALUE */
        if (time.minute>=35) {
                individualPixels[19].state=true;
                time.hour++; //Add 1 from current hour
                /*Set time to twelve for hour around midnight, noon */
                if (time.hour==0) {
                        time.hour=12;
                }
                /*Corner case for 12:35-12:59 */
                if (time.hour==13) {
                        time.hour=1;
                }
        }

        /* Hour=1 - Light "ONE" */
        if (time.hour==1) {
                individualPixels[18].state=true;
        }

        /* Hour=2 - Light "TWO" */
        if (time.hour==2) {
                individualPixels[17].state=true;
        }

        /* Hour=3 - Light "THREE" */
        if (time.hour==3) {
                individualPixels[13].state=true;
                individualPixels[14].state=true;
        }

        /* Hour=4 - Light "FOUR" */
        if (time.hour==4) {
                individualPixels[15].state=true;
        }

        /* Hour=5 - Light "FIVE" */
        if (time.hour==5) {
                individualPixels[16].state=true;
        }

        /* Hour=6 - Light "SIX" */
        if (time.hour==6) {
                individualPixels[12].state=true;
        }

        /* Hour=7 - Light "SEVEN" */
        if (time.hour==7) {
                individualPixels[10].state=true;
                individualPixels[11].state=true;
        }


        /* Hour=8 - Light "EIGHT" */
        if (time.hour==8) {
                individualPixels[9].state=true;
                individualPixels[8].state=true;
        }

        /* Hour=9 - Light "NINE" */
        if (time.hour==9) {
                individualPixels[4].state=true;
        }

        /* Hour=10 - Light "TEN" */
        if (time.hour==10) {
                individualPixels[5].state=true;
        }

        /* Hour=11 - Light "ELEVEN" */
        if (time.hour==11) {
                individualPixels[6].state=true;
                individualPixels[7].state=true;
        }

        /* Hour=12 - Light "TWELVE" */
        if (time.hour==12) {
                individualPixels[3].state=true;
                individualPixels[2].state=true;
        }
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
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



//switch construct to call appropriate functions based on colormode
void setLEDColor(struct PIXEL *individualPixels,int colormode){
        switch (colormode) {
		// set to white. no change in color
        case 0:
                setLEDSolidColor(individualPixels,SOLID_red,SOLID_green,SOLID_blue);
                break;
        case 1:
                setLEDWordRandomColor(individualPixels);
                break;
        case 2:
                setLEDRandomLetters(individualPixels);
                break;

        default:
				// set to white
                setLEDSolidColor(individualPixels,255,255,255);
                break;
        }
        //set brightness for the all pixels
        for(int i=0; i<NUMPIXELS; i++) {
                // yes, we set brightness by manipulating all 3 color values uniformly
                individualPixels[i].color=setBrightness(individualPixels[i].color, BRIGHTNESS);
        }
}


// sets leds to one solid color
void setLEDSolidColor(PIXEL *individualPixels, int r, int g, int b){
        for(int i=0; i<NUMPIXELS; i++) {
                individualPixels[i].color=convertColor(r,g,b);
        }
}

//sets each word random individual color
/*
WORDS AND THEIR CORROSPONDING LED NUMBERS:
IT	 			28
IS 				29
O'clock 		0,1
Minutes 		22,23
minutes-five 	21
minutes-ten     30
QUARTER	   		26,27
minutes-TWENTY 	24,25
HALF			31
PAST			20
TO				19
ONE				18
TWO				17
THREE			13,14
FOUR			15
FIVE			16
SIX				12
SEVEN			10,11
EIGHT			8,9
NINE 			4
TEN				5
ELEVEN			6,7
TWELVE			2,3
*/
void setLEDWordRandomColor(PIXEL *individualPixels){
	// total 23 different words on the face, one word can have a max of 2 LEDS. -1 means LED not present
	int words[23][2] = {
{28,-1},{29,-1},{0,1},{22,23},{21,-1},{30,-1},{26,27},{24,25},{31,-1},{20,-1},{19,-1},{18,-1},{17,-1},{13,14},{15,-1},{16,-1},{12,-1},{10,11},{8,9},{5,-1},{4,-1},{6,7},{2,3} }
    for int (i=0; i<23; i++){
	  int red = random(256);
	  int green = random(256);
	  int blue = random(256);
	  individualPixels[words[i][0]].color=convertColor(red,green,blue);
	  if(words[i][1] != -1)
		individualPixels[words[i][1]].color=convertColor(red,green,blue);
	}
}


// sets each led to a random color. May look HOrrible.
void setLEDRandomLetters(PIXEL *individualPixels){
	for(int i=0;i<NUMPIXELS; i++){
		individualPixels[i].color=convertColor(random(256),random(256),random(256))
	}

}
