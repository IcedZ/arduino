/*
 IR Proximity Test
 This is to test the functionality of the IR Proximity detector - A Sharp GP2Y0A21
 The sensor has three wires:
 Red: VCC (+5.0VDC)
 Black: GND
 Yellow: Analog out (3.1V @ 10cm, 0.4V @ 80cm)
 
 The circuit:
 * The yellow wire is attached to any analog pin on the arduino (A0 for this test)
 
 */

int sensorPin = A0;    // select the input pin for the potentiometer
int sensorValue = 0;  // variable to store the value coming from the sensor
int ledPin = 13;

void setup() {
// nothing here
pinMode(ledPin, OUTPUT);
}

void loop() {
  // read the value from the sensor:
  sensorValue = analogRead(sensorPin);    
  // turn the ledPin on
  digitalWrite(ledPin, HIGH);  
  // stop the program for <sensorValue> milliseconds:
  delay(sensorValue);          
  // turn the ledPin off:        
  digitalWrite(ledPin, LOW);   
  // stop the program for for <sensorValue> milliseconds:
  delay(sensorValue);                  
}
