#include "93C46.h"
/*
 * Example Sketch demonstration on how to read from a 93C46 EEPROM
 * 
 * Wiring:
 * Pin pCS to Chip pin 1
 * Pin pSK to Chip pin 2
 * Pin pDI to Chip pin 3
 * Pin pDO to Chip pin 4
 * 
 * (For some chips:) GND/VCC to Chip pin 6
 * This determines the organization:
 * HIGH is 64x16
 * LOW is 128x8
 * 
 */
#define pCS 21
#define pSK 22
#define pDI 23
#define pDO 32

char newOdometer[] = "00000"; // skip the ones digit
bool doWrite = false;

// Prints all words of the buffer
void debugPrint(word* buff, int len) {
  Serial.print("\n\t00\t01\t02\t03\t04\t05\t06\t07\t08\t09\t0A\t0B\t0C\t0D\t0E\t0F");
  for(int i = 0; i < len; i++) {
    if(i % 16 == 0) {
      Serial.println();
      Serial.print(i, HEX);
    }
    Serial.print("\t");
    if(buff[i] < 0x10) {
      Serial.print("0");
    }
    Serial.print(buff[i], HEX);
  }
}

char invertHex(char h)
{
  return 0xf - h;
}

void setup() {
  bool longMode = EEPROM_93C46_MODE_16BIT;
  
  eeprom_93C46 e = eeprom_93C46(pCS, pSK, pDI, pDO);
  e.set_mode(longMode);
  Serial.begin(115200);
  delay(1000);
  
  int len = longMode ? 64 : 128;
  word readBuffer[len];
  for(int i = 0; i < len; i++) {
    // Read by address
    word r = e.read(i);
    readBuffer[i] = r;
    Serial.print(char(r));
  }
  debugPrint(readBuffer, len);
  Serial.printf("\n\nRead Done\n\nCurrent odometer reading:\n");


  // read out the current mileage (it's stored 3x, read each)
  Serial.printf("As hex:\n%1x %1x %1x %1x %1x\n", readBuffer[5] >> 4 & 0x000f, readBuffer[5] & 0x000f, readBuffer[4] >> 12 & 0x000f,
                              readBuffer[4] >> 8 & 0x000f, readBuffer[4] >> 4 & 0x000f);
                              

  // edit the mileage (3x). The ones digit is unknown
  // invert the hex for the digits
  Serial.printf("Invert hex for digits:\n");
  for (int i = 0; i < sizeof(newOdometer) - 1; i++)
  {
    Serial.printf("(%c)", newOdometer[i]);
    newOdometer[i] = invertHex(newOdometer[i] - 48);
    Serial.printf("%1x ", newOdometer[i]);
  }

  // this gets written 3x
  for (int i = 0; i <3; i++)
  {
    readBuffer[5 + (i * 2)] = (readBuffer[5] & 0xff00) | (newOdometer[0] << 4) | newOdometer[1];
    readBuffer[4 + (i * 2)] = (readBuffer[4] & 0x000f) | (newOdometer[2] << 12) | (newOdometer[3] << 8) | (newOdometer[4] << 4);
  }
  
  // print out the new dump for testing
  Serial.printf("\nNew value:\n%1x%1x%1x%1x%1x\n\n", invertHex(readBuffer[5] >> 4 & 0x000f), invertHex(readBuffer[5] & 0x000f), invertHex(readBuffer[4] >> 12 & 0x000f),
                              invertHex(readBuffer[4] >> 8 & 0x000f), invertHex(readBuffer[4] >> 4 & 0x000f));

  debugPrint(readBuffer, len);


  Serial.printf("\n\nWriting data in 5 seconds!!!!\n");
  delay(4000);

  if (doWrite)
    e.ew_enable();
  else
    Serial.printf("Write NOT enabled! Just going through the motions...");
  delay(1000);
  
  for (int i = 0; i < len; i++)
  {
    if (doWrite)
      e.write(i, readBuffer[i]);
    Serial.printf(".");
    delay(50);
  }

  e.ew_disable();
  delay(1000);
  
  Serial.printf("Write done. Reading back:\n");
    for(int i = 0; i < len; i++) {
    word r = e.read(i);
    readBuffer[i] = r;
    Serial.print(char(r));
  }
  debugPrint(readBuffer, len);
  Serial.println();
}

void loop() {}
