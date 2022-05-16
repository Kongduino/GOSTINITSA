#include "gost.h"

void setup() {
  Serial.begin(115200, RAK_CUSTOM_MODE);
  // RAK_CUSTOM_MODE disables AT firmware parsing
  time_t timeout = millis();
  while (!Serial) {
    // on nRF52840, Serial is not available right away.
    // make the MCU wait a little
    if ((millis() - timeout) < 5000) {
      delay(100);
    } else {
      break;
    }
  }
  uint8_t x = 5;
  while (x > 0) {
    Serial.printf("%d, ", x--);
    delay(500);
  } // Just for show
  Serial.println("0!");
  Serial.println("\nGOST Test");
  Serial.println("------------------------------------------------------");
  unsigned char IV[16];
  unsigned char block[16];
  unsigned char result[16];
  GOST_CTX ctx;
  unsigned char key[32] = {
    0x4B, 0x9E, 0xA4, 0xD1, 0x1A, 0x32, 0x17, 0xD2,
    0xF3, 0xB1, 0xBD, 0x9C, 0xE9, 0x89, 0xB4, 0x64,
    0x63, 0xAA, 0xE5, 0x9A, 0x26, 0x68, 0xB5, 0x18,
    0x4A, 0xC2, 0xBE, 0x79, 0x71, 0x24, 0x54, 0xAD
  };
  int i;
  /* Run test vectors */
  memset(IV, 1, 16);
  GOST_Init(&ctx, key, IV); /* use block = all 0 for IV */
  i = 0;
  block[i++] = 0x68;
  block[i++] = 0x53;
  block[i++] = 0xcd;
  block[i++] = 0x1d;
  block[i++] = 0x54;
  block[i++] = 0xc2;
  block[i++] = 0xd8;
  block[i++] = 0x7d;

  block[i++] = 0xBD;
  block[i++] = 0x26;
  block[i++] = 0x89;
  block[i++] = 0x17;
  block[i++] = 0x64;
  block[i++] = 0xAA;
  block[i++] = 0xE9;
  block[i++] = 0x4B;
  memcpy(result, block, 16);

  Serial.printf("Plain     ="); for (i = 0; i < 16; i++) Serial.printf(" %02x", block[i]); Serial.printf("\n");
  GOST_EncryptECB(&ctx, block);
  GOST_EncryptECB(&ctx, block + 8);
  Serial.printf("Encrypted ="); for (i = 0; i < 16; i++) Serial.printf(" %02x", block[i]); Serial.printf("\n");

  memset(IV, 1, 16);
  GOST_Init(&ctx, key, IV); /* use block = all 0 for IV */
  GOST_DecryptECB(&ctx, block);
  GOST_DecryptECB(&ctx, block + 8);
  bool match = true;
  Serial.printf("Decrypted =");
  for (i = 0; i < 16; i++) {
    if (block[i] != result[i]) match = false;
    Serial.printf(" %02x", block[i]);
  } Serial.printf("\n");
  Serial.println(match ? "Goodie gumdrops. That's a match!" : "Bzzert! Not a match!");
}

void loop() {
  // put your main code here, to run repeatedly:

}
