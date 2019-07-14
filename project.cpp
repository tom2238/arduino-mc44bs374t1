#include <Arduino.h>
#include <Wire.h>
#include "MC44BS374T1.h"

void setup() {
 /* Serial.begin(9600);
  Serial.println("RF Start...");*/
  Wire.begin();
}

void loop() {
  //Serial.println("Line>>");

  MC44BS374T1::RFModulator rc(0x65);
  rc.SetFrequency(96000); // 96 MHz
  rc.SetPictureSoundRatio(MC44BS374T1_PS_12);
  rc.SetSoundSubcarrier(MC44BS374T1_SFD_65); // Audio 89.5, 102.5 MHz
  delay(1000);
}
