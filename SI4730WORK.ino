/*

목표 - Si4730의 기본적인 제어 라이브러리 제작 

유의 사항 - 각 명령을 전달한 후, 최소 10ms 대기할 것 

AM 채널 필터 1KHz 옵션, PowerLine Noise 제거 옵션 호환 불가능?

*/

#include <Wire.h>
#include <IRremote.h>

#define IR_REMOTE_PIN 12
#define SI4730_RST_PIN 13



int frequency = 9870, i=6;
char highbyte, lowbyte, filter=6;
int channel[16] = {8850, 8970, 9190, 9310, 9450, 9530, 9650, 9710, 9830, 9930, 10130, 10230, 10310, 10390, 10510, 10750}; // 16 Ch

IRrecv irrecv(IR_REMOTE_PIN);
decode_results results;


void setup() 
{
	Wire.begin();
	Serial.begin(9600);
	irrecv.enableIRIn(); // Start the receiver
	pinMode(SI4730_RST_PIN, OUTPUT);

	Serial.println("ARDUINO START.. wait 4 sec");
	digitalWrite(SI4730_RST_PIN, LOW);
	delay(300);
	digitalWrite(SI4730_RST_PIN, HIGH);  // RST 핀에 전류 인가, SI4730 전원 켜기 
	delay(300);
	Serial.println("SI4730 START..");

	powerUpRadio(true);
	setFMFrequency(9650);

	delay(100);
	//GET_REV();
	FM_STATUS();
}

void loop() 
{
	RemoteCont();
  	delay(500);
}


void RemoteCont()
{
    if (irrecv.decode(&results)) 
    {
        Serial.println(results.value);
        switch(results.value)
        {
            // case 0xFF42BD: // *  모드 변경 
            // mode++;
            // if(mode>2)
            //     mode=0;
            // lcd.clear();
            // lcd.setCursor(0,0);
            // lcd.print("   MODE : ");
            // lcd.print(mode);
            // lcdClear=false;
            // delay(700);
            // lcd.clear();
            // break;

            case 0xFFC23D:
            Serial.println("NEXT CHANNEL");
            i++;
            if(i>16)
              i=0;
            setFMFrequency(channel[i]); 
            break;

            case 0xFF22DD:
            Serial.println("PREV CHANNEL");
            i--;
            if(i<0)
              i=16;
            setFMFrequency(channel[i]);
            break;

            // case 0xFF02FD: // OK
            // Serial.println("RADIO ON/OFF");
            // if(radio_onoff==false)
            // {
            //     radio.setMute(false);
            //     radio_onoff=true;
            //     lcd.clear();
            //     lcd.print("   RADIO ON   ");
            //     delay(700);
            //     lcd.clear();
            // }
            // else
            // {
            //     radio.setMute(true);
            //     radio_onoff=false;
            //     lcd.clear();
            //     lcd.print("   RADIO OFF  ");
            //     delay(700);
            //     lcd.clear();
            // }
            // break;

            // case 0xFF629D:
            // Serial.println("VOLUME UP");
            // if(vol<15)
            //   vol+=2;
            // radio.setVolume(vol);
            // lcd.clear();
            // lcd.print("  VOLUME : ");
            // lcd.print(vol);
            // delay(700);
            // lcd.clear();
            // break;

            // case 0xFFA857:
            // Serial.println("VOLUME DOWN");
            // if(vol>1)
            //   vol-=2;
            // radio.setVolume(vol);
            // lcd.clear();
            // lcd.print("  VOLUME : ");
            // lcd.print(vol);
            // delay(700);
            // lcd.clear();
            // break;

            // case 0xFF6897: // 1
            // Serial.println("CHANNEL 1");
            // i=0;
            // radio.setFrequency(channel[i]);
            // break;

            // case 0xFF9867: // 2
            // Serial.println("CHANNEL 2");
            // i=1;
            // radio.setFrequency(channel[i]);
            // break;

            // case 0xFFB04F: // 3
            // Serial.println("CHANNEL 3");
            // i=6;
            // radio.setFrequency(channel[i]);
            // break;

            // case 0xFF30CF: // 4
            // Serial.println("CHANNEL 4");
            // i=11;
            // radio.setFrequency(channel[i]);
            // break;

            // case 0xFF52AD: // #
            // if(lcdBacklight==true)
            // {
            //     Serial.println("LCD OFF");
            //     lcd.noBacklight();
            //     lcdBacklight=false;
            //     break;
            // }
            // else if(lcdBacklight==false)
            // {
            //     Serial.println("LCD ON");
            //     lcd.backlight();
            //     lcdBacklight=true;
            //     lcd.clear();
            //     lcd.setCursor(0,0);
            //     lcd.print("LCD BACKLIGHT ON");
            //     delay(800);
            //     lcd.clear();
            //     break;
            // }
            // break;
            
        }
        irrecv.resume();
    }
}



void powerUpRadio(bool mode) // Si4730 초기화, mode가 true 면 FM, false면 AM
{
	digitalWrite(3, HIGH);  // RST 전류 인가
	delay(50);  // 50ms 동안 대기


	Wire.beginTransmission(SI4730_ADDR);
	Wire.write(SI4730_POWER_UP);
	Wire.write(0x90); // FM 수신 모드          ARG 1
	Wire.write(0x05); // 아날로그 오디오 출력   ARG 2
	Wire.endTransmission(true);

	delay(500); // 오실레이터 발진 대기 500ms

	receiveResponse();
}


void setFMFrequency(int freq)
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


void setAMFrequency(int freq)
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


void seekFM(bool updown) // TRUE 값 받으면 주파수 위로 검색, FALSE면 아래로 검색
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


void seekAM(bool updown) // TRUE 값 받으면 주파수 위로 검색, FALSE면 아래로 검색
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


void channelFilterAM(char filter)
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


void receiveResponse()
{
	int cache;

	Wire.requestFrom(SI4730_ADDR, 1);   
	cache = Wire.read();
	Serial.println("I2C Response!");

	Serial.print("Received DATA : ");
	Serial.println(cache);
}



void GET_REV()
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


void FM_STATUS()
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


void powerDownRadio() // PowerDown 모드로 전환 
{
	Wire.beginTransmission(SI4730_ADDR);
	Wire.write(SI4730_POWER_DOWN);
	Wire.endTransmission();
	delay(50);
	digitalWrite(3, LOW);
}