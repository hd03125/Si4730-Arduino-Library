#ifndef Si4730_h
#define Si4730_h
 
#include "Arduino.h"
#include <Wire.h>

#define SI4730_ADDR 0x63
#define SI4730_POWER_UP 0x01
#define SI4730_POWER_DOWN 0x11
#define SI4730_SET_PROPERTY 0x12  // SI4730 변수 값 변경
#define SI4730_ANALOGOUT 0x05
#define SI4730_SET_FM_FREQ 0x20
#define SI4730_SET_AM_FREQ 0x40
#define SI4730_SEEK_FM 0x21
#define SI4730_SEEK_AM 0x41
#define SI4730_SEEK_UP 0x0C
#define SI4730_SEEK_DOWN 0x04
#define SI4730_MODE_FM 0x50
#define SI4730_MODE_AM 0x51 // 0x01 이라는 데이터시트 내용도 있음. 참고

// AM 채널 필터 값 
#define SI4730_AM_FILTER_6 0x00
#define SI4730_AM_FILTER_4 0x01
#define SI4730_AM_FILTER_3 0x02
#define SI4730_AM_FILTER_2 0x03
#define SI4730_AM_FILTER_1 0x04
 
class Si4730
{
  	public:
    	Si4730(int rstpin);
    	void powerUp(bool mode);
    	void setFMFrequency(int freq);
    	void setAMFrequency(int freq);
    	void seekFM(bool updown);
    	void seekAM(bool updown);
    	void channelFilterAM(char filter);
    	void receiveResponse(void);
    	void GET_REV(void);
    	void FM_STATUS(void);
    	void powerDown(void);

        int frequency = 9870, i=6;
        char highbyte, lowbyte, filter=6;

  	private:
        int _pin;
};
 
#endif