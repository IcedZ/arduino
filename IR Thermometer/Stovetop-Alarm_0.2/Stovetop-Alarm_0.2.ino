 /*************************************************** 

  Designed specifically to work with the MLX90614 sensors

  These sensors use I2C to communicate, 2 pins are required to  

Assumptions:
 - Noise / interference will not exceed trigger temperature
 - Seems to work best with sensor directly in center above burners

Sensor:

PWR -> 3.3V
GND -> GND
SCL -> PIN A5 (10k pull-up)
SDA -> PIN A4 (10k pull-up)
  
 
 Left to do:
 12/10/18: 
  - Need a simple-(ish) way to calibrate the device so it knows it's seeing the stove top
    - "test" button (long-press small inset button) that puts the device into test mode:
       - Timer is set to 5-10 minutes (arbirtrary) and must boil a pot of water on each burner, with it cooling down completely in between each.  Alarm must go off for each burner.
       - During calibration, alarms don't sound just light / maybe different sound
  - Program Snooze button (large single-press button, like snooze on alarm clock)
  - Program dismiss button (probably more than a simple press; either long press, 2-factor, or multiple push)
  - Final product will measure significantly less frequently to save battery; likely 1 sample per minute (or 1 per 2-minutes)

 Basic operation:
 Flag is triggered if the temperature threshold is exceeded.
 Loop continues to average readings as long as flag is true
 Flag stays true until average temperature drops below threshold
  If average temp doesn't drop below threshold in certain duration, alarms happen
 
 ****************************************************/


/*
  LiquidCrystal Library - Hello World
 
 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the 
 Hitachi HD44780 driver.
 
 
  The circuit:
 * LCD RS pin to digital pin 8
 * LCD Enable pin to digital pin 9
 * LCD D4 pin to digital pin 10
 * LCD D5 pin to digital pin 11
 * LCD D6 pin to digital pin 12
 * LCD D7 pin to digital pin 13
 * LCD R/W pin to ground
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

Piezo goes on PWM pin, driven by tone() function. Put ~100 ohm resistor inline with Piezo
 
 */
 

#include <Wire.h>
#include <Adafruit_MLX90614.h>

#define triggerTemp  30.0      // in Deg C - ~45  probably used for production.  25-30 used for testing
#define highTempTime 5       // seconds before alarm is triggered (~30 mins?) (based on 1000ms delay at the end of loop()) - test mode, should be ~5 mins

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

bool flagTempExceeded = 0;          // flag used to determine if averaging is needed. Set to true when triggerTemp is met or exceeded, stays true until *avgTemp* falls below triggerTemp

float objectTempC = 0.0;            // variable used for storing the measured object (stove top) temperature

float tempSum = 0.0;                // variable for keeping running sum to be used by average
unsigned int tempSumCounter = 0;    // counter used for incrementing the number of measurements that are above the threshold
float avgTemp = 0.0;                // calculated by using tempSum / tempSumCounter

unsigned int hotAlarm = 0;          // variable used for counting how many times the average exceeds the triggerTemp; if this exceeds highTempTime; it alarms


/* LCD STUFF */
#include <LiquidCrystal.h>

//Pin assignments for LCD
static const int RS = 8;
static const int E = 9;
static const int DB4 = 10;
static const int DB5 = 11;
static const int DB6 = 12;
static const int DB7 = 13;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(RS, E, DB4, DB5, DB6, DB7);

#define BACKLITE 6
#define CONTRAST 5        // set this to low PWM value to adjust contrast

int piezoPin = 7;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.begin(9600);
  mlx.begin();

  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("TEMP *C: ");
  
  pinMode(BACKLITE, OUTPUT);
  analogWrite(BACKLITE, 90);

  pinMode(CONTRAST, OUTPUT);
  analogWrite(CONTRAST,90);
  
}

void loop() {

  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  //lcd.setCursor(0, 0);
  
  objectTempC = mlx.readObjectTempC();      // Read the temperature of the stovetop
/*
  Serial.print("Ambient = "); Serial.print(mlx.readAmbientTempC()); 
  Serial.print("*C\tObject = "); Serial.print(mlx.readObjectTempC()); Serial.println("*C");
  Serial.print("Ambient = "); Serial.print(mlx.readAmbientTempF()); 
  Serial.print("*F\tObject = "); Serial.print(mlx.readObjectTempF()); Serial.println("*F");
*/
 
  
  if (objectTempC >= triggerTemp)           // If the temp is too high, make flag true; flag tells algorighm to start averaging.
  {
    flagTempExceeded = 1;
  }

  if (flagTempExceeded == 1)                // High temps were detected, begin averaging.
  {
    tempSum = tempSum + objectTempC;        // add the measured temp to the rolling temperature sum for future averaging
    tempSumCounter++;                       // increment the counter to be used in the average calculation
    avgTemp = tempSum / tempSumCounter;     // calculate the new average

    if (avgTemp < triggerTemp)              // in future, call function to reset variables
    {
      flagTempExceeded = 0;                 // avgTemp is the only thing that can reset the flag, see if below threshold 
      // Reset variables
      tempSum = 0.0;                // variable for keeping running sum to be used by average
      tempSumCounter = 0;    // counter used for incrementing the number of measurements that are above the threshold
      avgTemp = 0.0;                // calculated by using tempSum / tempSumCounter
      hotAlarm = 0;          // variable used for counting how many times the average exceeds the triggerTemp; if this exceeds highTempTime; it alarms
    }

    else if (avgTemp >= triggerTemp)        // if avgTemp is high, start counting how long
    {
      hotAlarm++;

    
      if (hotAlarm >= highTempTime)         // Sound the alarms!
      {
        digitalWrite(LED_BUILTIN, HIGH);
        Serial.println("HOT!!!");
        // Call function to sound alarm
        tone(piezoPin, 2000, 100);
        delay(200);
        tone(piezoPin, 3000, 100);
        delay(200);
        tone(piezoPin, 2000, 100);
        delay(200);
        tone(piezoPin, 3000, 100);
        }
    }
    
  }

  else                                      // low temps all around
  {
    Serial.println("nothing to see here");
  }


  // Always do this stuff - mostly debugging
/*  
  Serial.print("Object = "); Serial.print(objectTempC); Serial.println("*C");
  Serial.print("temp-sum-counter: ");Serial.println(tempSumCounter);
  Serial.print("Avg Temp: "); Serial.println(avgTemp);
  Serial.print("Hot Alarm: "); Serial.println(hotAlarm);
  Serial.println();
*/

  // print temps:
  lcd.setCursor(0, 0);
  lcd.print("OBJ: ");lcd.print(objectTempC);
  lcd.setCursor(0,1);
  lcd.print("AVG: ");lcd.print(avgTemp);
  delay(1000);                                // Approx 1-second measuring interval.  
}
