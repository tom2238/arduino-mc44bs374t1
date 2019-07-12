#include <Arduino.h>
#include <Wire.h>
#include "MC44BS374T1.h"

void setup() {

}

void loop() {
  MC44BS374T1::RFModulator rc(0x65);

  delay(500);
}
