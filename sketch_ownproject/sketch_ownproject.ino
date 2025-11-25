#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9
#define SS_PIN 10

MFRC522 rfid(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(115200);
  SPI.begin();
  rfid.PCD_Init();
  delay(50);

  byte v = rfid.PCD_ReadRegister(MFRC522::VersionReg);
  Serial.print("Firmware Version: 0x");
  if (v < 0x10) Serial.print("0");
  Serial.println(v, HEX);

  Serial.println("Ready — håll ett kort mot läsaren...");
}

void loop() {
  // vänta på nytt kort
  if (!rfid.PICC_IsNewCardPresent()) return;
  if (!rfid.PICC_ReadCardSerial()) return;

  Serial.print("UID: ");
  for (byte i = 0; i < rfid.uid.size; i++) {
    if (rfid.uid.uidByte[i] < 0x10) Serial.print("0");
    Serial.print(rfid.uid.uidByte[i], HEX);
    if (i < rfid.uid.size - 1) Serial.print(":");
  }
  Serial.println();

  // Visa typ av kort
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  Serial.print("Type: ");
  Serial.println(rfid.PICC_GetTypeName(piccType));

  rfid.PICC_HaltA();        // stoppa kommunikationen med kortet
  rfid.PCD_StopCrypto1();   // stäng kryptering om den var igång
  delay(500);
}
