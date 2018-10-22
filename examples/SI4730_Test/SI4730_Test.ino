#include <Wire.h>
#include <SI4730.h>

#define RSTPIN 13  // RESET Pin is set to 13

Si4730 radio = Si4730(RSTPIN);

void setup()
{
	Wire.begin();
	Serial.begin(9600);
	delay(200);

	Serial.println("Starting...");
	
	radio.powerUp(true);   // Initialize Si4730.
	radio.setFMFrequency(8970); // Set Frequency to FM 89.7MHz
	radio.AM_STATUS();
	radio.channelFilterFM(4);
}

void loop()
{

}