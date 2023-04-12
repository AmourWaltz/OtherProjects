#ifndef _SCOPE_H_
#define _SCOPE_H_


#include "include.h"
#include "common.h"

#define SCOPE_DATA_MODE 0
#define SCOPE_IMAGE_MODE 1


unsigned short CRC_CHECK(unsigned char *Buf, unsigned char CRC_CNT);


void VisualScope_Send(uint8 u8Mode);


#endif