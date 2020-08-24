#define relay1  2
#define relay2  3
#define relay3  4
#define relay4  5
#define relay5  6
#define relay6  7
#define relay7  8
#define relay8  9


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  pinMode(relay5, OUTPUT);
  pinMode(relay6, OUTPUT);
  pinMode(relay7, OUTPUT);
  pinMode(relay8, OUTPUT);
  
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW);
  digitalWrite(relay3, LOW);
  digitalWrite(relay4, LOW);
  digitalWrite(relay5, LOW);
  digitalWrite(relay6, LOW);
  digitalWrite(relay7, LOW);
  digitalWrite(relay8, LOW);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Testing relays");
  delay(5000);
  relayOn(relay1);
  delay(100);
/*  Serial.println("Relay 2 ON");
  digitalWrite(relay2, HIGH);
  delay(100);
  Serial.println("Relay 3 ON");
  digitalWrite(relay3, HIGH);
  delay(100);
  Serial.println("Relay 4 ON");
  digitalWrite(relay4, HIGH);
  delay(100);
  Serial.println("Relay 5 ON");
  digitalWrite(relay5, HIGH);
  delay(100);
  Serial.println("Relay 6 ON");
  digitalWrite(relay6, HIGH);
  delay(100);
  Serial.println("Relay 7 ON");
  digitalWrite(relay7, HIGH);
  delay(100);
  Serial.println("Relay 8 ON");
  digitalWrite(relay8, HIGH);
  delay(1000);
 */
  
  Serial.println("Relay 1 OFF");
  digitalWrite(relay1, LOW);
  delay(100);
/*  Serial.println("Relay 2 OFF");
  digitalWrite(relay7, LOW);
  delay(100);
  Serial.println("Relay 3 OFF");
  digitalWrite(relay6, LOW);
  delay(100);
  Serial.println("Relay 4 OFF");
  digitalWrite(relay5, LOW);
  delay(100);
  Serial.println("Relay 5 OFF");
  digitalWrite(relay4, LOW);
  delay(100);
  Serial.println("Relay 6 OFF");
  digitalWrite(relay3, LOW);
  delay(100);
  Serial.println("Relay 7 OFF");
  digitalWrite(relay2, LOW);
  delay(100);
  Serial.println("Relay 8 OFF");
  digitalWrite(relay1, LOW);
  delay(1000);
*/  
}

void relayOn(int relay) {
  
  Serial.print("Relay ");
  Serial.print(relay);
  Serial.print(" ON\n");
  digitalWrite(relay, HIGH);
  
}
