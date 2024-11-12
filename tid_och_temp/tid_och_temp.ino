/*
* Name: clock and temp project
* Author: Figge Hellström
* Date: 2024-10-10
* Description: This project uses a ds3231 to measure time and displays the time to an 1306 oled display,
* Further, it measures temprature with a analog temprature module and displays a mapped value to a 9g-servo-motor
*/

// Include Libraries

// Init constants

// Init global variables

// construct objects


#include "U8glib.h"
#include <RTClib.h>
#include <Wire.h>
#include "Servo.h"



U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);



Servo myservo;

#define servoPin 9
const int blinkblink = 7;
const int wait = 200;
const int waitey = 1000;

DateTime now;

RTC_DS3231 rtc;
char t[32];



void setup() {
// init communication

// Init Hardware
  
  for(int i= 4; i <= 6; i++){
    pinMode(i, OUTPUT);
  }
  pinMode(A1, INPUT);
  pinMode(7, OUTPUT);
  Serial.begin(9600);
  Serial.begin(9600);
  Wire.begin();
  rtc.begin();
  rtc.adjust(DateTime(F(__DATE__),F(__TIME__)));
  //rtc.adjust(DateTime(2019, 1, 21, 5, 0, 0));

  myservo.attach(servoPin);
  myservo.write(0);

}

void loop() {
  now = rtc.now();
  
  oledWrite(getTime(), getTemp()); // write time and temp on screen
  delay(1000);
  servoWrite(getTemp()); // Make servo go to the same degrees as the temp
  blinkblonk(); // makes lamp blink when temp > 28

}

/*
* This function reads an analog pin connected to an analog temprature sensor and calculates the corresponding temp
*Parameters: Void
*Returns: temprature as float
*/
float getTemp() {
  int Vo;
  float R1 = 10000;
  float logR2;
  float R2;
  float T;
  float c1 = 0.001129148, c2 = 0.000234125, c3 = 0.0000000876741;
  
  
  Vo = analogRead(A1);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2)); // temperature in Kelvin
  T = T - 273.15;

  return T;
}

/*
* This function takes a string and draws it to an oled display
*Parameters: - text: String to write to display
*Returns: void
*/
void oledWrite(String time, float temp){
  String temp_str = String(temp,2);
  u8g.firstPage();
  do {
    u8g.setFont(u8g_font_unifont);
    u8g.drawStr(10, 20, time.c_str());
    u8g.drawStr(10, 40, "Temp: ");
    u8g.drawStr(50, 40, temp_str.c_str());

  } while (u8g.nextPage());
}

/*
*This function reads time from an ds3231 module and package the time as a String
*Parameters: Void
*Returns: time in hh:mm:ss as String
*/
String getTime(){ 
  DateTime now = rtc.now();  
  return ("Time: " + String(now.hour()) + ":" + String(now.minute()) +  ":" + String(now.second()));
}

/*
* takes a temprature value and maps it to corresppnding degree on a servo
*Parameters: - value: temprature
*Returns: void
*/
void servoWrite(float value){
  int temp = map(getTemp(), 18, 26, 0,10);
  myservo.write(temp);
}

// lamp blinks if temp > 28
void blinkblonk() {
  if (getTemp() > 28) {
    digitalWrite(blinkblink, HIGH);
    delay(waitey);
    digitalWrite(blinkblink, LOW);
    delay(wait);
  }
}







