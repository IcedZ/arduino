//LCDTestExample by Nodcah 
//A simple sketch to make sure your Serial LCD Kit from Sparkfun
//is working

#include "LiquidCrystal.h"

LiquidCrystal lcd(2,3,4,5,6,7,8);

void setup() {
  pinMode(9, OUTPUT); //the backlight
  pinMode(13, OUTPUT); //the buzzer
  
  lcd.begin(16, 2); //16 chars wide, 2 tall
  
  digitalWrite(9, HIGH); //set the backlight to HIGH
  
  lcd.print("  Hello world!  "); //use spaces to center the text
  delay(2000);
}

void loop() { 
  //buzzer turns on and off and its status is displayed on the LCD
  lcd.clear();
  lcd.print("  Buzzer is on  ");
  tone(13, 262, 1000);
  delay(1000);
  lcd.clear();
  lcd.print(" Buzzer is off  ");
  delay(1000);
  lcd.clear();
  lcd.print("There is no buzzer");
  delay(1000);
  }
