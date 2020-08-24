/* Example for PIR motion sensor SE-10
Wiring: Red: +5V, White: GND, Black: Alarm - put 10k resistor to +5V (pull-up)
Arduino: Alarm pin to A0
*/
 int timer = 500;
 int alarmPin = 0;
 int alarmValue = 0;
 int ledPin = 13;

 void setup () {
   Serial.begin (9600);
   pinMode(ledPin, OUTPUT);  
   pinMode(alarmPin, INPUT);
   delay (2000); // it takes the sensor 2 seconds to scan the area around it before it can detect infrared presence. 
 }

 void loop (){
   alarmValue = analogRead(alarmPin);
   if (alarmValue < 100){
     blinky(); // blinks when the motion has been detected, just for confirmation.
   }
   delay(timer);
   Serial.println (alarmValue);
   delay (10);
 }

 void blinky() {
  for(int i=0; i<3; i++) {
    digitalWrite(ledPin,HIGH);
    delay(200);
    digitalWrite(ledPin,LOW);
    delay(200);
  }
 }
