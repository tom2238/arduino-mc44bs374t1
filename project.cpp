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
  rc.SetFrequency(495250); // 495.25 MHz - UHF CH 24
  //rc.SetChannel(24);
  rc.SetPictureSoundRatio(MC44BS374T1_PS_12);
  rc.SetSoundSubcarrier(MC44BS374T1_SFD_65); // Audio 488.75, 501.75 MHz
  delay(5000);
}
