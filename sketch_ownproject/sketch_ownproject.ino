/*
* Name: Own Project 
* Author: Vincent Stridh
* Date: 2025-12-2
* Description: This project uses a rc522 to read the rfid cards and and processes what card it is and then responds in the terminal and on the oled ring, 
* Further, the servo motor will allow me to open and close a door depending on if the rfid card is set to be allowed or not. 
*/

//Import libraries
#include <SPI.h>
#include <MFRC522.h>
#include <VarSpeedServo.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>  // Required for 16 MHz Adafruit Trinket
#endif

//Define variables
int servoPin = 9;
#define RST_PIN 9
#define SS_PIN 10
#define LED_PIN 6
#define LED_COUNT 24
byte readCard[4];
String MasterTag = "9394C1D";  // REPLACE this Tag ID with your Tag ID
String tagID = "";

// Create instances
MFRC522 mfrc522(SS_PIN, RST_PIN);
Adafruit_NeoPixel ring = (LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
VarSpeedServo Servo1;

void setup() {
  Serial.begin(9600);
  SPI.begin();         // SPI bus
  mfrc522.PCD_Init();  // MFRC522
  Servo1.attach(servoPin);
  Servo1.write(150, 50, true);
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif
  ring.begin();
  ring.show();
  ring.setBrightness(50);
}

void loop() {
  //Wait until new tag is available
  while (getID()) {
    if (tagID == MasterTag) {
      Serial.println("Scanning tag: '" + tagID + "'..");
      delay(600);
      Serial.println("Scanning data..");
      //      rainbow(2);
      Serial.println("Access Granted!");
      green_brighten();
      Servo1.write(30, 35, true);
      delay(500);
      green_darken();
      ring.show();
    } else {
      Serial.println("Scanning tag: '" + tagID + "'..");
      delay(600);
      Serial.println("Scanning data..");
      //      rainbow(2);
      Serial.println("Access Denied!");
      red_brighten();
      delay(500);
      red_darken();
      ring.show();
    }
    delay(3500);
    Servo1.write(150, 50, true);
  }
 // rainbow(10);
}

//Function: Turns the leds on the neoring to red
void red_brighten() {
  uint16_t i, j;

  for (j = 45; j < 255; j += 3) {
    for (i = 0; i < ring.numPixels(); i++) {
      ring.setPixelColor(i, j, 0, 0);
    }
    ring.show();
    delay(10);
  }
  ring.clear();
}

//Function: Makes the leds on the neoring return to unlit
void red_darken() {
  Serial.begin(9600);
  uint16_t i, j;

  for (j = 255; j > 45; j -= 3) {
    for (i = 0; i < ring.numPixels(); i++) {
      ring.setPixelColor(i, j, 0, 0);
    }
    ring.show();
    delay(10);
  }
  ring.clear();
}

//Function: Turns the leds on the neoring to green
void green_brighten() {
  uint16_t i, j;

  for (j = 45; j < 255; j++) {
    for (i = 0; i < ring.numPixels(); i++) {
      ring.setPixelColor(i, 0, j, 0);
    }
    ring.show();
    delay(10);
  }
  ring.clear();
}

//Function: Makes the leds on the neoring return to unlit
void green_darken() {
  Serial.begin(9600);
  uint16_t i, j;

  for (j = 255; j > 45; j--) {
    for (i = 0; i < ring.numPixels(); i++) {
      ring.setPixelColor(i, 0, j, 0);
    }
    ring.show();
    delay(10);
  }
  ring.clear();
}

// Rainbow cycle along whole ring. Pass delay time (in ms) between frames.
void rainbow(int wait) {
  // Hue of first pixel runs 5 complete loops through the color wheel.
  for (long firstPixelHue = 0; firstPixelHue < 5 * 65536; firstPixelHue += 256) {
    ring.rainbow(firstPixelHue);
    ring.show();  // Update ring with new contents
    delay(wait);  // Pause for a moment
  }
}

//Read new tag if available
boolean getID() {
  // Getting ready for Reading PICCs
  if (!mfrc522.PICC_IsNewCardPresent()) {  //If a new PICC placed to RFID reader continue
    return false;
  }
  if (!mfrc522.PICC_ReadCardSerial()) {  //Since a PICC placed get Serial and continue
    return false;
  }
  tagID = "";
  for (uint8_t i = 0; i < 4; i++) {  // The MIFARE PICCs that we use have 4 byte UID
    //readCard[i] = mfrc522.uid.uidByte[i];
    tagID.concat(String(mfrc522.uid.uidByte[i], HEX));  // Adds the 4 bytes in a single String variable
  }
  tagID.toUpperCase();
  mfrc522.PICC_HaltA();  // Stop reading
  return true;
}