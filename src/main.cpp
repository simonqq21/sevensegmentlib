#include <Arduino.h>

const int numDigits = 4;
const int digitPins[numDigits] = {7,8,9,10};
const int dataPin = 13, shiftClkPin = 12, storeClkPin = 11;
int curDigit = 0;
int curDigitValue;
unsigned long lastTimeDigitUpdated = 0, lastTimeIncremented;
const int digits[10] = {
  //dp abcdefg
  0xFC, // 1111 1100 = 0
  0x60, // 0110 0000 = 1
  0xDA, // 1101 1010 = 2
  0xF2, // 1111 0010 = 3
  0x66, // 0110 0110 = 4
  0xB6, // 1011 0110 = 5
  0xBE, // 1011 1110 = 6
  0xE0, // 1110 0000 = 7
  0xFE, // 1111 1110 = 8
  0xF6, // 1111 0110 = 9
};
int displayDigits[] = {1,2,3,4};
int ctr;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  Serial.println("abcdef");
  digitalWrite(LED_BUILTIN, HIGH);
  for (int i=0;i<numDigits;i++) {
    pinMode(digitPins[i], OUTPUT);
  }
  pinMode(shiftClkPin, OUTPUT);
  pinMode(storeClkPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  ctr = 0;
}

void loop() {
  /**
   * 1234
   * 0th digit, thousands digit - 1234 / 1000 % 10 = 1
   * 1st digit, hundreds digit - 1234 / 100 % 10 = 2
   * 2nd digit, tens digit - 1234 / 10 % 10 = 3
   * 3rd digit, ones digit - 1234 / 1 % 10 = 4
   * 
   * generalization: 
   * n / 10^(numDigits-i-1) % 10
   */
  if (micros() - lastTimeDigitUpdated > 2500) {
    lastTimeDigitUpdated = micros();
    for (int i=0;i<numDigits;i++) {
      digitalWrite(digitPins[i], LOW);
    }
    digitalWrite(storeClkPin, LOW);
    shiftOut(dataPin, shiftClkPin, LSBFIRST, ~digits[displayDigits[curDigit]]);
    digitalWrite(storeClkPin, HIGH);
    digitalWrite(digitPins[curDigit], HIGH);
    curDigit++;
    if (curDigit >= numDigits) curDigit = 0;
  }

  if (millis() - lastTimeIncremented > 1000) {
    lastTimeIncremented = millis();
    Serial.print("ctr=");
    Serial.println(ctr);
    for (int i=0;i<numDigits;i++) {
      displayDigits[i] = ctr / (int) pow(10,(numDigits-i-1)) % 10; 
      Serial.print(displayDigits[i]);
      Serial.print(',');
    }
    Serial.println();
    ctr++;
  }
}
