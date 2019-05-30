#include <Wire.h>

#ifdef ARDUINO_SAMD_VARIANT_COMPLIANCE
  #define SERIAL SerialUSB
#else
  #define SERIAL Serial
#endif

unsigned char low_data = 0;
unsigned char high_data[2] = {0};


#define NO_TOUCH       0xFE

#define ATTINY1_HIGH_ADDR   0x78
#define ATTINY2_LOW_ADDR   0x77


uint32_t touch_val = 0;

void getHigh12SectionValue(void)
{
	Wire.requestFrom(ATTINY1_HIGH_ADDR, 2);    
    while (Wire.available()) { 
        high_data[0] = Wire.read(); 
		high_data[1] = Wire.read();
	}
	delay(10);
}


void getLow8SectionValue(void)
{
	Wire.requestFrom(ATTINY2_LOW_ADDR, 1);    
    if (Wire.available()) { // slave may send less than requested
        low_data = Wire.read(); // receive a byte as character
    }
	delay(10);
}


void setup() {
    SERIAL.begin(115200);
    Wire.begin();
} 

void loop() {
    uint8_t trig_section = 0;
	
    getHigh12SectionValue();
    getLow8SectionValue();
	touch_val = 0;
	touch_val |= low_data&0xff;
	touch_val |= ((uint32_t)high_data[1]&0xff) << 8;
	touch_val |= ((uint32_t)high_data[0]&0xff) << 16;
	
	//SERIAL.println(low_data,HEX);
	//SERIAL.println(high_data[0],HEX);
	//SERIAL.println(high_data[1],HEX);
	//SERIAL.println(touch_val,HEX);
	
	while(touch_val & 0x01)
	{
		trig_section++;
		touch_val >>= 1;
	}
	SERIAL.print("The water level = ");
	SERIAL.print(trig_section*5);
	SERIAL.println("% ");
	
    delay(1000);
}
