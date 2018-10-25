#include <Wire.h>
#include <SI4730.h>

#define RSTPIN 13  // RESET Pin is connected to Pin 13

Si4730 radio = Si4730(RSTPIN);

// Si4730 doesn't turn on just connect GND and VCC(3.3V) like other chips.
// This chip has RST Pin, If you connect RST to DC 3.3V, chip will turn on.

// So, you can use transistor or relay to control this 3.3V. 
// Si4730 radio = Si4730(PIN number connect with relay or transistor);

// or if you use board which uses 3.3V output GPIO, just set to 
// Si4730 radio = Si4730(Pin number which is directly connected to RST Pin);




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
