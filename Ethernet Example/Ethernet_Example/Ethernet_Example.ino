/*
Arduino Ethernet Test
Ethernet.begin(mac, ip, dns, gateway, subnet); 
Device MAC: 90-A2-DA-0D-D7-1C
*/

byte mac[] = {0x90, 0xA2, 0xDA, 0x0D, 0xD7, 0x1C};
byte ip[] = {10,20,30,18};

void setup() {
  Ethernet.begin(mac, ip);
}

void loop () {
    
}


