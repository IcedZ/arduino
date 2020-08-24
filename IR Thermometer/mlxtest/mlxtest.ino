 /*************************************************** 

  Designed specifically to work with the MLX90614 sensors

  These sensors use I2C to communicate, 2 pins are required to  

Assumptions:
 - Noise / interference will not exceed trigger temperature

Sensor:

PWR -> 3.3V
GND -> GND
SCL -> PIN A5 (10k pull-up)
SDA -> PIN A4 (10k pull-up)
  
 
 Left to do:
 12/10/18: 
  - Need a simple-(ish) way to calibrate the device so it knows it's seeing the stove top
  - Program Snooze button (large single-press button, like snooze on alarm clock)
  - Program dismiss button (probably more than a simple press; either long press, 2-factor, or multiple push)
  - consider failsafe for unsigned int overflow?
  - Final product will measure significantly less frequently to save battery; likely 1 sample per minute (or 1 per 2-minutes)
 
 
 ****************************************************/

#include <Wire.h>
#include <Adafruit_MLX90614.h>

#define triggerTemp  24.0      // in Deg C
#define highTempTime 5        // seconds before alarm is triggered (~30 mins?) (based on 1000ms delay at the end of loop())

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

float ObjectTempC = 0.0;            // variable used for storing the measured object (stove top) temperature

float tempSum = 0.0;                // variable for keeping running sum to be used by average
unsigned int hot_duration = 0;      // counter used for incrementing the number of measurements that are above the threshold
float avgTemp = 0.0;                // calculated by using tempSum / hot_duration

unsigned int hotAlarm = 0;          // variable used for counting how many times the average exceeds the triggerTemp; if this exceeds highTempTime; it alarms

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.begin(9600);
  mlx.begin();
}

void loop() {
  ObjectTempC = mlx.readObjectTempC();
/*
  Serial.print("Ambient = "); Serial.print(mlx.readAmbientTempC()); 
  Serial.print("*C\tObject = "); Serial.print(mlx.readObjectTempC()); Serial.println("*C");
  Serial.print("Ambient = "); Serial.print(mlx.readAmbientTempF()); 
  Serial.print("*F\tObject = "); Serial.print(mlx.readObjectTempF()); Serial.println("*F");
*/
  
  /*
   * As soon as a high temperature threshold is read, start averaging
   *  If average > threshold for extended duration (HighTempTime), then start alarm
   */

  
  if (ObjectTempC >= triggerTemp)
  {
    if (avgTemp <= triggerTemp)
    {
      tempSum = ObjectTempC;                  // assign the average temperature counter to the detected temperature only if the current average temp is below the threshold; presumably the first iteration of this loop.
    }

    if (hot_duration > 0)
      tempSum = tempSum + ObjectTempC;      // On every subsequent iteration of the loop, add up the tempSum parameter for averaging.

    avgTemp = tempSum / hot_duration;        // recalculate the average temp
    
    hot_duration++;                         // increment if the measured temperature is greater than the trigger temp - should do averaging algorithm instead
  }
  
  else if (ObjectTempC < triggerTemp && avgTemp >= triggerTemp)
  {
    tempSum = tempSum + ObjectTempC;
    avgTemp = tempSum / hot_duration;
    hot_duration++;
  }
  
  if (avgTemp < triggerTemp)
  {
    // Reset variables for next time temp gets high
    float tempSum = 0.0;                // variable for keeping running sum to be used by average
    unsigned int hot_duration = 0;      // counter used for incrementing the number of measurements that are above the threshold
    float avgTemp = 0.0;                // calculated by using tempSum / hot_duration
    unsigned int hotAlarm = 0;          // variable used for counting how many times the average exceeds the triggerTemp; if this exceeds highTempTime; it alarms
    digitalWrite(LED_BUILTIN, LOW);
    hot_duration = 0;
  }

  if (hotAlarm >= highTempTime)
  {
    // Call the alarm function
    // but for now:
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("HOT!!!");
  }

  if (avgTemp >= triggerTemp) 
  {
    hotAlarm++;
  }
  
  Serial.print("Object = "); Serial.print(ObjectTempC); Serial.println("*C");
  Serial.print("Hot-Duration: ");Serial.println(hot_duration);
  Serial.print("Avg Temp: "); Serial.println(avgTemp);
  Serial.print("Hot Alarm: "); Serial.println(hotAlarm);
  Serial.println();
  delay(1000);                                // Approx 1-second measuring interval.  
}
