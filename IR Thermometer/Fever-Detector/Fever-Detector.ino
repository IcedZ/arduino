#include <Wire.h>
#include <Adafruit_AMG88xx.h>

// Includes for the screen - remove for production
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>

// Screen definitions
#define TFT_CS     10 //chip select pin for the TFT screen
#define TFT_RST    9  // you can also connect this to the Arduino reset
                      // in which case, set this #define pin to 0!
#define TFT_DC     8
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);


char sensorPrintout[4];

Adafruit_AMG88xx amg;

float pixels[AMG88xx_PIXEL_ARRAY_SIZE];
float highest = 0.0;
float fever_temp = 31.0;
bool flagFever = 0;

void setup() {
    Serial.begin(9600);
    Serial.println(F("AMG88xx pixels"));

    bool status;
    
    // default settings
    status = amg.begin();
    if (!status) {
        Serial.println("Could not find a valid AMG88xx sensor, check wiring!");
        while (1);
    }

    Serial.println();

    delay(100); // let sensor boot up

    // Screen setup stuff
    tft.initR(INITR_BLACKTAB);   // initialize a ST7735S chip, black tab
    tft.fillScreen(ST7735_BLACK);


    delay(100);

}


void loop() { 
  
    //read all the pixels
    amg.readPixels(pixels);

    Serial.print("[");
    for(int i=1; i<=AMG88xx_PIXEL_ARRAY_SIZE; i++){
      Serial.print(pixels[i-1]);
      Serial.print(", ");
      if( i%8 == 0 ) Serial.println();
    }

    // Find the largest value in the array (looking for the highest temperature)
    highest = pixels[1];
    for(int i=1; i<=AMG88xx_PIXEL_ARRAY_SIZE; i++){
      if( pixels[i-1] > highest ) {
        highest = pixels[i-1];
      }
    }
    /*
    Serial.println("]");
    Serial.println();
    Serial.print("Highest temp: ");
    Serial.print(highest);
*/

//    highest.toCharArray(sensorPrintout, 4);

    //delay 

    // tft print function!
    //testText();
    if (highest >= fever_temp)
    {
      flagFever = 1;
    }
    else if (highest < fever_temp)
    {
      flagFever = 0;
    }

    if (flagFever)
    {
      tft.fillScreen(ST7735_BLACK);
      tft.setCursor(2,2);
      tft.setTextColor(ST7735_BLUE);
      tft.setTextSize(3);
      tft.println("FEVER!");
      tft.setTextSize(2);
      tft.println("");
      tft.println("");
      tft.println("T: ");
      tft.println(highest);
      digitalWrite(LED_BUILTIN, HIGH);
    }

    else if (!flagFever)
    {
      tft.setTextColor(ST7735_WHITE);
      tft.fillScreen(ST7735_BLACK);
      tft.setCursor(2,2);
      tft.setTextColor(ST7735_WHITE);
      tft.setTextSize(2);
      tft.println("Max T: ");
      tft.println(highest);
      digitalWrite(LED_BUILTIN, LOW);
    }
      
    delay(300);
}


/*
unsigned long testText() {
  tft.fillScreen(ILI9341_BLACK);
  unsigned long start = micros();
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(1);
  tft.println("Hello World!");
  tft.setTextColor(ILI9341_YELLOW); tft.setTextSize(2);
  tft.println(1234.56);
  tft.setTextColor(ILI9341_RED);    tft.setTextSize(3);
  tft.println(0xDEADBEEF, HEX);
  tft.println();
  tft.setTextColor(ILI9341_GREEN);
  tft.setTextSize(5);
  tft.println("Groop");
  tft.setTextSize(2);
  tft.println("I implore thee,");
  tft.setTextSize(1);
  tft.println("my foonting turlingdromes.");
  tft.println("And hooptiously drangle me");
  tft.println("with crinkly bindlewurdles,");
  tft.println("Or I will rend thee");
  tft.println("in the gobberwarts");
  tft.println("with my blurglecruncheon,");
  tft.println("see if I don't!");
  return micros() - start;
}

 */
