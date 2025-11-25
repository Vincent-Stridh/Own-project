#include <SPI.h>
#include <MFRC522.h>

int led1 = 6;
int led2 = 7;

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
}

void loop() {
  Serial.begin(9600);
  //Wait until new tag is available
  while (getID()) {
    if (tagID == MasterTag) {
      
      Serial.print(tagID + " Scanning.. ");
      delay(200);
      Serial.println("Access Granted!");
      digitalWrite(led1, HIGH);
      digitalWrite(led2, LOW);
      // You can write any code here like opening doors, switching on a relay, lighting up an LED, or anything else you can think of.
    } else {
      digitalWrite(led2, HIGH);
      digitalWrite(led1, LOW);
      Serial.print(tagID + " Scanning.. ");
      delay(200);
      Serial.println("Access Denied!");
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