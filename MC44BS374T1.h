#ifndef MC44BS374T1_H
#define MC44BS374T1_H

#include <Arduino.h>

#define MC44BS374T1_SO_ON      0x00  // Sound Oscillator on
#define MC44BS374T1_SO_OFF     0x01  // Sound Oscillator off
#define MC44BS374T1_LOP_LOW    0x00  // Logic Output Port low voltage
#define MC44BS374T1_LOP_HIGH   0x01  // Logic Output Port high impedance
#define MC44BS374T1_PS_12      0x00  // Picture-to-Sound Ratio 12 dB
#define MC44BS374T1_PS_16      0x01  // Picture-to-Sound Ratio 16 dB
#define MC44BS374T1_PWC_ON     0x00  // Peak White Clip System B/G
#define MC44BS374T1_PWC_OFF    0x01  // Peak White Clip System L
#define MC44BS374T1_OSC_NORMAL 0x01  // UHF Oscillator Normal operation
#define MC44BS374T1_OSC_DISABLE 0x00 // UHF Oscillator disabled
#define MC44BS374T1_ATT_NORMAL 0x00  // Modulator Output Attenuation Normal operation
#define MC44BS374T1_ATT_ATT    0x01  // Modulator Output Attenuation Video+Sound modulator OFF
#define MC44BS374T1_SFD_45     0x00  // Sound Subcarrier Freq 4.5 MHz
#define MC44BS374T1_SFD_55     0x01  // Sound Subcarrier Freq 5.5 MHz
#define MC44BS374T1_SFD_60     0x02  // Sound Subcarrier Freq 6.0 MHz
#define MC44BS374T1_SFD_65     0x03  // Sound Subcarrier Freq 6.5 MHz
#define MC44BS374T1_TPEN_OFF   0x00  // Test Pattern Signal OFF
#define MC44BS374T1_TPEN_ON    0x01  // Test Pattern Signal ON
#define MC44BS374T1_UHF_MAX    880000 // Max UHF oscillator freq
#define MC44BS374T1_UHF_MIN    460000 // Min UHF oscillator freq
#define MC44BS374T1_VHF_MAX    460000 // Max VHF oscillator freq
#define MC44BS374T1_VHF_MIN    45000  // Min VHF oscillator freq
#define MC44BS374T1_CH_1       49750  // I - VHF, freq kHz, CH 1
#define MC44BS374T1_CH_2       77250  // II - VHF, freq kHz, CH 3
#define MC44BS374T1_CH_3       175250 // III - VHF, freq kHz, CH 6
#define MC44BS374T1_CH_4       471250 // IV+V - UHF, freq kHz, CH 21


enum MC44BS374T1_WMODE_1 {
  MC44BS374T1_WM1_NORMAL,   // Normal operation
  MC44BS374T1_WM1_RF2,      // RF frequency divided for low frequency testing or VHF range: RF/2
  MC44BS374T1_WM1_RF4,      // RF/4
  MC44BS374T1_WM1_RF8,      // RF/8
  MC44BS374T1_WM1_RF16,     // RF/16
  MC44BS374T1_WM1_DC_NR_VID,// DC drive applied to modulators: Non-inverted video at TVOUT
  MC44BS374T1_WM1_DC_IN_VID,// DC drive applied to modulators: Inverted video at TVOUT
  MC44BS374T1_WM1_SPEEDUP   // Transient output inhibit disabled (ie speed up mode)
};

enum MC44BS374T1_WMODE_2 {
  MC44BS374T1_WM2_NORMAL,     // Normal operation
  MC44BS374T1_WM2_PATTERN,    // Test pattern generator DC verification
  MC44BS374T1_WM2_PRG_DIVIDER,// Program divider test
  MC44BS374T1_WM2_REF_DIVIDER,// Reference divider test
  MC44BS374T1_WM2_UHFPC_US,   // UHF phase comparison, upper source on PLLFILT pin
  MC44BS374T1_WM2_UHFPC_LS,   // UHF phase comparison, lower source on PLLFILT pin
  MC44BS374T1_WM2_SPC_US,     // Sound phase comparison 1 μA upper source on SPLLFIL
  MC44BS374T1_WM2_SPC_LS      // Sound phase comparison 1 μA lower source on SPLLFIL
};

namespace MC44BS374T1 {
  class RFModulator {
    private:
      uint8_t iic_address;    // 0x65 - default
      uint16_t rf_value;      // 12 bit divider 250 kHz step
      uint8_t rf_divider;     // WM1 - for VHF range
      uint8_t rf_test;        // WM2 - test modes
      uint8_t rf_so;          // Sound oscillator
      uint8_t rf_lop;         // Logic output pin
      uint8_t rf_ps;          // Picture to sound ratio
      uint8_t rf_pwc;         // Peak white clip
      uint8_t rf_osc;         // UHF oscillator
      uint8_t rf_att;         // Attenuation
      uint8_t rf_sfd;         // Sound subcarrier
      uint8_t rf_tpen;        // Test pattern
      void SendDataRaw(uint8_t c1, uint8_t c0, uint8_t fm, uint8_t fl);
      uint8_t RegisterC1(uint8_t so, uint8_t lop, uint8_t ps, uint8_t wm2, uint8_t wm1);
      uint8_t RegisterC0(uint8_t pwc, uint8_t osc, uint8_t att, uint8_t sfd, uint8_t wm2);
      uint8_t RegisterFM(uint8_t tpen, uint16_t divider);
      uint8_t RegisterFL(uint16_t divider, uint8_t wm1);
      void SendRegister();
    public:
      // Set IIC address of modulator
      RFModulator(uint8_t address);
      void SetFrequency(uint32_t freq);
      void SetPictureSoundRatio(uint8_t val);
      void SetSoundSubcarrier(uint8_t val);
      void SetChannel(uint8_t channel);      // CCIR D/K, East Europe
      void SetSoundOscillator(uint8_t state);
      void SetLogicOutputPort(uint8_t state);
      void SetPeakWhiteClip(uint8_t state);
      void SetOscillator(uint8_t state);
      void SetAttenuation(uint8_t state);
      void SetTestPattern(uint8_t state);
      void SetTestMode(uint8_t state);
      uint8_t GetStatus();
  };
}

#endif // MC44BS374T1_H
