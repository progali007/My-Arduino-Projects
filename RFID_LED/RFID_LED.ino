#include "SPI.h"
#include "MFRC522.h"

#define SS_PIN 10
#define RST_PIN 9
#define SP_PIN 8

MFRC522 rfid(SS_PIN, RST_PIN);

MFRC522::MIFARE_Key key;

int red = 2;
int yellow = 3;

void setup() {
  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);
  Serial.begin(9600);
  SPI.begin();
  delay(4);
  rfid.PCD_Init();
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
    return;

  // Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  // Serial.println(rfid.PICC_GetTypeName(piccType));

  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }

  String strID = "";
  for (byte i = 0; i < 4; i++) {
    strID +=
    (rfid.uid.uidByte[i] < 0x10 ? "0" : "") +
    String(rfid.uid.uidByte[i], HEX) +
    (i!=3 ? ":" : "");
  }
  strID.toUpperCase();

  // เมื่อถึงส่วนนี้ ตัวแปร strID จะเก็บค่า UID ของแท็กไว้แล้ว
  // สามารถนำไปใช้งานได้เลย เช่น นำไปเข้า IF เพื่อให้หลอด
  // LED ติดสว่าง หรือดับได้
  Serial.print("Tap card key: ");
  Serial.println(strID);

  if(strID.indexOf("A9:AB:A3:29")>=0){
    digitalWrite(red,HIGH);
    digitalWrite(yellow,LOW);
  }else if(strID.indexOf("4B:7F:A1:0D")>=0){
    digitalWrite(yellow,HIGH);
    digitalWrite(red,LOW);
  }

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}
