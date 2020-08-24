#include "DHT.h"
#define DHTPIN 8
#define DHTTYPE DHT22
  
DHT dht(DHTPIN,DHTTYPE);
float hightemp = 0.0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("DHT22 Test");
  
  dht.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
   delay(2500);  
     // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit
  float f = dht.readTemperature(true);
  
    // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
   return;
  }
    // Compute heat index
  // Must send in temp in Fahrenheit!
  float high = dht.computeHeatIndex(f, h);

  if (t > hightemp) {
    hightemp = t;
  }

  if (t >= 27.90)
    Serial.println ("Akshay Stinks");

  Serial.print("Humidity: "); 
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: "); 
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(high);
  Serial.print(" *F\t");
  Serial.print("High Temp: ");
  Serial.println(hightemp);
}
