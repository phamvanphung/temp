#include <stdio.h>
#include "M051Series.h"
#include "TM1368.h"
#include "Delay_sys.h"

const char FontNumber[] =
{
  0x3F, //  0  0
  0x06, //  1  1
  0x5B, //  2  2
  0x4F, //  3  3
  0x66, //  4  4
  0x6D, //  5  5
  0x7D, //  6  6
  0x07, //  7  7
  0x7F, //  8  8
  0x6F, //  9  9
  0x77, // 10  A
  0x7C, // 11  B
  0x39, // 12  C
  0x5E, // 13  D
  0x79, // 14  E
  0x71, // 15  F
  0x3D, // 16  G
  0x76, // 17  H
  0x00, // 18
  0x40, // 19  -
  0x80, // 20  .
  0x63, // 21  *
  0x78, // 22  t
  0x1E, // 23  J
  0x38, // 24  L
};

void delay_us_soft(unsigned int t)
{
 	while(t--);
}

void Write1638(unsigned char Data)
{
  unsigned char i;
  for(i=0 ; i<8 ; i++)
  {
    PIN_DIO = (Data & 0x01);
    PIN_CLK = 0;
    Data >>= 1;
    PIN_CLK = 1;
  }
}

void WriteCmd(unsigned char cmd)
{
  PIN_STB = 0;
  Write1638(cmd);
  PIN_STB = 1;
}

///////////////////////////////////////////////////////
//                                                   //
//      Read buttons                                 //
//      1F 17 0F 07 1B 13 0B 03                      //
//                                                   //
///////////////////////////////////////////////////////
unsigned char ReadKey(void)
{
  unsigned char i;
  unsigned char temp=32;
  PIN_STB = 0;
  Write1638(0x42);              // Switch to read mode
  Set_DI;						            // Set DIO = input
  for(i=0; i<32; i++)           // Read 4 bytes
  {
    temp--;
    PIN_CLK = 0;
    delay_us_soft(1);                // Fast MCU (>4MHz)
    if(PIN_DIO){
      temp<<=1;
    }
      
    PIN_CLK = 1;
  }
  Set_DO;                       // Set DIO = output
  PIN_STB = 1;
  return temp;                  // 0x00 ~ 0xFF
}

void WriteData(unsigned char Adress,unsigned char Data)
{
  WriteCmd(0x44);
  PIN_STB = 0;
  Write1638(0xC0 | Adress);
  Write1638(Data);
  PIN_STB = 1;
}

///////////////////////////////////////////////////////
//                                                   //
//      Clear display, turn leds off                 //
//                                                   //
///////////////////////////////////////////////////////
void LedReset (void)
{
  unsigned char i;
  PIN_CLK = 1;
  WriteCmd(0x40);
	WriteCmd(0x8B);
  PIN_STB = 0;
  Write1638(0xC0);
  for(i=0 ; i<32 ; i++)
  {
    Write1638(0x00);
  }
  PIN_STB = 1;
  PIN_CLK = 1;
}

///////////////////////////////////////////////////////
//                                                   //
//        Led - positions 1,3,5,7,9,11,13,15         //
//        0 - turn led ON, 1 - turn OFF              //
//        7Seg - positions 0,2,4,6,8,10,12,14        //
//                                                   //
///////////////////////////////////////////////////////
void WriteDigit(char number,char pos)
{
  WriteCmd(0x44);
  PIN_STB = 0;
  Write1638(0xC0 | pos);
  Write1638(FontNumber[number]);
  PIN_STB = 1;
}
