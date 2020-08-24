// Sweep
// This is to test the Futaba S3003 steering servo on the Traxxas Stampeded turned robot.
// This is a 6 volt servo, powered by the 5V rail on the Arduino board seems to work
// Red = VCC
// Black= GND
// Yellow = PWM (Pin 9 for this example)

#include <Servo.h> 
 
Servo myservo;  // create servo object to control a servo 
                // a maximum of eight servo objects can be created 
#define straight 91
#define turnleft 117
#define turnright 70
 
int pos = straight;          // Position of servo (will be assigned straight, left, or right)
 
void setup() 
{ 
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object 
} 
 
 
void loop() 
{ 
  delay(15);                       // waits 15ms for the servo to reach the position 
  pos = straight;
  myservo.write(pos);              // tell servo to go to position in variable 'pos' 
  delay(1500);                     // waits 1.5s for the servo to reach the position 
  
  pos = turnleft;
  myservo.write(pos);              // tell servo to go to position in variable 'pos' 
  delay(1500);                     // waits 1.5s for the servo to reach the position 

  pos = turnright;
  myservo.write(pos);              // tell servo to go to position in variable 'pos' 
  delay(1500);                     // waits 1.5s for the servo to reach the position 
/*
  pos = turnleft;
  myservo.write(pos);              // tell servo to go to position in variable 'pos' 
  delay(1500);                     // waits 1.5s for the servo to reach the position 
*/
} 
