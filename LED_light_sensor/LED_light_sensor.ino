/*
This will use an LED to detect if there is light present.  
*/
  const int threshold = 100;  // The ADC value the LED must reach to be considered "on"
  int sensorvalue = 0;
// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(9600);       // initialize the serial port - for testing
  // initialize the digital pin as an output.
}

// the loop routine runs over and over again forever:
void loop() { 
  int compare0 = 0;
  int compare1 = 0;
  int compare2 = 0;
  int compare3 = 0;
  int compare4 = 0;

  sensorvalue = analogRead(0);
  compare0 = sensorvalue; 
  delay(15);
  sensorvalue = analogRead(0);
  compare1 = sensorvalue; 
  delay(15);
  sensorvalue = analogRead(0);
  compare2 = sensorvalue; 
  delay(15);
  sensorvalue = analogRead(0);
  compare3 = sensorvalue; 
  delay(15);
  sensorvalue = analogRead(0);
  compare4 = sensorvalue; 
  delay(15);
  
  if (compare4 > compare3){
    if (compare3 > compare2)
      Serial.println("Light!"); 
    delay(20);
  }
  
  else
  {
    Serial.println("Nope"); 
    delay(20);

  } 
}
