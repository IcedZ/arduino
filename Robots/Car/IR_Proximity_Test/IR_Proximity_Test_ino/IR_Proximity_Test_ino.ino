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

#include <DistanceGP2Y0A21YK.h>

DistanceGP2Y0A21YK Dist;
int distance;

void setup()
{
  Serial.begin(9600);
  Dist.begin(A0);
}

void loop()
{
  distance = Dist.getDistanceCentimeter();
  Serial.print("\nDistance in inches: ");
  Serial.print(distance);  
  delay(500); //make it readable
}
