// Sweep
// by BARRAGAN <http://barraganstudio.com> 
// This example code is in the public domain.


#include <Servo.h> 
 
Servo myservo;  // create servo object to control a servo 
                // a maximum of eight servo objects can be created 
 
int pos = 90;    // variable to store the servo position 
int leftpos = 70;
int rightpos = 112;
 
void setup() 
{ 
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object 
  // initialize serial communications at 9600 bps:
  Serial.begin(9600); 
} 
 
 
void loop() 
{ 
    delay(1000);                       // waits 15ms for the servo to reach the position 
  for(pos = leftpos; pos < rightpos; pos += 1)  // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(50);                       // waits 15ms for the servo to reach the position 
    Serial.print("\t angle = ");      
    Serial.println(pos);  
  } 
  delay(2000);
  for(pos = rightpos; pos>=leftpos; pos-=1)     // goes from 180 degrees to 0 degrees 
  {                                
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(50);                       // waits 15ms for the servo to reach the position 
    Serial.print("\t angle = ");      
    Serial.println(pos); 
  } 
  delay(2000);
} 
