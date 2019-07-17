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
    uint8_t output = 0x80;
    bitWrite(output,5,so);
    bitWrite(output,4,lop);
    bitWrite(output,3,ps);
    bitWrite(output,2,(wm2 & 0x01));
    bitWrite(output,1,(wm1 & 0x04) >> 2);
    bitWrite(output,0,0);
    return output;
  }

  uint8_t RFModulator::RegisterC0(uint8_t pwc, uint8_t osc, uint8_t att, uint8_t sfd, uint8_t wm2) {
    uint8_t output = 0x00;
    bitWrite(output,7,pwc);
    bitWrite(output,6,osc);
    bitWrite(output,5,att);
    bitWrite(output,4,(sfd & 0x02) >> 1);
    bitWrite(output,3,(sfd & 0x01));
    bitWrite(output,2,0);
    bitWrite(output,1,(wm2 & 0x04) >> 2);
    bitWrite(output,0,(wm2 & 0x02) >> 1);
    return output;
  }

  uint8_t RFModulator::RegisterFM(uint8_t tpen, uint16_t divider) {
    uint8_t output = 0x00;
    bitWrite(output,6,tpen);
    bitWrite(output,5,(divider & 0x800) >> 11);
    bitWrite(output,4,(divider & 0x400) >> 10);
    bitWrite(output,3,(divider & 0x200) >> 9);
    bitWrite(output,2,(divider & 0x100) >> 8);
    bitWrite(output,1,(divider & 0x080) >> 7);
    bitWrite(output,0,(divider & 0x040) >> 6);
    return output;
  }

  uint8_t RFModulator::RegisterFL(uint16_t divider, uint8_t wm1) {
    uint8_t output = 0x00;
    bitWrite(output,7,(divider & 0x020) >> 5);
    bitWrite(output,6,(divider & 0x010) >> 4);
    bitWrite(output,5,(divider & 0x008) >> 3);
    bitWrite(output,4,(divider & 0x004) >> 2);
    bitWrite(output,3,(divider & 0x002) >> 1);
    bitWrite(output,2,(divider & 0x001));
    bitWrite(output,1,(wm1 & 0x02) >> 1);
    bitWrite(output,0,(wm1 & 0x01));
    return output;
  }


  RFModulator::RFModulator(uint8_t address) {
    iic_address = address;
    rf_test = MC44BS374T1_WM2_NORMAL;
    rf_divider = MC44BS374T1_WM1_NORMAL;
    rf_so = MC44BS374T1_SO_ON;
    rf_lop = MC44BS374T1_LOP_HIGH;
    rf_ps = MC44BS374T1_PS_12;
    rf_pwc = MC44BS374T1_PWC_ON;
    rf_osc = MC44BS374T1_OSC_NORMAL;
    rf_att = MC44BS374T1_ATT_NORMAL;
    rf_sfd = MC44BS374T1_SFD_65;
    rf_tpen = MC44BS374T1_TPEN_OFF;
  }

  void RFModulator::SetFrequency(uint32_t freq) {
      uint8_t rfdivider = 1;
      if((freq <= MC44BS374T1_UHF_MAX) && (freq > MC44BS374T1_UHF_MIN)) {
        rfdivider = 1;
        rf_divider = MC44BS374T1_WM1_NORMAL;
      }
      else if((freq <= MC44BS374T1_VHF_MAX) && (freq > 230000)) {
        rfdivider = 2;
        rf_divider = MC44BS374T1_WM1_RF2;
      }
      else if((freq <= 230000) && (freq > 115000)) {
        rfdivider = 4;
        rf_divider = MC44BS374T1_WM1_RF4;
      }
      else if((freq <= 115000) && (freq > 57500)) {
        rfdivider = 8;
        rf_divider = MC44BS374T1_WM1_RF8;
      }
      else if((freq <= 57500) && (freq > MC44BS374T1_VHF_MIN)) {
        rfdivider = 16;
        rf_divider = MC44BS374T1_WM1_RF16;
      }
      else {
        rfdivider = 1;
        rf_divider = MC44BS374T1_WM1_NORMAL;
        freq = 871250;
      }
     /* Serial.print("FS:");
      Serial.print(freq);
      Serial.print(":DV:");
      Serial.print(rfdivider);
      Serial.print(":RV:");*/
      uint32_t freqdiv = freq / 10;
      freqdiv *= 4 * rfdivider;
      rf_value = freqdiv / 100;
      // Serial.println(rf_value);
      SendRegister();
  }

  void RFModulator::SetPictureSoundRatio(uint8_t val) {
    rf_ps = val;
    SendRegister();
  }

  void RFModulator::SetSoundSubcarrier(uint8_t val) {
    rf_sfd = val;
    SendRegister();
  }

  void RFModulator::SetChannel(uint8_t channel) { // CCIR D/K
    if((channel <= 71) && (channel >= 21)) {
      SetFrequency(MC44BS374T1_CH_4 + (channel - 21) * 8); // 8 MHz step
    }
    else if((channel <= 12) && (channel >= 6)) {
      SetFrequency(MC44BS374T1_CH_3 + (channel - 6) * 8); // 8 MHz step
    }
    else if((channel <= 5) && (channel >= 3)) {
      SetFrequency(MC44BS374T1_CH_2 + (channel - 3) * 8); // 8 MHz step
    }
    else if((channel <= 2) && (channel >= 1)) {
      SetFrequency(MC44BS374T1_CH_1 + (channel - 1) * 9.5); // 9.5 MHz step
    }
  }

  void RFModulator::SendRegister() {
    uint8_t c0 = RegisterC0(rf_pwc, rf_osc, rf_att, rf_sfd, rf_test);
    uint8_t c1 = RegisterC1(rf_so, rf_lop, rf_ps, rf_test, rf_divider);
    uint8_t fm = RegisterFM(rf_tpen, rf_value);
    uint8_t fl = RegisterFL(rf_value, rf_divider);
    /*Serial.print("SR:");
    Serial.print(c1, HEX);
    Serial.print(":");
    Serial.print(c0, HEX);
    Serial.print(":");
    Serial.print(fm, HEX);
    Serial.print(":");
    Serial.println(fl, HEX);*/
    SendDataRaw(c1,c0,fm,fl);
  }
}

