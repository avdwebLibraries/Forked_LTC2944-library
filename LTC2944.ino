#include "LTC2944.h" 

// Only prescalerM = 1024 is implemented now

/*(LTC2944_1024::LTC2944_1024(uint16_t prescalerM, float rSense): // not implemented yet: has to change also with prescalerM
prescalerM(prescalerM), rSense(rSense)
{
}*/

LTC2944_1024::LTC2944_1024(float rSense): 
rSense(rSense)
{
}

byte LTC2944_1024::init(TwoWire &wirePort) 
{	i2cPort = &wirePort; 
  return writeByteToRegister(REG_B_CONTROL, controlRegisterB);
}

byte LTC2944_1024::set_mAh(long mAh)
{ uint16_t value = (mAh / f) + 32767; // LTC2944 starts with mid-scale 32767
  return writeWordToRegisters(REG_C_ACC_CHG_MSB, value);
}

byte LTC2944_1024::get_mAh(long &mAh)
{ uint16_t value=0;
  byte returnValue = readWordFromRegisters(REG_C_ACC_CHG_MSB, value); // LTC2944 starts with mid-scale 32767
  mAh = ((long)value - 32767) * f; 
  return returnValue;      
}

byte LTC2944_1024::getVoltage(float &v) 
{	uint16_t value=0; // must be unsigned
  byte returnValue = readWordFromRegisters(REG_I_VOLTAGE_MSB, value);
  v = 70.8 * ((float) value / 0xFFFF);
  return returnValue;      
}

byte LTC2944_1024::getCelsius(float &t) 
{ uint16_t value=0; // must be unsigned
  byte returnValue = readWordFromRegisters(REG_U_TEMP_MSB, value);
  t = 510 * ((float) value / 0xFFFF) - 273.15; 
  return returnValue; 
}

byte LTC2944_1024::getCurrent(float &i)
{ uint16_t value=0;
  byte returnValue = readWordFromRegisters(REG_O_CURRENT_MSB, value); 
  const float f = 0.064 / (rSense * 32767);
  i = ((int)value - 32767) * f; // must be int
  return returnValue;
}

//------------------------------------------------------------------------------------------------

byte LTC2944_1024::writeWordToRegisters(byte msbAddress, uint16_t value)
{ i2cPort->beginTransmission(LTC2944_1024_ADDRESS);
  i2cPort->write(msbAddress); 
  i2cPort->write((uint8_t) (value >> 8));
  i2cPort->write((uint8_t) value);
  return i2cPort->endTransmission(false); // returns the status of the transmission
}

byte LTC2944_1024::readWordFromRegisters(byte msbAddress, uint16_t &value)
{ byte msb = 0;
  byte lsb = 0;
  if(!i2cPort) return 4; // else jumps to start when init was not done
  i2cPort->beginTransmission(LTC2944_1024_ADDRESS);
  i2cPort->write(msbAddress);  
  i2cPort->endTransmission(false); // If false, endTransmission() sends a restart message after transmission. The bus will not be released

  i2cPort->requestFrom(LTC2944_1024_ADDRESS, 2); // returns 1
  msb = i2cPort->read(); // must start with msb
  lsb = i2cPort->read();
  value = (msb << 8) | lsb;
  return i2cPort->endTransmission(false); // returns the status of the transmission
}

byte LTC2944_1024::readByteFromRegister(byte address, byte &value) 
{	i2cPort->beginTransmission(LTC2944_1024_ADDRESS);
	i2cPort->write(address);
	i2cPort->endTransmission(false);

	i2cPort->requestFrom(LTC2944_1024_ADDRESS, 1);
	value = i2cPort->read();
	return i2cPort->endTransmission(false);
}

byte LTC2944_1024::writeByteToRegister(byte address, byte value)
{	i2cPort->beginTransmission(LTC2944_1024_ADDRESS);
	i2cPort->write(address);
	i2cPort->write(value);
	return i2cPort->endTransmission(false);
}
