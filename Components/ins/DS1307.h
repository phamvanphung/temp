
#define SEC     	0x00
#define MIN     	0x01
#define HOUR    	0x02
#define DAY		    0x03
#define DATE		0x04
#define MONTH		0x05
#define YEAR		0x06

unsigned char DS1307_read(unsigned char addr);
void DS1307_Write(unsigned char addr,unsigned char dat);
void Out_1Hz();