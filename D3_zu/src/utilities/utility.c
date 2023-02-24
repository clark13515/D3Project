#include "utility.h"

void U32ToBuf(uint8_t *buf, uint32_t data)
{
	buf[0] = data & 0xFF;
	buf[1] = (data >> 8) & 0xFF;
	buf[2] = (data >> 16) & 0xFF;
	buf[3] = (data >> 24) & 0xFF;
}

uint32_t BufToU32(uint8_t *buf)
{
	return buf[0] + ((uint32_t)buf[1] << 8) + ((uint32_t)buf[2] << 16) + ((uint32_t)buf[3] << 24);
}

void S32ToBuf(uint8_t *buf, int32_t data)
{
	buf[0] = data & 0xFF;
	buf[1] = (data >> 8) & 0xFF;
	buf[2] = (data >> 16) & 0xFF;
	buf[3] = (data >> 24) & 0xFF;
}

int32_t BufToS32(uint8_t *buf)
{
	return (int32_t)(buf[0] + ((uint32_t)buf[1] << 8) + ((uint32_t)buf[2] << 16) + ((uint32_t)buf[3] << 24));
}


void U16ToBuf(uint8_t *buf, uint16_t data)
{
	buf[0] = data & 0xFF;
	buf[1] = (data >> 8) & 0xFF;
}

uint16_t BufToU16(uint8_t *buf)
{
	return buf[0] + ((uint16_t)buf[1] << 8);
}

void FloatToBuf(uint8_t *buf, float data)
{
    uint8_t *p = (uint8_t*)(&data);
    buf[0] = p[0];
    buf[1] = p[1];
    buf[2] = p[2];
    buf[3] = p[3];
}

float BufToFloat(uint8_t *buf)
{
    float data;
    uint8_t *p = (uint8_t*)(&data);
    p[0] = buf[0];
    p[1] = buf[1];
    p[2] = buf[2];
    p[3] = buf[3];
    return data;
}

uint32_t CalCrc32(uint32_t *data, uint32_t size)
{
	uint32_t i,j,temp,crc = 0xFFFFFFFF;
	for(i = 0; i < size; i++)
	{
		temp = data[i];
		for(j = 0; j < 32; j++)
		{
			if( (crc ^ temp) & 0x80000000 )
			{
				crc = 0x04C11DB7 ^ (crc<<1);
			}
			else
			{
				crc <<=1;
			}
			temp<<=1;
		}
	}
	return crc;
}

uint8_t CalSum(uint8_t *buf, uint32_t len)
{
	uint8_t sum = 0;
	for(uint32_t i = 0; i < len; i++)
	{
		sum += buf[i];
	}
	return sum;
}

uint32_t FindMinPow2(uint32_t d)
{
	uint32_t res = 1;

	for(int n = 0; n < 32; n++)
	{
		res = 1U << n;
		if(res >= d)
			break;
	}

	return res;
}

int32_t MyRound(double x)
{
	return (((x) >= 0) ? (int32_t)((x) + 0.5) : (int32_t)((x) - 0.5));
}
