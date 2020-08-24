// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

#include <MicroView.h>

MicroViewWidget *widget;		// declaring an array of 2 MicroViewWidget

#define buttonPin 2     // Pin the button is connected to
#define launchPin 3     // Pin the launch relay is connected to
#define timer 10      // Countdown time

int buttonState = 0;    // Default button press to "no"

void setup() {

  // Initialize button as an input
  pinMode(buttonPin, INPUT);
  // Initialize relay pin as an output
  pinMode(launchPin, OUTPUT);

  uView.begin();          // Start MicroView
  uView.clear(PAGE);      
  
  Serial.begin(9600); 
  Serial.println("Launcher 0.1");
  Serial.println("Push button to launch");

  uView.print("Push button to launch");
  uView.display();
  delay(50);     
}

void loop() {
  
  // read the button
  buttonState = digitalRead(buttonPin);
  
  if (buttonState == HIGH) {
    launch();
    
    uView.clear(PAGE);
    uView.setCursor(0,0); 
    uView.print("Push button to launch");
    uView.display();
  }

}

void launch() {  // This function starts countdown and turns on relay

  buttonState = 0;
  uView.clear(PAGE);
  uView.display();
  widget = new MicroViewGauge(32,30,0,timer);  // draw Gauge widget at x=32,y=30,min=0, max=timer (10)
  for(int i = timer; i > 0; i--) {
    widget->setValue(i);
    uView.display();
    delay(1000);
  }
  
  uView.clear(PAGE);
  uView.display();
  delay(10);
  uView.setCursor(0,0); 
  uView.print("Blastoff!");
  uView.display();
  digitalWrite(launchPin, HIGH); 
  
  delay(2500);
  
  digitalWrite(launchPin, LOW);   
  
}
