// instruction format over bluetooth Serial
//time:hh:mm:ss     sets the time
//colormode:x(0-5)  sets color modes
//brightness:x(0-255)  sets brightness.
//color:R,G,B    sets solid color for colormode 0

//calls appropriate function
String splitAndGetValue(String data, char separator, int index);
void changeTime(String timestr);
void changeColorMode(String value);
void changeBrightness(String value);
void changecColor(String value);

// parses command sent over bluetooth
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

// splits the string based on separator and gets the index'th part
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
  BRIGHTNESS=value.toInt();
}

void changeColorMode(String value){
  COLORMODE = value.toInt();
}

// sets the solid color values.
void changeColor(String value){
  // if colormode is set to solid color, correct function will be called next cycle in loop()
  // function will use these global values for RGB
  SOLID_red = splitAndGetValue(value, ',', 0).toInt();
  SOLID_blue = splitAndGetValue(value, ',', 1).toInt();
  SOLID_green = splitAndGetValue(value, ',', 2).toInt();
}
