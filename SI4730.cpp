#include <Wire.h>
#include <Arduino.h>
#include <Si4730.h>


Si4730::Si4730(int rstpin)
{
	_pin = rstpin;
}

void Si4730::powerUp(bool mode) // Si4730 초기화, mode가 true 면 FM, false면 AM
{
	pinMode(_pin, OUTPUT);
	Serial.println("ARDUINO START.. wait 4 sec");
	digitalWrite(_pin, LOW);
	delay(300);
	digitalWrite(_pin, HIGH);  // RST 핀에 전류 인가, SI4730 전원 켜기 
	delay(300);
	Serial.println("SI4730 START..");

	Wire.beginTransmission(SI4730_ADDR);
	Wire.write(SI4730_POWER_UP);
	Wire.write(0x90); // FM 수신 모드          ARG 1
	Wire.write(0x05); // 아날로그 오디오 출력   ARG 2
	Wire.endTransmission(true);

	delay(500); // 오실레이터 발진 대기 500ms

	receiveResponse();
}


void Si4730::setFMFrequency(int freq)
{
	highbyte = freq >> 8;
	lowbyte = freq & 0x00FF;  // 주파수(16비트)를 8비트 2개로 분할 

	Wire.beginTransmission(SI4730_ADDR);
	Wire.write(SI4730_SET_FM_FREQ);
	Wire.write(0x00); // ARG1에서 BIT 기능 사용 X
	Wire.write(highbyte);
	Wire.write(lowbyte);
	// Wire.write(ANTCAP); 안테나 캐패시터 수동 설정, 사용하지 않음. ARG4.
	Wire.endTransmission(true);
	delay(50);

	receiveResponse();
}


void Si4730::setAMFrequency(int freq)
{
	highbyte = freq >> 8;
	lowbyte = freq & 0x00FF;  // 주파수(16비트)를 8비트 2개로 분할 

	Wire.beginTransmission(SI4730_ADDR);
	Wire.write(SI4730_SET_AM_FREQ);
	Wire.write(0x00);  // FAST Tuning 비활성, 활성화 0x11
	Wire.write(highbyte);
	Wire.write(lowbyte);
	Wire.endTransmission();
	delay(50);
}


void Si4730::seekFM(bool updown) // TRUE 값 받으면 주파수 위로 검색, FALSE면 아래로 검색
{
	Wire.beginTransmission(SI4730_ADDR);
	Wire.write(SI4730_SEEK_FM);
	if(updown==true)
		Wire.write(SI4730_SEEK_UP);
	else
		Wire.write(SI4730_SEEK_DOWN);
	Wire.endTransmission();

	delay(50);
}


void Si4730::seekAM(bool updown) // TRUE 값 받으면 주파수 위로 검색, FALSE면 아래로 검색
{
	Wire.beginTransmission(SI4730_ADDR);
	Wire.write(SI4730_SEEK_AM);
	if(updown==true)
		Wire.write(SI4730_SEEK_UP);
	else
		Wire.write(SI4730_SEEK_DOWN);
	Wire.write(0x00);
	Wire.write(0x00);
	Wire.write(0x00);
	Wire.write(0x00);
	delay(50);
}


void Si4730::channelFilterAM(char filter)
{
	Wire.beginTransmission(SI4730_ADDR);
	Wire.write(SI4730_SET_PROPERTY);
	Wire.write(0x00);
	Wire.write(0x31);  // AM 채널 필터 설정 
	Wire.write(0x02);  // AM 채널 필터 설정 
	Wire.write(0x01);  // AM PowerLine Noise 제거 필터 활성화(비활성 0x00)
	switch(filter)
	{
		case 6:
		Wire.write(SI4730_AM_FILTER_6);
		break;

		case 4:
		Wire.write(SI4730_AM_FILTER_4);
		break;

		case 3:
		Wire.write(SI4730_AM_FILTER_3);
		break;

		case 2:
		Wire.write(SI4730_AM_FILTER_2);
		break;

		case 1:
		Wire.write(SI4730_AM_FILTER_1);
		break;

		default:
		break;
	}
	delay(50);
}


void Si4730::receiveResponse()
{
	int cache;

	Wire.requestFrom(SI4730_ADDR, 1);   
	cache = Wire.read();
	Serial.println("I2C Response!");

	Serial.print("Received DATA : ");
	Serial.println(cache);
}



void Si4730::GET_REV()
{
	int rev[15];
	Wire.beginTransmission(SI4730_ADDR);
	Wire.write(0x10);
	Wire.endTransmission(true);

	delay(100);

	Wire.requestFrom(SI4730_ADDR, 16);

	for(int i=0; i<16; i++)
	{
		rev[i] = Wire.read();
		Serial.println(rev[i]);
	}
	delay(50);
}


void Si4730::FM_STATUS()
{
	int cache;
	Wire.beginTransmission(SI4730_ADDR);
	Wire.write(0x22);
	Wire.write(0x00);
	Wire.endTransmission(true);

	delay(50);
	Serial.println("FM STATUS");

	Wire.requestFrom(SI4730_ADDR, 8);
	for(int i=0; i<8; i++)
	{
		cache = Wire.read();
		Serial.println(cache);
	}
	delay(50);
}


void Si4730::powerDown() // PowerDown 모드로 전환 
{
	Wire.beginTransmission(SI4730_ADDR);
	Wire.write(SI4730_POWER_DOWN);
	Wire.endTransmission();
	delay(50);
	digitalWrite(_pin, LOW); // RST 핀 전원 공급 X
}