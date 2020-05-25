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
//  delayMicroseconds(1);
  digitalWrite((SH_REG->SH_REG_P[RCLK]) , HIGH);
//  delayMicroseconds(1);
}
unsigned char SN74HC595_SRCLK(SN74HC595 *SH_REG) {
  digitalWrite((SH_REG->SH_REG_P[SRCLK]) , LOW);
  delayMicroseconds(1);
  digitalWrite((SH_REG->SH_REG_P[SRCLK]) , HIGH);
  delayMicroseconds(1);
}
unsigned char SN74HC595_Write(SN74HC595 *SH_REG, unsigned char PIN_NUM, unsigned char STATE) {

  uint8_t SH_REG_UNIT = PIN_NUM / 8;
  uint8_t SH_REG_PIN  = PIN_NUM % 8;

  if (STATE == HIGH) {
    set_bit(SH_REG->OUT[SH_REG_UNIT], SH_REG_PIN);
  }
  else {
    clr_bit(SH_REG->OUT[SH_REG_UNIT], SH_REG_PIN);
  }

  for (unsigned char UNIT = 0; UNIT < SN74HC595_PCS_N; UNIT++) {
    for (int BIT = 7; BIT >= 0; BIT--) {
//      Serial.print(UNIT);
//      Serial.print(" : ");
//      Serial.print(BIT);
//      Serial.print(" : ");
//      Serial.println(get_bit(SH_REG->OUT[SN74HC595_PCS_N - UNIT - 1], BIT));
//      delay(10);
      digitalWrite((SH_REG->SH_REG_P[SER]), get_bit(SH_REG->OUT[SN74HC595_PCS_N - UNIT - 1], BIT));

//      delayMicroseconds(1);
      SN74HC595_SRCLK(SH_REG);
    }
  }
  SN74HC595_RCLK(SH_REG);
}
