/*-----------------------------------------------------------------------------------------------------------
[GST2015]

Name : Huynh Ngoc Vinh
MS: 12520784
Info:Final Test


-----------------------------------------------------------------------------------------------------------*/
#include  <stdio.h>
#include <stdlib.h>
#include <string.h>
#include  "MKL46Z4.h"
#include 	"LED.h"
#include  "Button.h"
#include "_uart.h"
#include "PIT_TM.h"
unsigned int t,n,z=0,s2=0,s3=0,x=0;
int check;
char c;	
int g=0;
char *hexstring = "abcd";


unsigned char Hex[3000];
//unsigned char Hex3[]={0x15,0x1f,0xff,0xe0,0x00,0x68,0x41,0x00,0x20,0x1d,0xe3,0xff,0x1f,0x33,0xe3,0xff,0x1f,0x35,0xe3,0xff,0x1f,0x9b,
//0x15,0x1f,0xff,0xe0,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xdc,
//0x15,0x1f,0xff,0xe0,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x37,0xe3,0xff,0x1f,0x94,
//0x15,0x1f,0xff,0xe0,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x39,0xe3,0xff,0x1f,0x65,0xe1,0xff,0x1f,0x1e,
//0x15,0x1f,0xff,0xe0,0x40,0x3d,0xe3,0xff,0x1f,0x3d,0xe3,0xff,0x1f,0x3d,0xe3,0xff,0x1f,0x3d,0xe3,0xff,0x1f,0xb4,
//0x15,0x1f,0xff,0xe0,0x50,0x3d,0xe3,0xff,0x1f,0x3d,0xe3,0xff,0x1f,0x3d,0xe3,0xff,0x1f,0x3d,0xe3,0xff,0x1f,0xa4,
//0x15,0x1f,0xff,0xe0,0x60,0x3d,0xe3,0xff,0x1f,0x3d,0xe3,0xff,0x1f,0x3d,0xe3,0xff,0x1f,0x3d,0xe3,0xff,0x1f,0x94,
//0x15,0x1f,0xff,0xe0,0x70,0x3d,0xe3,0xff,0x1f,0x3d,0xe3,0xff,0x1f,0x3d,0xe3,0xff,0x1f,0x3d,0xe3,0xff,0x1f,0x84,
//0x15,0x1f,0xff,0xe0,0x80,0x3d,0xe3,0xff,0x1f,0x3d,0xe3,0xff,0x1f,0x3d,0xe3,0xff,0x1f,0x3d,0xe3,0xff,0x1f,0x74,
//0x15,0x1f,0xff,0xe0,0x90,0x3d,0xe3,0xff,0x1f,0x3d,0xe3,0xff,0x1f,0x3d,0xe3,0xff,0x1f,0x3d,0xe3,0xff,0x1f,0x64,
//0x15,0x1f,0xff,0xe0,0xa0,0x3d,0xe3,0xff,0x1f,0x3d,0xe3,0xff,0x1f,0x3d,0xe3,0xff,0x1f,0x3d,0xe3,0xff,0x1f,0x54,
//0x15,0x1f,0xff,0xe0,0xb0,0x3d,0xe3,0xff,0x1f,0x3d,0xe3,0xff,0x1f,0x3d,0xe3,0xff,0x1f,0x3d,0xe3,0xff,0x1f,0x44,
//0x15,0x1f,0xff,0xe0,0xc0,0x00,0xf0,0x02,0xf8,0x00,0xf0,0x3e,0xf8,0x0c,0xa0,0x30,0xc8,0x08,0x38,0x24,0x18,0xfc,
//0x15,0x1f,0xff,0xe0,0xd0,0x2d,0x18,0xa2,0x46,0x67,0x1e,0xab,0x46,0x54,0x46,0x5d,0x46,0xac,0x42,0x01,0xd1,0x7c,
//0x15,0x1f,0xff,0xe0,0xe0,0x00,0xf0,0x30,0xf8,0x7e,0x46,0x0f,0x3e,0x0f,0xcc,0xb6,0x46,0x01,0x26,0x33,0x42,0x70,
//0x15,0x1f,0xff,0xe0,0xf0,0x00,0xd0,0xfb,0x1a,0xa2,0x46,0xab,0x46,0x33,0x43,0x18,0x47,0x18,0x06,0x00,0x00,0x4b,
//0x15,0x1f,0xff,0xe1,0x00,0x38,0x06,0x00,0x00,0x10,0x3a,0x02,0xd3,0x78,0xc8,0x78,0xc1,0xfa,0xd8,0x52,0x07,0xea,
//0x15,0x1f,0xff,0xe1,0x10,0x01,0xd3,0x30,0xc8,0x30,0xc1,0x01,0xd5,0x04,0x68,0x0c,0x60,0x70,0x47,0x00,0x00,0xb9,
//0x15,0x1f,0xff,0xe1,0x20,0x00,0x23,0x00,0x24,0x00,0x25,0x00,0x26,0x10,0x3a,0x01,0xd3,0x78,0xc1,0xfb,0xd8,0x0f,
//0x15,0x1f,0xff,0xe1,0x30,0x52,0x07,0x00,0xd3,0x30,0xc1,0x00,0xd5,0x0b,0x60,0x70,0x47,0x1f,0xb5,0x1f,0xbd,0xf7,
//0x15,0x1f,0xff,0xe1,0x40,0x10,0xb5,0x10,0xbd,0x00,0xf0,0xa2,0xfa,0x11,0x46,0xff,0xf7,0xf7,0xff,0x00,0xf0,0x5a,
//0x15,0x1f,0xff,0xe1,0x50,0x63,0xf8,0x00,0xf0,0xba,0xfa,0x03,0xb4,0xff,0xf7,0xf2,0xff,0x03,0xbc,0x00,0xf0,0x4f,
//0x15,0x1f,0xff,0xe1,0x60,0xbf,0xfa,0x00,0x00,0x61,0x48,0x00,0x68,0x40,0x1c,0x60,0x49,0x08,0x60,0x70,0x47,0x9d,
//0x15,0x1f,0xff,0xe1,0x70,0x5e,0x4a,0x11,0x68,0x00,0xbf,0x5d,0x4a,0x12,0x68,0x52,0x1a,0x82,0x42,0xfa,0xd3,0x7d,
//0x15,0x1f,0xff,0xe1,0x80,0x70,0x47,0x5b,0x48,0x80,0x6b,0x03,0x21,0x09,0x03,0x08,0x43,0x58,0x49,0x88,0x63,0x1f,
//0x15,0x1f,0xff,0xe1,0x90,0x88,0x15,0x58,0x49,0x48,0x63,0x58,0x49,0x48,0x61,0x58,0x48,0x00,0x68,0x58,0x49,0x7f,
//0x15,0x1f,0xff,0xe1,0xa0,0x08,0x60,0x56,0x48,0x40,0x68,0x56,0x49,0x40,0x39,0x08,0x60,0x53,0x48,0x00,0x68,0x1a,
//0x15,0x1f,0xff,0xe1,0xb0,0x53,0x49,0x48,0x61,0x51,0x48,0x40,0x68,0x51,0x49,0x40,0x39,0x48,0x61,0x70,0x47,0x42,
//0x15,0x1f,0xff,0xe1,0xc0,0x00,0x28,0x05,0xd1,0x81,0x00,0x4d,0x4a,0x51,0x58,0x4d,0x4a,0x91,0x60,0x05,0xe0,0xff,
//0x15,0x1f,0xff,0xe1,0xd0,0x81,0x00,0x4a,0x4a,0x51,0x58,0x4a,0x4a,0x40,0x3a,0x91,0x60,0x70,0x47,0x00,0x28,0x7f,
//0x15,0x1f,0xff,0xe1,0xe0,0x05,0xd1,0x81,0x00,0x45,0x4a,0x51,0x58,0x45,0x4a,0x51,0x60,0x05,0xe0,0x81,0x00,0xd6,
//0x15,0x1f,0xff,0xe1,0xf0,0x42,0x4a,0x51,0x58,0x42,0x4a,0x40,0x3a,0x51,0x60,0x70,0x47,0x02,0x46,0x00,0x20,0xf0,
//0x15,0x1f,0xff,0xe2,0x00,0x06,0xe0,0x00,0x21,0x00,0xe0,0x49,0x1c,0x3e,0x4b,0x99,0x42,0xfb,0xd3,0x40,0x1c,0x10,
//0x15,0x1f,0xff,0xe2,0x10,0x3c,0x4b,0x98,0x42,0xf5,0xd3,0x70,0x47,0x00,0x25,0x00,0xf0,0xd6,0xf8,0x7d,0x21,0x79,
//0x15,0x1f,0xff,0xe2,0x20,0xc9,0x00,0x39,0x48,0x00,0x68,0x00,0xf0,0x81,0xf9,0x04,0x46,0x60,0x1e,0x37,0x49,0x66,
//0x15,0x1f,0xff,0xe2,0x30,0x88,0x42,0x00,0xd9,0x0c,0xe0,0x60,0x1e,0x35,0x49,0x48,0x61,0x03,0x21,0x08,0x1f,0x3b,
//0x15,0x1f,0xff,0xe2,0x40,0x00,0xf0,0x1c,0xf8,0x00,0x20,0x32,0x49,0x88,0x61,0x07,0x20,0x08,0x61,0x00,0xbf,0xd3,
//0x15,0x1f,0xff,0xe2,0x50,0xff,0xf7,0x97,0xff,0x11,0xe0,0x28,0x46,0xff,0xf7,0xb2,0xff,0x68,0x1c,0xff,0xf7,0x8e,
//0x15,0x1f,0xff,0xe2,0x60,0xaf,0xff,0xfa,0x20,0xff,0xf7,0xca,0xff,0x28,0x46,0xff,0xf7,0xb8,0xff,0x68,0x1c,0x64,
//0x15,0x1f,0xff,0xe2,0x70,0xff,0xf7,0xb5,0xff,0xfa,0x20,0xff,0xf7,0xc1,0xff,0xec,0xe7,0x10,0xb5,0x00,0x28,0x40,
//0x15,0x1f,0xff,0xe2,0x80,0x19,0xda,0x24,0x4a,0x03,0x07,0x1b,0x0f,0x08,0x3b,0x9b,0x08,0x9b,0x00,0xd2,0x58,0x2a,
//0x15,0x1f,0xff,0xe2,0x90,0x83,0x07,0xdc,0x0e,0xff,0x23,0xa3,0x40,0x9a,0x43,0x8b,0x07,0x1b,0x0e,0x84,0x07,0xbe,
//0x15,0x1f,0xff,0xe2,0xa0,0xe4,0x0e,0xa3,0x40,0x1a,0x43,0x1b,0x4b,0x04,0x07,0x24,0x0f,0x08,0x3c,0xa4,0x08,0x84,
//0x15,0x1f,0xff,0xe2,0xb0,0xa4,0x00,0x1a,0x51,0x18,0xe0,0x18,0x4a,0x03,0x23,0x1b,0x02,0xd2,0x18,0x83,0x08,0x19,
//0x15,0x1f,0xff,0xe2,0xc0,0x9b,0x00,0xd2,0x58,0x83,0x07,0xdc,0x0e,0xff,0x23,0xa3,0x40,0x9a,0x43,0x8b,0x07,0x7d,
//0x15,0x1f,0xff,0xe2,0xd0,0x1b,0x0e,0x84,0x07,0xe4,0x0e,0xa3,0x40,0x1a,0x43,0x0f,0x4b,0x03,0x24,0x24,0x02,0x8d,
//0x15,0x1f,0xff,0xe2,0xe0,0x1b,0x19,0x84,0x08,0xa4,0x00,0x1a,0x51,0x10,0xbd,0x00,0x00,0x00,0x40,0x00,0x20,0x0e,
//0x15,0x1f,0xff,0xe2,0xf0,0x00,0x80,0x04,0x40,0x40,0xd0,0x04,0x40,0x00,0xc0,0x04,0x40,0x0c,0xe7,0xff,0x1f,0xcd,
//0x15,0x1f,0xff,0xe3,0x00,0x00,0xf1,0x0f,0x40,0xc4,0x09,0x00,0x00,0x04,0x40,0x00,0x20,0xff,0xff,0xff,0x00,0x7b,
//0x15,0x1f,0xff,0xe3,0x10,0x00,0xe0,0x00,0xe0,0x1c,0xed,0x00,0xe0,0x00,0xe1,0x00,0xe0,0x0b,0x48,0x85,0x46,0x51,
//0x15,0x1f,0xff,0xe3,0x20,0x72,0xb6,0x0b,0x48,0x0b,0x49,0x01,0x60,0x62,0xb6,0x0b,0x48,0x80,0x47,0x0b,0x48,0x14,
//0x15,0x1f,0xff,0xe3,0x30,0x00,0x47,0xfe,0xe7,0xfe,0xe7,0xfe,0xe7,0xfe,0xe7,0xfe,0xe7,0xfe,0xe7,0x00,0x00,0x14,
//0x15,0x1f,0xff,0xe3,0x40,0x07,0x48,0x08,0x49,0x06,0x4a,0x08,0x4b,0x70,0x47,0x00,0x00,0x68,0x41,0x00,0x20,0xe6,
//0x15,0x1f,0xff,0xe3,0x50,0x08,0xed,0x00,0xe0,0x00,0xe0,0xff,0x1f,0x6d,0xe3,0xff,0x1f,0xc1,0xe0,0xff,0x1f,0x99,
//0x15,0x1f,0xff,0xe3,0x60,0x68,0x40,0x00,0x20,0x68,0x41,0x00,0x20,0x68,0x40,0x00,0x20,0x00,0x20,0x67,0x49,0x60,
//0x15,0x1f,0xff,0xe3,0x70,0x08,0x60,0x01,0x20,0x40,0x04,0x65,0x49,0xc0,0x39,0x48,0x60,0x06,0x20,0x64,0x49,0x8a,
//0x15,0x1f,0xff,0xe3,0x80,0x08,0x70,0x08,0x46,0x40,0x78,0x40,0x21,0x08,0x40,0x61,0x49,0x48,0x70,0x08,0x46,0x92,
//0x15,0x1f,0xff,0xe3,0x90,0xc0,0x78,0x80,0x06,0x80,0x0e,0x20,0x21,0x08,0x43,0x5d,0x49,0xc8,0x70,0x80,0x20,0x03,
//0x15,0x1f,0xff,0xe3,0xa0,0x5c,0x49,0x08,0x70,0x00,0x20,0x5a,0x49,0x08,0x71,0x48,0x71,0x00,0xbf,0x58,0x48,0xd8,
//0x15,0x1f,0xff,0xe3,0xb0,0x80,0x79,0x10,0x21,0x08,0x40,0x00,0x28,0xf9,0xd0,0x00,0xbf,0x54,0x48,0x80,0x79,0x82,
//0x15,0x1f,0xff,0xe3,0xc0,0x0c,0x21,0x08,0x40,0x00,0x28,0xf9,0xd1,0x70,0x47,0x70,0xb5,0x50,0x48,0x00,0x78,0xd6,
//0x15,0x1f,0xff,0xe3,0xd0,0xc0,0x21,0x08,0x40,0x00,0x28,0x68,0xd1,0x4d,0x48,0x40,0x79,0x40,0x21,0x08,0x40,0x98,
//0x15,0x1f,0xff,0xe3,0xe0,0x00,0x28,0x4e,0xd1,0x4a,0x48,0x00,0x78,0x04,0x21,0x08,0x40,0x00,0x28,0x15,0xd1,0x3d,
//0x15,0x1f,0xff,0xe3,0xf0,0x49,0x4c,0x47,0x48,0x00,0x78,0x38,0x21,0x08,0x40,0xc1,0x08,0x01,0x20,0x88,0x40,0x0a,
//0x15,0x1f,0xff,0xe4,0x00,0xc5,0xb2,0x29,0x46,0x20,0x46,0x00,0xf0,0x91,0xf8,0x04,0x46,0x40,0x48,0x40,0x78,0x99,
//0x15,0x1f,0xff,0xe4,0x10,0x30,0x21,0x08,0x40,0x00,0x28,0x03,0xd0,0x64,0x09,0x01,0xe0,0x01,0x24,0xe4,0x03,0xea,
//0x15,0x1f,0xff,0xe4,0x20,0x3b,0x48,0xc0,0x78,0xe0,0x21,0x08,0x40,0xc1,0x06,0xc9,0x0e,0x00,0x29,0x26,0xd1,0x06,
//0x15,0x1f,0xff,0xe4,0x30,0x40,0x11,0x03,0x00,0x00,0xf0,0x5b,0xf9,0x08,0x05,0x09,0x0d,0x11,0x15,0x19,0x1d,0xa1,
//0x15,0x1f,0xff,0xe4,0x40,0x20,0x23,0x05,0x20,0xc0,0x01,0x44,0x43,0x1a,0xe0,0x05,0x20,0x00,0x02,0x44,0x43,0x50,
//0x15,0x1f,0xff,0xe4,0x50,0x16,0xe0,0x0f,0x20,0xc0,0x01,0x44,0x43,0x12,0xe0,0x05,0x20,0x40,0x02,0x44,0x43,0x4b,
//0x15,0x1f,0xff,0xe4,0x60,0x0e,0xe0,0xb7,0x20,0x80,0x00,0x44,0x43,0x0a,0xe0,0xb7,0x20,0xc0,0x00,0x44,0x43,0xb4,
//0x15,0x1f,0xff,0xe4,0x70,0x06,0xe0,0x2a,0x48,0x44,0x43,0x03,0xe0,0x29,0x48,0x44,0x43,0x00,0xe0,0x00,0xbf,0x1f,
//0x15,0x1f,0xff,0xe4,0x80,0x38,0xe0,0x23,0x48,0x00,0x79,0xc0,0x06,0xc0,0x0e,0x45,0x1c,0x29,0x46,0x22,0x48,0x9e,
//0x15,0x1f,0xff,0xe4,0x90,0x00,0xf0,0x4c,0xf8,0x04,0x46,0x1e,0x48,0x40,0x79,0xc0,0x06,0xc0,0x0e,0x05,0x46,0xdc,
//0x15,0x1f,0xff,0xe4,0xa0,0x18,0x35,0x20,0x46,0x68,0x43,0x04,0x46,0x24,0xe0,0x19,0x48,0x00,0x78,0xc0,0x21,0xe2,
//0x15,0x1f,0xff,0xe4,0xb0,0x08,0x40,0x40,0x28,0x15,0xd1,0x16,0x48,0x40,0x78,0xc0,0x07,0xc0,0x0f,0x00,0x28,0xce,
//0x15,0x1f,0xff,0xe4,0xc0,0x02,0xd1,0x01,0x24,0xe4,0x03,0x15,0xe0,0x11,0x48,0x00,0x7a,0x0e,0x22,0x10,0x40,0x01,
//0x15,0x1f,0xff,0xe4,0xd0,0x42,0x08,0x01,0x20,0x90,0x40,0x01,0x46,0x12,0x48,0x00,0xf0,0x27,0xf8,0x04,0x46,0xe3,
//0x15,0x1f,0xff,0xe4,0xe0,0x08,0xe0,0x0b,0x48,0x00,0x78,0xc0,0x21,0x08,0x40,0x80,0x28,0x01,0xd1,0x0a,0x4c,0x5c,
//0x15,0x1f,0xff,0xe4,0xf0,0x00,0xe0,0x70,0xbd,0x05,0x48,0xc0,0x38,0x40,0x68,0x00,0x0f,0x41,0x1c,0x20,0x46,0x2c,
//0x15,0x1f,0xff,0xe5,0x00,0x00,0xf0,0x14,0xf8,0x08,0x49,0x08,0x60,0x00,0xbf,0xf2,0xe7,0x00,0x81,0x04,0x40,0xd5,
//0x15,0x1f,0xff,0xe5,0x10,0x00,0x40,0x06,0x40,0x00,0x50,0x06,0x40,0x00,0x12,0x7a,0x00,0x95,0x08,0x00,0x00,0x92,
//0x15,0x1f,0xff,0xe5,0x20,0x71,0x0b,0x00,0x00,0x00,0x09,0x3d,0x00,0x04,0x40,0x00,0x20,0x00,0x22,0x03,0x09,0x73,
//0x15,0x1f,0xff,0xe5,0x30,0x8b,0x42,0x2c,0xd3,0x03,0x0a,0x8b,0x42,0x11,0xd3,0x00,0x23,0x9c,0x46,0x4e,0xe0,0xfa,
//0x15,0x1f,0xff,0xe5,0x40,0x03,0x46,0x0b,0x43,0x3c,0xd4,0x00,0x22,0x43,0x08,0x8b,0x42,0x31,0xd3,0x03,0x09,0xb6,
//0x15,0x1f,0xff,0xe5,0x50,0x8b,0x42,0x1c,0xd3,0x03,0x0a,0x8b,0x42,0x01,0xd3,0x94,0x46,0x3f,0xe0,0xc3,0x09,0x68,
//0x15,0x1f,0xff,0xe5,0x60,0x8b,0x42,0x01,0xd3,0xcb,0x01,0xc0,0x1a,0x52,0x41,0x83,0x09,0x8b,0x42,0x01,0xd3,0x80,
//0x15,0x1f,0xff,0xe5,0x70,0x8b,0x01,0xc0,0x1a,0x52,0x41,0x43,0x09,0x8b,0x42,0x01,0xd3,0x4b,0x01,0xc0,0x1a,0x6b,
//0x15,0x1f,0xff,0xe5,0x80,0x52,0x41,0x03,0x09,0x8b,0x42,0x01,0xd3,0x0b,0x01,0xc0,0x1a,0x52,0x41,0xc3,0x08,0xe3,
//0x15,0x1f,0xff,0xe5,0x90,0x8b,0x42,0x01,0xd3,0xcb,0x00,0xc0,0x1a,0x52,0x41,0x83,0x08,0x8b,0x42,0x01,0xd3,0x52,
//0x15,0x1f,0xff,0xe5,0xa0,0x8b,0x00,0xc0,0x1a,0x52,0x41,0x43,0x08,0x8b,0x42,0x01,0xd3,0x4b,0x00,0xc0,0x1a,0x3e,
//0x15,0x1f,0xff,0xe5,0xb0,0x52,0x41,0x41,0x1a,0x00,0xd2,0x01,0x46,0x52,0x41,0x10,0x46,0x70,0x47,0x5d,0xe0,0x53,
//0x15,0x1f,0xff,0xe5,0xc0,0xca,0x0f,0x00,0xd0,0x49,0x42,0x03,0x10,0x00,0xd3,0x40,0x42,0x53,0x40,0x00,0x22,0xd6,
//0x15,0x1f,0xff,0xe5,0xd0,0x9c,0x46,0x03,0x09,0x8b,0x42,0x2d,0xd3,0x03,0x0a,0x8b,0x42,0x12,0xd3,0xfc,0x22,0x7f,
//0x15,0x1f,0xff,0xe5,0xe0,0x89,0x01,0x12,0xba,0x03,0x0a,0x8b,0x42,0x0c,0xd3,0x89,0x01,0x92,0x11,0x8b,0x42,0xfe,
//0x15,0x1f,0xff,0xe5,0xf0,0x08,0xd3,0x89,0x01,0x92,0x11,0x8b,0x42,0x04,0xd3,0x89,0x01,0x3a,0xd0,0x92,0x11,0x14,
//0x15,0x1f,0xff,0xe6,0x00,0x00,0xe0,0x89,0x09,0xc3,0x09,0x8b,0x42,0x01,0xd3,0xcb,0x01,0xc0,0x1a,0x52,0x41,0xce,
//0x15,0x1f,0xff,0xe6,0x10,0x83,0x09,0x8b,0x42,0x01,0xd3,0x8b,0x01,0xc0,0x1a,0x52,0x41,0x43,0x09,0x8b,0x42,0x97,
//0x15,0x1f,0xff,0xe6,0x20,0x01,0xd3,0x4b,0x01,0xc0,0x1a,0x52,0x41,0x03,0x09,0x8b,0x42,0x01,0xd3,0x0b,0x01,0x80,
//0x15,0x1f,0xff,0xe6,0x30,0xc0,0x1a,0x52,0x41,0xc3,0x08,0x8b,0x42,0x01,0xd3,0xcb,0x00,0xc0,0x1a,0x52,0x41,0xa5,
//0x15,0x1f,0xff,0xe6,0x40,0x83,0x08,0x8b,0x42,0x01,0xd3,0x8b,0x00,0xc0,0x1a,0x52,0x41,0xd9,0xd2,0x43,0x08,0x8c,
//0x15,0x1f,0xff,0xe6,0x50,0x8b,0x42,0x01,0xd3,0x4b,0x00,0xc0,0x1a,0x52,0x41,0x41,0x1a,0x00,0xd2,0x01,0x46,0xc9,
//0x15,0x1f,0xff,0xe6,0x60,0x63,0x46,0x52,0x41,0x5b,0x10,0x10,0x46,0x01,0xd3,0x40,0x42,0x00,0x2b,0x00,0xd5,0x33,
//0x15,0x1f,0xff,0xe6,0x70,0x49,0x42,0x70,0x47,0x63,0x46,0x5b,0x10,0x00,0xd3,0x40,0x42,0x01,0xb5,0x00,0x20,0xf5,
//0x15,0x1f,0xff,0xe6,0x80,0xc0,0x46,0xc0,0x46,0x02,0xbd,0x70,0x47,0x70,0x47,0x70,0x47,0x75,0x46,0x00,0xf0,0xcb,
//0x15,0x1f,0xff,0xe6,0x90,0x23,0xf8,0xae,0x46,0x05,0x00,0x69,0x46,0x53,0x46,0xc0,0x08,0xc0,0x00,0x85,0x46,0xa7,
//0x15,0x1f,0xff,0xe6,0xa0,0x18,0xb0,0x20,0xb5,0xff,0xf7,0x4c,0xfe,0x60,0xbc,0x00,0x27,0x49,0x08,0xb6,0x46,0xd9,
//0x15,0x1f,0xff,0xe6,0xb0,0x00,0x26,0xc0,0xc5,0xc0,0xc5,0xc0,0xc5,0xc0,0xc5,0xc0,0xc5,0xc0,0xc5,0xc0,0xc5,0x6d,
//0x15,0x1f,0xff,0xe6,0xc0,0xc0,0xc5,0x40,0x3d,0x49,0x00,0x8d,0x46,0x70,0x47,0x04,0x46,0xc0,0x46,0xc0,0x46,0xfb,
//0x15,0x1f,0xff,0xe6,0xd0,0x20,0x46,0xff,0xf7,0x40,0xfd,0x00,0x00,0x00,0x48,0x70,0x47,0x08,0x40,0x00,0x20,0x16,
//0x15,0x1f,0xff,0xe6,0xe0,0x01,0x49,0x18,0x20,0xab,0xbe,0xfe,0xe7,0x26,0x00,0x02,0x00,0x70,0x47,0x30,0xb4,0x73,
//0x15,0x1f,0xff,0xe6,0xf0,0x74,0x46,0x64,0x1e,0x25,0x78,0x64,0x1c,0xab,0x42,0x04,0xd3,0x63,0x5d,0x5b,0x00,0xbe,
//0x15,0x1f,0xff,0xe7,0x00,0xe3,0x18,0x30,0xbc,0x18,0x47,0x1d,0x46,0xf8,0xe7,0x00,0x00,0x00,0x00,0x00,0x20,0x3d,
//0x15,0x1f,0xff,0xe7,0x10,0x20,0x00,0x00,0x00,0x34,0xe7,0xff,0x1f,0x00,0x40,0x00,0x20,0x08,0x00,0x00,0x00,0x14,
//0x15,0x1f,0xff,0xe7,0x20,0x04,0xe1,0xff,0x1f,0x3c,0xe7,0xff,0x1f,0x08,0x40,0x00,0x20,0x60,0x01,0x00,0x00,0xb8,
//0x11,0x1f,0xff,0xe7,0x30,0x20,0xe1,0xff,0x1f,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x02,0x18,0x05,0x1F,0xFF,0xE0,
//0xC1,0x3B};

	char buffer[47];

int count =0;
char *get_string(void);
unsigned char TG =0xFF;
/*-----------------------------------------------------*/
/* Delay */
/*-----------------------------------------------------*/
volatile uint32_t msTicks; /* count 1ms timerStick */
void SysTick_Handler(void) {
  msTicks++;  								  /* increment counter necessary in Delay() */
}
/*Delay number of tick stick (1ms)*/
static void Delay (uint32_t dlyTicks) {
  uint32_t curTicks;

  curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks);
}
__asm void Jump_To_Address(void)
{
	LDR R0,=0x1FFFE0C1
	LDR R1,[R0,#4]
	BX R1
}



int Check_Sum(char c[47],unsigned char hex[3000],int temp)
{
	int i,j=0,sums=0,r, sum[47], s[47];
	if(c[0] !=0&&c[1]!=0)
	{
	for(i=2;i<47;i++)
	{
	if(c[i]<='9')
		s[i]=c[i]-48;
	if(c[i]>='A')
		s[i]=c[i]-55;
	}
	for(i=2;i<45;i+=2)
{
	sum[j]=((s[i]*16)|s[i+1]);
	j++;
}

if(temp==1)
{
	for(i=0;i<sum[0]+1;i++)
{
	hex[z]=sum[i];
	z++;
}
}




if(temp == 1)
{
for(i=0;i<sum[0];i++)
				sums+=sum[i];
}


}

if((~sums&0x00FF)==sum[sum[0]])
//check =1;
printf("T");
else 
//	check =0;
printf("F");

return sums;

}
void StartApplication( void (*StartAddress)() )
{
     (*StartAddress)();
		Jump_To_Address();
}

uint32_t getAdrr(unsigned char *ptrData){
 return ((ptrData[1] << 24) | (ptrData[2] <<16)| (ptrData[3] <<8) | (ptrData[4])) ;
 }
//void write(unsigned char * ptrData)
//{
//	unsigned char *ptrWrite;
//	uint16_t iIndex = 0,iLength = 0,iIndexWrite = 0,iTemp  = 0;
//	uint32_t u32Adreess = 0;
//	unsigned int  	m_StartAddress = 0x1FFFE0C1; 
//	
//	while(1)
//	{		
//		if(ptrData[iIndex] == 0x15|ptrData[iIndex] == 0x11){
//			//== S3, get length and adrress after that write data to RAM
//			iLength = ptrData[iIndex ];
//			u32Adreess = getAdrr(&ptrData[iIndex]);//iIndex l� cua tung dong
//			ptrWrite = (unsigned char*)u32Adreess;
//			for(iIndexWrite = 0; iIndexWrite < (iLength - 5); iIndexWrite++){
//				iTemp = iIndexWrite + 5 + iIndex;
//				ptrWrite[iIndexWrite] =  ptrData[iTemp];
//			}			
//			
//			iIndex += iLength ; //+ 1 checksum.
//		}//End if S3
//		if(ptrData[iIndex] == 0x05){
//			//== S7, it mean end of write. now jump to RAM
//	 
//     StartApplication( (void (*)())m_StartAddress );
//			//StartApplication( (void (*)())m_StartAddress );
//		//	Jump_To_Address();
//		}//end if S7
//		iIndex++;
//	}//End while(1)
////	m_StartAddress = 0x1FFFE0C1;
////	StartApplication( (void (*)())m_StartAddress );
//}
void Load_To_Ram(unsigned char *Data)
{
	int e,s,size,count;
		uint32_t temp;
		unsigned char i;
		unsigned char *ptr;
		unsigned int leghth;
		unsigned int  	m_StartAddress = 0x1FFFE0C1; 
	
	size =sizeof(Data) / sizeof(int);
		for(e=0;e<2579;e++) //5412
	{
			if((Data[e] == 0x15| Data[e] == 0x11)&& Data[e+1] == 0x1f)
			{
				leghth =Data[e];
				temp=getAdrr(&Data[e]);
				ptr= (unsigned char *)temp;
				for(i=0;i<(leghth-5);i++)
				{
				ptr[i]= Data[e+5+i];
				
				}
				e=e+leghth;
			}
			if(Data[e] == 0x05 && Data[e+1] == 0x1f)
			{
				StartApplication( (void (*)())m_StartAddress );
				
				
//				for(s=0;s<4;s++)
//			{
//				iAddress[s] = Data[e+1];
//				e++;
//				Address[s2] =iAddress[s];
//				s2++;
//			}
//				for(s=0;s<16;s++)
//			{
//			iDl[s] = Data[e+4];
//			e++;
//			Dl[s3] =iDl[s];
//			s3++;
//			}
			}
}
//	for(e=0;e<1000;e++) //5412
//	{
//	if(e%16 ==0)
//		printf("\n");
//	printf("%02x",Dl[e]);
//}
//	for(e=0;e<1000;e++) //5412
//	{
//	if(e%4 ==0)
//		printf("\n");
//	printf("%02x",Address[e]);
//}

}


void PORTC_PORTD_IRQHandler(void) // Handler INTERRUPT SW1
{
	int e,a;
	TG=~TG; //Bien dao
	if(TG == 0x00)
	{
			UART0->C2 |= UART0_C2_RE_MASK;
		LED_Off(1);
	}
	else
	{
			
				Load_To_Ram(Hex);
//			for(e=0;e<3000;e++)
//			{
//				if(e%22==0)
//					printf("\n");
//				printf("0x%02x,",Hex[e]);
//			}
//			
	}


		PORTC->PCR[12]|= (1UL << 24);
		PORTC->PCR[3]|= (1UL << 24);			/* Clear  Flag */
}


void PIT_IRQHandler(void)
{ int a,e;

	/* every 30s toggle LED */
	/* Interrupt PIT1 */
	if(PIT->CHANNEL[0].TFLG == 1)
	{
			if(c == 'N')
			{
				LED_Off(0);
				printf("\nUart0\nBaud:115200\nData bits: 8\nParity bit: no\nStop bit: 1 \n");
				printf("Da Truyen Hoan Tat\n");
				printf("Nhan Button 1 De Load Program to Ram\n");
				printf("Good Luck\n");


			}

		count ++;
		
		PIT->CHANNEL[0].TFLG =0x01; // clear flag
	}
	if(PIT->CHANNEL[1].TFLG == 1)
	{
		
	
		PIT->CHANNEL[1].TFLG =0x01; // clear flag
	}
	
			
		
				
			
			
		}

void UART0_IRQHandler(void)
{
int a,e,i,i2,i3;
	if (UART0->S1&UART_S1_RDRF_MASK)
	{
		c = UART0->D;
		buffer[g]=c;
		g++;
		if(g==3 && buffer[0]=='S')
		{
		i2=(buffer[2]-48)*16+(buffer[3]-48);
		}
		i3=i2*2+3;
		printf("%d",i3);
	}

	
if(g==47) //size 45
{
	
for(e=0;e<47;e++)
	{
							if(buffer[e] == 'S')
							{ 
		
								a=Check_Sum(buffer,Hex,1);
							//	printf("%d\n",a);
								
							}
						
							
						
						}

	g=0;
}
}

void MCG_FEI(void)
{
    MCG->C1 |= MCG_C1_IREFS_MASK; // Internal clock (32.768kHz) for FLL

    MCG->C4 &= ~MCG_C4_DRST_DRS_MASK;
    MCG->C4 |= MCG_C4_DMX32_MASK | MCG_C4_DRST_DRS(1);// 32.768 * 1464 = 47972.352kHz ~ 48MHz
    
    MCG->C1 &= ~MCG_C1_CLKS_MASK; // Output of FLL is selected for MCGOUTCLK

    while((MCG->S & MCG_S_IREFST_MASK) == 0); // wait for Internal clock is selected
    while((MCG->S & MCG_S_CLKST_MASK) != 0); // wait for FLL is selected
}

int main()
{

	uint32_t r;
 /* Config LED,BUTTON,LCD,PIT */
		
	LED_Init(); 
	Button_init();
	Pit_init();
	
  MCG_FEI();
 Uart0_init(115200); // selection baud with FEQ =48MHZ
	/*	-	Baud rate: 115200, Data bit: 8; Parity: None; Stop bits: 1; Flow Control: None. */
	// Load_To_Ram(Hex);
//	LED_On(1); //LED DO;
//	LED_On(0); //LED Xanh
	while(1)
	{

			
		
	
	}
}