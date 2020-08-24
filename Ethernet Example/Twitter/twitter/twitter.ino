/*******************************************************************************************
 * 
 * REPLACE TOKEN
 * 
 *********************************************************************************************/

#if defined(ARDUINO) && ARDUINO > 18
#include <SPI.h>
#endif
#include <Ethernet.h>
#include <Twitter.h>

// MAC address
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0xD7, 0x1C };  
//the IP address for the shield:
byte ip[] = { 10, 20, 30, 33 };    

Twitter twitter("xxxxxxxxx-xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");  // Token obtained from arduino-tweet.appspot.com
char msg[] = "Hello, World! I'm Arduino Ethernet Shield!";

void setup()
{
  Ethernet.begin(mac, ip);
  Serial.begin(9600);
  
  delay (1000);
  
  Serial.println("connecting ...");
    if (twitter.post(msg)) {
      int status = twitter.wait(&Serial);
      if (status == 200) {
        Serial.println("OK.");
      } else {
      Serial.print("failed : code ");
      Serial.println(status);
    }
  } else {
    Serial.println("connection failed.");
  }
}

void loop() {}
