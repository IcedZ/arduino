/*

 This example shows how to write PWM to the XL-1 ESC. It defines neutral, minimum and maximum forward and reverse speeds.
 
 The circuit:
 * XL-1 red = 5V (Maybe 7.x?)
 * XL-1 Black = GND
 * XL-1 White = PWM signal (Arduino Pin 10)

 ESC is treated as a servo. 90deg is neutral

According to https://webfiles.uci.edu/cagell/Dreambot/about_me/about_me.htm - PWM from 1000 - 2000 us (1500 neutral?)
 
 Based off of: http://arduino.cc/en/Tutorial/Fading
 
 */

#include <Servo.h> 

#define speedstop 200

#define speedminforward 180
#define speedminreverse 220

#define speedmaxforward 10
#define speedmaxreverse 250

Servo ESC;            // Define the ESC object

void setup()  { 
  ESC.attach(10);
} 

void loop()  { 

  ESC.writeMicroseconds(1500);   // Stop 
  delay(2000);                  // Delay
  ESC.write(1250);              // Reverse?
  delay(2000);                  // Delay
  ESC.write(1750);              // Forward
  delay(2000);                  // Delay
  
  /*  // fade in from min to max in increments of 5 points:
  for(int fadeValue = 0 ; fadeValue <= 255; fadeValue +=5) { 
    // sets the value (range from 0 to 255):
    analogWrite(escpin, fadeValue);         
    // wait for 30 milliseconds to see the dimming effect    
    delay(30);                            
  } 

  // fade out from max to min in increments of 5 points:
  for(int fadeValue = 255 ; fadeValue >= 0; fadeValue -=5) { 
    // sets the value (range from 0 to 255):
    analogWrite(escpin, fadeValue);         
    // wait for 30 milliseconds to see the dimming effect    
    delay(30);                            
  } 
*/
}


