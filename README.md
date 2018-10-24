# Si4730-Arduino-Library
SiliconLabs Si4730 FM/AM Receiver Arduino Library

Library for Si4730 D60.




Command to use : 



powerUp(bool); - turn on Si4730.  true = FM  /  false = AM

setFMFrequency(uint16_t); - Set Frequency to received integer. ex) 98.7MHz -> 9870, 102.35MHz -> 10235

setAMFrequency(uint16_t); - Set Frequency to received integer. ex) 1566KHz -> 1566.

seekFM(bool); - Seek FM Frequency.  True to seek UP Frequency, False to DOWN.

seekAM(bool); - Seek AM Frequency.  True to seek UP Frequency, False to DOWN.

setVolume(uint8_t); - Set Volume. 0 ~ 63. default is 63.

setMute(bool); - Set Mute or UnMute.  True to set Mute, False to set UnMute.

channelFilterFM(uint8_t); - Set channel bandwidth of FM. 5=auto, 4=110KHz, 3=84KHz, 2=60KHz, 1=40KHz.

channelFilterAM(uint8_t); - Set channel bandwidth of AM. 1~6. 1=1KHz, ... , 6=6KHz.

deEmphasisAM(bool); - Set DeEmphasis to AM. True to set. False to clear.

spaceSeekAM(bool); - Set AM Seek Frequency 10KHz or 9KHz. True to 9KHz, False to 10KHz.

receiveResponse(); - Receive response from Si4730.

GET_REV(); - Get revision from Si4730.

FM_STATUS(); - Get FM status from Si4730. 

AM_STATUS(); - Get AM status from Si4730.

powerDown(); - Turn Off Si4730 Chip. 



May compatible with other Si473x chip, but can't use LW, SW, WB etc...
