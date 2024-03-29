
/*
  Keyboard logout

  This sketch demonstrates the Keyboard library.

  When you connect pin 2 to ground, it performs a logout.
  It uses keyboard combinations to do this, as follows:

  On Windows, CTRL-ALT-DEL followed by ALT-l
  On Ubuntu, CTRL-ALT-DEL, and ENTER
  On OSX, CMD-SHIFT-q

  To wake: Spacebar.

  Circuit:
  - Arduino Leonardo or Micro
  - wire to connect D2 to ground

  created 6 Mar 2012
  modified 27 Mar 2012
  by Tom Igoe

  This example is in the public domain.

  http://www.arduino.cc/en/Tutorial/KeyboardLogout
*/

#define OSX 0
#define WINDOWS 1
#define UBUNTU 2

#include "Keyboard.h"

// change this to match your platform:
int platform = WINDOWS;

void setup() {
  // make pin 2 an input and turn on the pull-up resistor so it goes high unless
  // connected to ground:
  pinMode(2, INPUT_PULLUP);
  Keyboard.begin();
  delay(5000);
}

void loop() {
  //while (digitalRead(2) == HIGH) {
    // do nothing until pin 2 goes low
  // delay(500);
  //}
  //delay(1000);

  switch (platform) {
    case OSX:
      Keyboard.press(KEY_LEFT_GUI);
      // Shift-Q logs out:
      Keyboard.press(KEY_LEFT_SHIFT);
      Keyboard.press('Q');
      delay(100);
      Keyboard.releaseAll();
      // enter:
      Keyboard.write(KEY_RETURN);
      break;
    case WINDOWS:
      // CTRL-ALT-DEL:
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press(KEY_LEFT_ALT);
      Keyboard.press(KEY_DELETE);
      delay(100);
      Keyboard.releaseAll();
      // ALT-l:
      delay(200);
      Keyboard.press('3');
      Keyboard.releaseAll();
      delay(100);
      Keyboard.press('1');
      Keyboard.releaseAll();
      delay(100);
      Keyboard.press('4');
      Keyboard.releaseAll();
      delay(100);
      Keyboard.press('1');
      Keyboard.releaseAll();
      delay(100);
      Keyboard.press('5');
      Keyboard.releaseAll();
      delay(100);
      Keyboard.press('9');
      Keyboard.releaseAll();
      delay(100);
      Keyboard.press('2');
      Keyboard.releaseAll();
      delay(100);
      Keyboard.write(KEY_RETURN);
      /*  // This doesn't work on Windows 10
      Keyboard.press(KEY_LEFT_ALT);
      Keyboard.press('l');
      Keyboard.releaseAll();
      */
      // Enter to confirm logout:
      //Keyboard.write(KEY_RETURN);
      break;
    case UBUNTU:
      // CTRL-ALT-DEL:
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press(KEY_LEFT_ALT);
      Keyboard.press(KEY_DELETE);
      delay(1000);
      Keyboard.releaseAll();
      // Enter to confirm logout:
      Keyboard.write(KEY_RETURN);
      break;
  }

  // do nothing:
  while (true);
  delay(30000);
}
