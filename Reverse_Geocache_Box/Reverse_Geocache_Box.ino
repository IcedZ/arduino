#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include <TinyGPS.h>
#include <PWMServo.h>
#include <EEPROM.h>

//Pin assignments for LCD
static const int LCD_RS = 8;
static const int LCD_E = 9;
static const int LCD_DB4 = 10;
static const int LCD_DB5 = 11;
static const int LCD_DB6 = 12;
static const int LCD_DB7 = 13;

// Pin assignments for GPS
static const int GPS_rx = 7;
static const int GPS_tx = 6;

/* 
Closed angle and open angle are the angle of the servo to open / close the box
DEST values are the latitude and longitude of the destination
RADIUS is how far away from the target you can be
ATTEMPT_MAX is the maximum number of attempts the user has to open the box before being locked out - set low enough to maintain battery
*/
static const int CLOSED_ANGLE = 90; // degrees
static const int OPEN_ANGLE = 165; // degrees
static const float DEST_LATITUDE = 29.754909;
static const float DEST_LONGITUDE = -82.411636;
static const int RADIUS = 1000; // meters
static const int ATTEMPT_MAX = 50;
// EEPROM offset - don't change
static const int EEPROM_OFFSET = 100;
static const int switch_off = 5; // Pololu switch control to turn the Arduino off

/* Create LCD object, GPS object, and Servo object */
LiquidCrystal lcd(LCD_RS, LCD_E, LCD_DB4, LCD_DB5, LCD_DB6, LCD_DB7);
TinyGPS gps;
SoftwareSerial ss(GPS_rx, GPS_tx);
PWMServo servo;
int attempt_counter;

/* A helper function to display messages of a specified duration */
void Msg(LiquidCrystal &lcd, const char *top, const char *bottom, unsigned long del)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(top);
  lcd.setCursor(0, 1);
  lcd.print(bottom);
  delay(del);
}

void setup()
{
  
  // Begin serial communications (for prototype only, comment out before final)
  Serial.begin(9600);
  
  // Begin communication with GPS module
  ss.begin(9600);
  
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  
  /* Make sure Pololu switch pin is OUTPUT and LOW */
  pinMode(switch_off, OUTPUT);
  digitalWrite(switch_off, LOW);
  
  /* make sure motorized latch is closed */
//  servo.write(CLOSED_ANGLE);

  /* read the attempt counter from the EEPROM */
  attempt_counter = EEPROM.read(EEPROM_OFFSET);
  if (attempt_counter == 0xFF) // brand new EEPROM?
    attempt_counter = 0;

  /* increment it with each run */
  ++attempt_counter;

  // Print a message to the LCD.
  lcd.setCursor(0,0);
  lcd.print(" GPS PUZZLE BOX");
  delay(2500);
  
    /* Game over? */
  if (attempt_counter >= ATTEMPT_MAX)
  {
    Msg(lcd, "Game", "Over", 5000);
    PowerOff();
  }

  /* Print out the attempt counter */
  Msg(lcd, "This is", "attempt", 2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(attempt_counter);
  lcd.print(" of "); 
  lcd.print(ATTEMPT_MAX);
  delay(2000);

  /* Save the new attempt counter */
  EEPROM.write(EEPROM_OFFSET, attempt_counter);

  Msg(lcd, "Seeking","Signal...",1000);
  
}

void loop()
{
  /* Has a valid NMEA sentence been parsed? */
  if (ss.available() && gps.encode(ss.read()))
  {
    float lat, lon;
    unsigned long fix_age;

    /* Have we established our location? */
    gps.f_get_position(&lat, &lon, &fix_age);
    if (fix_age != TinyGPS::GPS_INVALID_AGE)
    {
      /* Calculate the distance to the destination */
      float distance_meters = TinyGPS::distance_between(lat, lon, DEST_LATITUDE, DEST_LONGITUDE);

      /* Are we close?? */
      if (distance_meters <= RADIUS)
      {
        Msg(lcd, "Access", "granted!", 2000);
//        servo.write(OPEN_ANGLE);
      }

      /* Nope.  Print the distance. */
      else
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Distance");
        lcd.setCursor(0, 1);
        if (distance_meters < 1000)
        {
          lcd.print((int)distance_meters);
          lcd.print(" m.");
        }

        else
        {
          lcd.print((int)(distance_meters / 1000));
          lcd.print(" km.");
        }
        delay(4000);
        Msg(lcd, "Access", "Denied!", 2000);
      }

      PowerOff();
    }
  }

  /* Turn off after 5 minutes */
  if (millis() >= 300000)
    PowerOff();
}

/* Called to shut off the system using the Pololu switch */
void PowerOff()
{
  lcd.clear();
  lcd.print("Goodbye!");
  delay(2000);
  lcd.clear(); 
  
  /* Bring Pololu switch control pin HIGH to turn off */
  digitalWrite(switch_off, HIGH);

  /* This is the back door.  If we get here, then the battery power */
  /* is being bypassed by the USB port.  We'll wait a couple of */
  /* minutes and then grant access. */
  delay(5000);
//  servo.write(OPEN_ANGLE); // and open the box 
  lcd.print("secret area");
  /* Reset the attempt counter */
//  EEPROM.write(EEPROM_OFFSET, 0); 
  
  /* Leave the latch open for 10 seconds */
//  delay(10000); 

  /* And then seal it back up */
//  servo.write(CLOSED_ANGLE); 

  /* Exit the program for real */
  exit(1);
} 
