#include <MatrixKeypad.h>
#include <RTClib.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

//oled init
#define OLED_WIDTH 128
#define OLED_HEIGHT 32
#define OLED_RESET -1 //shares mc reset as oled doesn't have reset pin 
#define OLED_ADDRESS 0x3C

Adafruit_SSD1306 oled(OLED_WIDTH,OLED_HEIGHT,&Wire,OLED_RESET);

//keypad init
const uint8_t rows=4;
const uint8_t columns=4;
uint8_t rowPins[rows]={26,27,12,13};
uint8_t columnPins[columns]={23,32,33,25};
char keymap[rows][columns]={
  {'0','4','8','C'},
  {'1','5','9','D'},
  {'2','6','A','E'},
  {'3','7','B','F'}};

MatrixKeypad_t *keypad;
char key;

//rtc
RTC_DS3231 rtc;
DateTime dateTime;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void setup() {

  Serial.begin(9600);
  Serial.println("Initializing the app...");
 
  //keypad-setup
  keypad=MatrixKeypad_create((char*)keymap,rowPins,columnPins,rows,columns);//this variable will passed for every function..

  //oled-setup
  oled.begin(SSD1306_SWITCHCAPVCC,OLED_ADDRESS);
  oled.clearDisplay();

  //rtc-setup
  if(!rtc.begin()){
    Serial.println("RTC module not found..");
    while (1){delay(10);}
  }

 


  // if(rtc.lostPower()){
    if(1){
    Serial.println("RTC lost its power, so set the time again..");
    rtcSet();
    }
  
  oled.clearDisplay();
  oled.display();

}

void loop() {
  dateTime=rtc.now();
      oled.clearDisplay();
      oled.setTextSize(2);
      oled.setCursor(0, 0);
      oled.setTextColor(WHITE);
      oled.print(dateTime.hour());
      oled.print(":");
      oled.print(dateTime.minute());
      oled.print(":");
      oled.println(dateTime.second());
      oled.display();
      Serial.println(dateTime.timestamp());
      delay(500);
    
    Serial.println(rtc.lostPower());

}
