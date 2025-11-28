/*
* Name: Own Project 
* Author: Vincent Stridh
* Date: 2025-12-2
* Description: This project uses a rc522 to read the rfid cards and and displays the time to an 1306 oled display, 
* Further, it measures temprature with ds3231 and displays a mapped value to a 9g-servo-motor.
*/



#include <SPI.h>
#include <MFRC522.h>
#include <VarSpeedServo.h>

VarSpeedServo Servo1;

int led1 = 6;
int led2 = 7;
int servoPin = 9;

#define RST_PIN 9
#define SS_PIN 10

byte readCard[4];
String MasterTag = "9394C1D";  // REPLACE this Tag ID with your Tag ID!!!
String tagID = "";

// Create instances
MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
  SPI.begin();         // SPI bus
  mfrc522.PCD_Init();  // MFRC522
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  Servo1.attach(servoPin);
  Servo1.write(0);
}

void loop() {
  Serial.begin(9600);
  
  //Wait until new tag is available
  while (getID()) {
    if (tagID == MasterTag) {
      
      Serial.println("Tag detected: '" + tagID + "'..");
      delay(600);
      Serial.println(" Scanning data.. ");
      delay(1300);
      Serial.println("Access Granted!");
      Servo1.write(90, 15, true);
    } else {
      Serial.println("Tag detected: '" + tagID + "'..");
      delay(600);
      Serial.println(" Scanning data.. ");
      delay(1500);
      Serial.println("Access Denied!");
      Servo1.write(0, 100, true);

    }
    delay(2000);
    digitalWrite(led2, LOW);
    digitalWrite(led1, LOW);
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