/**************************************************************
 * REPLACE TOKEN
 *************************************************************/



/* 
When the PIR sensor (SE-10) detects motion, it posts a message to twitter.
*** PIR Wiring ***
Wiring: Red: +5V, White: GND, Black: Alarm - put 10k resistor to +5V (pull-up)
Arduino: Alarm pin to A0
*/

// Ethernet setup stuff
#if defined(ARDUINO) && ARDUINO > 18
#include <SPI.h>
#endif
#include <Ethernet.h>
#include <Twitter.h>
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0xD7, 0x1C };  // MAC address
byte ip[] = { 10, 20, 30, 33 };  //the IP address for the shield:
Twitter twitter("xxxxxxxxx-xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");  // Token obtained from arduino-tweet.appspot.com
unsigned int tweetcount = 0;

// PIR Setup stuff
int timer = 400;
int alarmPin = 0;
int alarmValue = 0;
int ledPin = 13;

void setup () {
  Serial.begin (9600);
  // Ethernet Setup
  Ethernet.begin(mac, ip);
  delay (1000);  // Wait for Ethernet to setup
  // PIR Setup
  pinMode(ledPin, OUTPUT);  
  pinMode(alarmPin, INPUT);
  delay (2000); // it takes the sensor 2 seconds to scan the area around it before it can detect infrared presence. 
 }
 
void loop () {
  alarmValue = analogRead(alarmPin);
  if (alarmValue < 100){
    tweet(); // Post to twitter
  }
  delay(timer);
  Serial.println (alarmValue);
}

void tweet() {
  tweetcount++;
  char msg2[40];
  sprintf( msg2, "%d - Movement Detected", tweetcount );
  Serial.println("connecting ...");
  if (twitter.post(msg2)) {
    int status = twitter.wait(&Serial);
    if (status == 200) {
      Serial.println("OK.");
    } 
    else {
      Serial.print("failed : code ");
      Serial.println(status);
    }
  } 
  else {
    Serial.println("connection failed.");
  }
  delay (60000);  // Wait at least one minute before re-posting
}
