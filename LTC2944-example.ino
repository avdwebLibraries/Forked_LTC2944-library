#include "LTC2944.h" 

//LTC2944
const float rSense = 0.002; 
const uint16_t prescalerM = 1024; // can't be changed because of PRESCALER_1024  0b00101000
const float f = 0.34*0.05*prescalerM/(rSense*4096); // extra line with const reduces sketch size

LTC2944_1024 ltc2944(rSense); // Only prescalerM = 1024 is implemented

long mAh, eeprom_mAh=0;
float volt, amp, temp;

void readLTC2944_1024(long &_mAh, float &_v, float &_i)
{ if(!ltc2944.get_mAh(mAh)) _mAh = mAh; // 710us, skip value if error had occur 

  float i=0;
  if(!ltc2944.getCurrent(i)) _i = i; // 710us, skip value if error had occur;  
  
  float v=0;
  if(!ltc2944.getVoltage(v)) _v = v; // 710us, skip value if error had occur
}
   
void setup() 
{ Wire.begin(); // default 100kbit/s 
  if(ltc2944.init()); // setError(LTC2944init, "LTC2944 init");
  ltc2944.set_mAh(eeprom_mAh); // init gauge_mAh with the EEPROM value
  ltc2944.get_mAh(mAh);
  ltc2944.getCurrent(amp);
  ltc2944.getVoltage(volt);
  ltc2944.getCelsius(temp);
}

void loop() 
{ 
}
