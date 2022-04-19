/* Arduino USB Keyboard HID demo
 * Send CTRL+ALT+DEL
 */

#define KEY_LEFT_CTRL  0x80
#define KEY_LEFT_ALT  0x82
#define KEY_DEL  0xD4

uint8_t buf[8] = { 
  0 };  /* Keyboard report buffer */

int state = 1;

void setup() 
{
  Serial.begin(9600);

  delay(300);
}

void loop() 
{
  delay(2000);

  buf[0] = KEY_LEFT_CTRL;   // Ctrl Key
  buf[2] = KEY_LEFT_ALT;    // Alt Key
  buf[4] = KEY_DEL;         // Del Key
  Serial.write(buf, 8);     // Send Keypress
  delay(50);
  releaseKey();
/*  
  state = digitalRead(PIN_CUT);
  if (state != 1) {
    buf[0] = KEY_LEFT_CTRL;   // Ctrl
    buf[2] = 27;    // Letter X
    // buf[2] = 123;    // Cut key: Less portable
    Serial.write(buf, 8); // Ssend keypress
    releaseKey();
  } 

  state = digitalRead(PIN_COPY);
  if (state != 1) {
    buf[0] = KEY_LEFT_CTRL;   // Ctrl
    buf[2] = 6;    // Letter C
    // buf[2] = 124;    // Copy key: Less portable
    Serial.write(buf, 8); // Send keypress
    releaseKey();
  } 

  state = digitalRead(PIN_PASTE);
  if (state != 1) {
    buf[0] = KEY_LEFT_CTRL;   // Ctrl
    buf[2] = 25;    // Letter V
    // buf[2] = 125;    // Paste key: Less portable
    Serial.write(buf, 8); // Send keypress
    releaseKey();
  }
*/ 
}

void releaseKey() 
{
  buf[0] = 0;
  buf[2] = 0;
  buf[4] = 0;
  Serial.write(buf, 8); // Release key  
  delay(500);
}
