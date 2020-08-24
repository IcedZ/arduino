
/*FPSGarageDoorOpenner by Nodcah
 *
 *FPS_GT511C3 library created by Josh Hawley, July 23rd 2013
 *Licensed for non-commercial use, must include this license message
 *basically, Feel free to hack away at it, but just give me credit for my work =)
 *TLDR; Wil Wheaton's Law
 *
 *Opens a garage door if the scanned fingerprint is in
 *the FPS' database of prints.  
 */
#include "LiquidCrystal.h" //for the screen
#include "FPS_GT511C3.h" //the fps (fingerprint scanner) library
#include "SoftwareSerial.h" //used by fps library

//Setting up the pins for the LCD and the fps
LiquidCrystal lcd(2, 3, 4, 5, 6, 7, 8); //pinouts for LCD
FPS_GT511C3 fps(10, 11); //RX, TX

boolean isFinger = false; //true if the fps detects a finger on the scanner
int timer = 0; //this is for when there is too long of a delay, it turns off

//output pins
const int buzzerPin = 13;
const int backlightPin = 9;
const int attinyPin = 12;

void setup(){
  //set outputs
  pinMode(buzzerPin, OUTPUT);
  pinMode(backlightPin, OUTPUT);
  pinMode(attinyPin, OUTPUT);

  //for debugging
  //Serial.begin(9600);
  fps.UseSerialDebug = false; //set to true for fps debugging through serial

  //initializing the libraries
  lcd.begin(16,2);
  digitalWrite(backlightPin, HIGH); //the LCD backlight
  fps.Open();
  fps.SetLED(true); //the fps LED
  //boot up sound
  for(int i=0; i<30; i++){
    tone(buzzerPin, 50+10*i, 30);
    delay(30);
  }
  tone(buzzerPin, 350);

  //print starting message
  lcd.print("Put your finger "); //the command to print to the LCD
  lcd.setCursor(0, 1); //sets the cursor to the 0th column in the 1st row
  lcd.print(" on the scanner ");
  delay(150);
  noTone(buzzerPin); //stops the startup sound

}
void loop(){
  //scan and identify the finger when one is put on it
  waitForFinger();
  timer = 0; //resets timer for timeout
  lcd.clear(); //clears the screen and sets the cursor to 0,0
  fps.CaptureFinger(false); //captures the finger for identification
  int id = fps.Identify1_N(); //identifies print and stores the id
  if(id < 20){
    lcd.print(" Access granted "); //success message
    lcd.setCursor(0,1);

    //one line personalized messages
    //messages can't be more than 16 chars!!!
    switch(id){
    case 0:
      lcd.print("    Hi self!    ");
      break;
    case 1:
      lcd.print("    Hey Bro!    ");
      break;
    case 2:
      lcd.print("Wat up homeslice");
      break; 
    case 3:
      lcd.print("    Hi Mom!     ");
      break;
    case 4:
      lcd.print("    Hey Dad!    ");
      break;
    case 5:
      lcd.print("   Hi Auntie!   ");
      break;
    case 6:
      lcd.print("  Hi Grandma!   ");
      break;
    case 7:
      lcd.print("   Hi Zeide!    ");
      break;
    }
    tone(buzzerPin, 262, 1000);
    delay(1500);

    //sends simple pattern to attiny inside garage  (for security)
    digitalWrite(attinyPin, HIGH);
    delay(100);
    digitalWrite(attinyPin, LOW);
    delay(50);
    digitalWrite(attinyPin, HIGH);
    delay(50);
    digitalWrite(attinyPin, LOW);
    delay(50);
    digitalWrite(attinyPin, HIGH);
    delay(50);
    digitalWrite(attinyPin, LOW);
    delay(1000);

    lcd.clear();
    lcd.print("Don't forget to ");
    lcd.setCursor(0,1);
    lcd.print("  shut me off!  ");
    delay(2000);

    waitForFinger(); //tap to continue to enroll

    while(true){
      //save a new fingerprint

      //prints message to lcd
      lcd.clear();
      lcd.print(" So you want to ");
      lcd.setCursor(0,1);
      lcd.print("scan a new one? ");
      delay(2000);

      //Copied and slightly modified from the enroll example:
      int enrollid = 1;

      //choosing which id to overwrite/create
      //release your finger when you want to write to the id printed on the screen

      waitForFinger(); //waits for the fps to be pressed
      if(fps.IsPressFinger() == true){
        lcd.clear();
        lcd.print("       1?       ");
        delay(1000);
        if(fps.IsPressFinger() == true){
          lcd.clear();
          lcd.print("       2?       ");
          delay(1000);
          if(fps.IsPressFinger() == true){
            lcd.clear();
            lcd.print("       3?       ");
            delay(1000);
            if(fps.IsPressFinger() == true){
              lcd.clear();
              lcd.print("       4?       ");
              delay(1000);
              if(fps.IsPressFinger() == true){
                lcd.clear();
                lcd.print("       5?       ");
                delay(1000);
                if(fps.IsPressFinger() == true){
                  lcd.clear();
                  lcd.print("       6?       ");
                  delay(1000);
                  if(fps.IsPressFinger() == true){
                    lcd.clear();
                    lcd.print("       7?       ");
                    delay(1000);
                    if(fps.IsPressFinger() == true){
                      lcd.clear();
                      lcd.print("       8?       ");
                      delay(1000);
                      if(fps.IsPressFinger() == true){
                        lcd.clear();
                        lcd.print("       9?       ");
                        delay(1000);
                        if(fps.IsPressFinger() == true){
                          lcd.clear();
                          lcd.print("      10?       ");
                          delay(1000);
                          enrollid = 10; //can be expanded to up to 20
                        }
                        else enrollid = 9;
                      }
                      else enrollid = 8;
                    }
                    else enrollid = 7;
                  }
                  else enrollid = 6;
                }
                else enrollid = 5;
              }
              else enrollid = 4;
            }
            else enrollid = 3;
          }
          else enrollid = 2;
        }
        else enrollid = 1;
      }

      //warning if there is already data in this id slot
      if(fps.CheckEnrolled(enrollid)){ 
        lcd.clear();
        lcd.print(" Warning! ID #");
        lcd.print(enrollid);
        lcd.setCursor(0,1);
        lcd.print(" has data. OK?  ");

        waitForFinger(); //waits for the fps to be pressed

        fps.DeleteID(enrollid); //delete data
        delay(100);
      }

      //Enroll
      fps.EnrollStart(enrollid);
      lcd.clear(); 
      lcd.print("Place finger to ");
      lcd.setCursor(0,1);
      lcd.print("enroll #");
      lcd.print(enrollid); //prints id that is being enrolled
      waitForFinger(); //waits for the fps to be pressed

      //captures the finger and saves to memory three times for accurate data
      bool bret = fps.CaptureFinger(true); //high quality pic for enrollment
      int iret = 0; //error stuff

      if (bret != false){ //first enroll
        lcd.clear();
        lcd.print(" Remove finger  ");
        fps.Enroll1();
        while(fps.IsPressFinger() == true) delay(100); //waits until no finger
        lcd.clear();
        lcd.print("  Press again   ");
        waitForFinger(); //waits for the fps to be pressed
        bret = fps.CaptureFinger(true);

        if (bret != false){ //second enroll
          lcd.clear();
          lcd.print(" Remove finger  ");
          fps.Enroll2();
          while(fps.IsPressFinger() == true) delay(100);
          lcd.clear();
          lcd.print("Press yet again ");
          waitForFinger(); 
          bret = fps.CaptureFinger(true);

          if (bret != false){ //third enroll
            iret = fps.Enroll3();
            if (iret == 0){ //checks to see if there are any errors
              lcd.clear();
              lcd.print("    Success!    ");
              delay(2000);
              beep(); //shuts arduino off
            }
            else{ //if the enrollment fails in any way
              lcd.clear();
              lcd.print("Fail. Try again ");
              delay(1000);
            }
          }
          lcd.clear();
          lcd.print("   Failed 3rd   "); //error on 3rd
          delay(1000);
        }
        lcd.clear();
        lcd.print("   Failed 2nd   "); //error on 2nd
        delay(1000);
      }
      lcd.clear();
      lcd.print("   Failed 1st   "); //error on 1st
      delay(1000);
    }
  }

  else{
    lcd.print("Fingerprint is"); //if print isn't recognized
    lcd.setCursor(0,1);
    lcd.print("   unverified   ");
    delay(2000);
    lcd.clear();
    lcd.print("Please try again");
    lcd.setCursor(0,1);
    lcd.print("Use your pointer"); //I scanned everyone's pointer finger
    delay(500);
  }
  delay(250);
}


void beep(){ 
  //beeps in hopes of someone closing the case
  lcd.clear();
  lcd.print("Please close the");
  lcd.setCursor(0,1);
  lcd.print("     case!      ");
  for(int i=0;i<8;i++){
    tone(buzzerPin, 262, 500);
    delay(1000);
  }
  delay(5000); //wait for someone to close the case
  //if no one does, shut everything off
  lcd.clear();
  digitalWrite(backlightPin, LOW);
  fps.SetLED(LOW);
  while(true) delay(10000);
}

void waitForFinger(){
  timer = 0; //resets the timer everytime this function starts
  while(fps.IsPressFinger() == false){ //timeout
    timer++;
    delay(100); 
    if (timer>=80){
      timer = 0; //reset timer
      break;
    }
  }
  if(fps.IsPressFinger() == false)beep(); 
  timer = 0; //resets the timer everytime this function ends
}
