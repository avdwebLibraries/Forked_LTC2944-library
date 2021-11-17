#ifndef LTC2944_1024_H
#define LTC2944_1024_H

// Only prescalerM = 1024 is implemented

// https://github.com/PonyPC/LTC2944-Arduino-Library

#include "Arduino.h"
#include <SBWire.h> // because the standard lib <Wire.h> hangs if no connection

#define LTC2944_1024_ADDRESS			0x64

/* Register Map */
#define REG_A_STATUS			  0x00 // Status (R) not used
#define REG_B_CONTROL			  0x01 // Control (R/W)
#define REG_C_ACC_CHG_MSB		0x02 // Accumulated Charge MSB (R/W)
#define REG_D_ACC_CHG_LSB		0x03 // Accumulated Charge LSB (R/W)
#define REG_I_VOLTAGE_MSB		0x08 // Voltage MSB (R)
#define REG_J_VOLTAGE_LSB		0x09 // Voltage LSB (R)
#define REG_O_CURRENT_MSB   0x0E // Current MSB (R)
#define REG_P_CURRENT_LSB   0x0F // Current LSB (R)
#define REG_U_TEMP_MSB			0x14 // Temperature MSB (R)
#define REG_V_TEMP_LSB			0x15 // Temperature LSB (R)

#define ADC_MODE_AUTO       0b11000000
#define PRESCALER_1024      0b00101000 // not implemented yet: has to change also with prescalerM
#define ALCC_ALERT_MODE     0b00000100 // pin becomes logic output.
#define ALCC_DISABLED       0b00000000 
#define POWERDOWN_DISABLED  0b00000000 

const byte controlRegisterB = ADC_MODE_AUTO|PRESCALER_1024|ALCC_ALERT_MODE|POWERDOWN_DISABLED; // because ALC was open, not allowed

class LTC2944_1024 {
public:
  LTC2944_1024::LTC2944_1024(float rSense);
  //LTC2944_1024::LTC2944_1024(uint16_t prescalerM, float rSense); // only prescalerM = 1024 is implemented now
	byte init(TwoWire &wirePort = Wire);
  byte set_mAh(long mAh);
  byte get_mAh(long &mAh);
  byte getVoltage(float &v);
  byte getCelsius(float &t);
  byte getCurrent(float &i);
    
private:
  byte writeWordToRegisters(byte msbAddress, uint16_t value);
  byte readWordFromRegisters(byte msbAddress, uint16_t &value);
  byte readByteFromRegister(byte address, byte &value); 
  byte writeByteToRegister(byte address, byte value);

  float rSense=0;
	TwoWire *i2cPort;
};

#endif
