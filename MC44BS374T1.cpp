// To run on the board use: make raw_upload
#include "MC44BS374T1.h"
#include <Wire.h>

namespace MC44BS374T1 {

  void RFModulator::SendDataRaw(uint8_t c1, uint8_t c0, uint8_t fm, uint8_t fl) {
    Wire.beginTransmission(iic_address);
    Wire.write(c1);
    Wire.write(c0);
    Wire.write(fm);
    Wire.write(fl);
    Wire.endTransmission();
  }

  uint8_t RFModulator::RegisterC1(uint8_t so, uint8_t lop, uint8_t ps, uint8_t wm2, uint8_t wm1) {

  }

  uint8_t RFModulator::RegisterC0(uint8_t pwc, uint8_t osc, uint8_t att, uint8_t sfd, uint8_t wm2) {

  }

  uint8_t RFModulator::RegisterFM(uint8_t tpen, uint16_t divider) {

  }

  uint8_t RFModulator::RegisterFL(uint16_t divider, uint8_t wm1) {

  }


  RFModulator::RFModulator(uint8_t address) {
    Wire.begin();
    iic_address = address;
  }

}

