// #include "Wire.h"
// #define DS1307_ADDRESS 0x68
// byte zero = 0x00; //workaround for issue #527
//
// struct Time {
//     int hour;
//     int minute;
//     int second;
// };
//
//
// byte decToBcd(byte val){
// // Convert normal decimal numbers to binary coded decimal
//   return ( (val/10*16) + (val%10) );
// }
//
// byte bcdToDec(byte val)  {
// // Convert binary coded decimal to normal decimal numbers
//   return ( (val/16*10) + (val%16) );
// }
//
// struct Time getTime(){
//   struct Time currentTime = {0,0,0};
//   // Reset the register pointer
//   Wire.beginTransmission(DS1307_ADDRESS);
//   Wire.write(zero);
//   Wire.endTransmission();
//
//   Wire.requestFrom(DS1307_ADDRESS, 7);
//
//   int second = bcdToDec(Wire.read());
//   int minute = bcdToDec(Wire.read());
//   int hour = bcdToDec(Wire.read() & 0b111111); //24 hour time
//   int weekDay = bcdToDec(Wire.read()); //0-6 -> sunday - Saturday
//   int monthDay = bcdToDec(Wire.read());
//   int month = bcdToDec(Wire.read());
//   int year = bcdToDec(Wire.read());
//   //print the date EG   3/1/11 23:59:59
//   Serial.print(month);
//   Serial.print("/");
//   Serial.print(monthDay);
//   Serial.print("/");
//   Serial.print(year);
//   Serial.print("     ");
//   Serial.print(hour);
//   Serial.print(":");
//   Serial.print(minute);
//   Serial.print(":");
//   Serial.println(second);
//   currentTime.hour=hour;
//   currentTime.minute=minute;
//   currentTime.second=second;
//   return currentTime;
// }
