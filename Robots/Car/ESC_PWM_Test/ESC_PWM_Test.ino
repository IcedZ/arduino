/*
 Fading
 
 This example shows how to fade an LED using the analogWrite() function.
 
 The circuit:
 * LED attached from digital pin 9 to ground.
 
 Created 1 Nov 2008
 By David A. Mellis
 modified 30 Aug 2011
 By Tom Igoe
 
 http://arduino.cc/en/Tutorial/Fading
 
 This example code is in the public domain.
 
 */
#define speedstop 200

#define speedminforward 180
#define speedminreverse 220

#define speedmaxforward 10
#define speedmaxreverse 250

int escpin = 9;      // ESC orange connected to digital pin 9
int velocity = speedstop;  

void setup()  { 
  // nothing happens in setup 
} 

void loop()  { 

  analogWrite(escpin, speedstop);
  delay(1000);

  analogWrite(escpin, speedminforward);
  delay(1000);
  
  analogWrite(escpin, speedmaxforward);
  delay(1000);

  analogWrite(escpin, speedstop);
  delay(1000);
  
  analogWrite(escpin, speedminreverse);
  delay(2000);

  analogWrite(escpin, speedmaxreverse);
  delay(2000);

  
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


