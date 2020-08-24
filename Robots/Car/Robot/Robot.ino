/*

This is the main code for the robot car. It is just a simple obstacle avoiding robot.
Obstacle Sensor: Forward-pointing Sharp GP2Y0A21 IR sensor
Motors and steering servo are part of the original RC car platform
Motors are controlled by built-in ESC (HPI Micro Pro Controller)
Servo is connected and controlled directly from Arduino

The robot goes straight at near full speed by default as long as there are no obstacles directly in front of it. 
If an obstacle is detected within a set range (this will be a variable), it will slow down and make a random right or left turn.
If there is no obstacle in front, it will speed up and repeat.

*/

#include <Servo.h> 
 
Servo myservo;  // create servo object to control a servo 

/* Steering Servo Definitions */
#define straight 90          // Tell the Servo to steer straight
#define turnleft 70          // Tell the Servo to steer full left
#define turnright 112        // Tell the Servo to steer full right

/* ESC definitions */
#define speedstop 200        // Stop motors
#define speedminforward 180  // Minimum Forward Speed
#define speedmaxforward 10   // Maximum Forward Speed
#define speedminreverse 220  // Minimum Reverse Speed
#define speedmaxreverse 250  // Maximum Reverse Speed

int pos = straight;          // 'pos' is used to tell the car which way to steer (will be assigned straight, left, or right) - initialize to straight

int escpin = 3;              // ESC orange connected to digital pin 9 on Uno
int velocity = speedstop;    // Velocity Variable is used to define the speed of the car - initalize to stop

int LEDpin = 13;

/* Initialize/setup obstacle detection */
int distance = A0;            // Distance sensor output is set to pin A0 (pin 1 on sensor [yellow])
int distancevalue = 0;        // variable to store the value coming from the sensor
int slowdistance = 100;        // Distance from object car slows down (to turn) - lower is further
int stopdistance = 200;        // Distance from object car stops moving - lower is further
int randNumber = 0;          // initialize random number for random function later

void setup() 
{ 
  Serial.begin(9600);
  myservo.attach(8);  // Attaches the servo on pin 8 to the servo object 
  pinMode(LEDpin, OUTPUT);
  randomSeed(analogRead(4));  // Seed for random function - assign to unused anaolog input
  initialize();
} 

void loop()           // Time to do robot stuff!
{
  if (!checkobstacle())    // Make sure there isn't an obstacle in front of the car
  {
    Serial.println("Moving Forward");
    delay(25);
    carforward(175);    // Start the car moving forward slowly
    digitalWrite(LEDpin, HIGH);
    delay(10);
  }
  else
  {
    carstop();
    // Slow down car instead of stop, and initate turn
    digitalWrite(LEDpin, LOW);

    // Randomly decide which way to turn the car (left/right) - 0 = left | 1 = right
    randNumber = random(0,2);  // random function picks a number 0 or 1
    if (randNumber) {                  // If "randNumber" is 1, then turn right
      Serial.println("Turning Right");
      delay(1000);
      pos = turnright;                 // turn the wheels right
      myservo.write(pos);              // tell servo to go to position in variable 'pos' 
      delay(250);                     // waits 250ms for the servo to reach the position  
    }
    
    else {                            // Otherwise "randNumber" is 0, so turn left
      Serial.println("Turning Left");
      delay(1000);
      pos = turnleft;                // turn the w heels right
      myservo.write(pos);            // Send position to servo
      delay(250);                    // wait 250ms for servo to reach position
    }

    // Now that wheels are turned, continue to go forard slowly, until there is no obstacle in front
    do {
      Serial.println("Evading obstacle");
      delay(25);
      carforward(175);
    } while (checkobstacle());
    //Straighten the wheels
    Serial.println("Turning Straight");
    delay(1000); 
    pos = straight;
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(250);                     // waits 250ms for the servo to reach the position 
  }
}


void initialize()     //initialize stuff - just looks cool
{
 
  velocity = speedstop;           // Make sure car isn't moving at start
  
  // Test out the steering servo
  pos = turnright;
  myservo.write(pos);              // tell servo to go to position in variable 'pos' 
  delay(250);                     // waits 300ms for the servo to reach the position 
  pos = turnleft;
  myservo.write(pos);              // tell servo to go to position in variable 'pos' 
  delay(250);                     // waits 1.5s for the servo to reach the position 
  pos = straight;
  myservo.write(pos);              // tell servo to go to position in variable 'pos' 
  delay(250);                     // waits 250ms for the servo to reach the position 
}

int checkobstacle()
{
  distancevalue = analogRead(distance);
  if (distancevalue >= stopdistance)
    return 1;
  else
    return 0;
}

void carforward(int x)      // Makes the car go forward. Input to function is a percent of maximum
{
  analogWrite(escpin, x);
}

void carstop()              // Stops the car
{
  analogWrite(escpin, speedstop);
}

void carreverse(int x)      // Makes the car go reverse. Input to function is a percent of maximum
{
  analogWrite(escpin, x);
}

/* Function to make the care move forward or backward.  
dir is 0 or 1. 0 is reverse, 1 is forward
scale is % of full speed. Can be 0-100
*/
void carmove(int dir, int scale)      
{
  if(dir)    // direction is forward
  {
    // Do forward calculations
    
  }
  else      // direction is reverse
  {
    // Do reverse calculations
  }
}
