#include <Wire.h>

constexpr uint32_t kScanIntervalMs = 3000;
constexpr uint8_t kFirstUsable = 0x03;
constexpr uint8_t kLastUsable = 0x77;

void scanBus();
bool probeAddress(uint8_t address);

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 5000) {
    delay(10);
  }

  Wire.setSDA(12);
  Wire.setSCL(13);

  Wire.begin();

  Serial.println();
  Serial.println(F("RP2040 I2C scanner (i2cdetect style)"));
  Serial.println(F("Scanning every 3 seconds..."));
  scanBus();
}

void loop() {
  static uint32_t lastScan = 0;
  if (millis() - lastScan >= kScanIntervalMs) {
    lastScan = millis();
    scanBus();
  }
}

void scanBus() {
  uint8_t found[0x78];
  uint8_t foundCount = 0;

  Serial.println();
  Serial.println(F("     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f"));

  for (uint8_t row = 0; row < 8; ++row) {
    uint8_t base = row << 4;

    char prefix[5];
    snprintf(prefix, sizeof(prefix), "%02x:", base);
    Serial.print(prefix);

    for (uint8_t col = 0; col < 16; ++col) {
      uint8_t address = base + col;

      if (address < kFirstUsable || address > kLastUsable) {
        Serial.print(F(" --"));
        continue;
      }

      if (probeAddress(address)) {
        char cell[4];
        snprintf(cell, sizeof(cell), " %02x", address);
        Serial.print(cell);
        if (foundCount < sizeof(found)) {
          found[foundCount++] = address;
        }
      } else {
        Serial.print(F(" --"));
      }
      delayMicroseconds(50);
    }
    Serial.println();
  }

  if (!foundCount) {
    Serial.println(F("No I2C devices found."));
    return;
  }

  Serial.print(F("Found "));
  Serial.print(foundCount);
  Serial.print(F(" device"));
  if (foundCount > 1) {
    Serial.print('s');
  }
  Serial.print(F(" at:"));

  for (uint8_t i = 0; i < foundCount; ++i) {
    char addr[7];
    snprintf(addr, sizeof(addr), " 0x%02x", found[i]);
    Serial.print(addr);
  }
  Serial.println();
}

bool probeAddress(uint8_t address) {
  Wire.beginTransmission(address);
  uint8_t status = Wire.endTransmission();
  return status == 0;
}
