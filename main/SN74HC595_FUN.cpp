#include "HAL/SN74HC595/SN74HC595_FUNs.h"
#include <Arduino.h>

unsigned char SN74HC595_INIT_PIN(SN74HC595 *SH_REG, unsigned char PIN_NAME, unsigned char PIN_NUM) {
  SH_REG->SH_REG_P[PIN_NAME] = PIN_NUM;
}
unsigned char SN74HC595_INIT(SN74HC595 *SH_REG) {

  pinMode((SH_REG->SH_REG_P[SER])        , OUTPUT);
  pinMode((SH_REG->SH_REG_P[RCLK])       , OUTPUT);
  pinMode((SH_REG->SH_REG_P[SRCLK])      , OUTPUT);

  digitalWrite((SH_REG->SH_REG_P[SER])   , LOW);
  digitalWrite((SH_REG->SH_REG_P[RCLK])  , LOW);
  digitalWrite((SH_REG->SH_REG_P[SRCLK]) , LOW);

  for (uint8 i = 0; i < SN74HC595_PCS_N; i++) {
    SH_REG->OUT[i] = 0;
  }
}
unsigned char SN74HC595_RCLK(SN74HC595 *SH_REG) {
  digitalWrite((SH_REG->SH_REG_P[RCLK]) , LOW);
  digitalWrite((SH_REG->SH_REG_P[RCLK]) , HIGH);
}
unsigned char SN74HC595_SRCLK(SN74HC595 *SH_REG) {
  digitalWrite((SH_REG->SH_REG_P[SRCLK]) , LOW);
  digitalWrite((SH_REG->SH_REG_P[SRCLK]) , HIGH);
}
unsigned char SN74HC595_Write(SN74HC595 *SH_REG, unsigned char PIN_NUM, unsigned char STATE) {
  
  uint8 SH_REG_UNIT = PIN_NUM / 8;
  uint8 SH_REG_PIN  = PIN_NUM % 8;

  if (STATE == HIGH) {
    set_bit(SH_REG->OUT[SH_REG_UNIT], SH_REG_PIN);
  }
  else {
    clr_bit(SH_REG->OUT[SH_REG_UNIT], SH_REG_PIN);
  }
  
  for (uint8 UNIT = 0; UNIT < SN74HC595_PCS_N; UNIT++) {
    for (uint8 BIT = 0; BIT < 8; BIT++) {
      digitalWrite((SH_REG->SH_REG_P[SER]), get_bit(SH_REG->OUT[SN74HC595_PCS_N - UNIT - 1], 7 - BIT));
      SN74HC595_SRCLK(SH_REG);
    }
  }
  SN74HC595_RCLK(SH_REG);
}
