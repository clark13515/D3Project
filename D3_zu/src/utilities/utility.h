#ifndef _UTILITY_H
#define _UTILITY_H

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>

void     U32ToBuf(uint8_t *buf, uint32_t data);
uint32_t BufToU32(uint8_t *buf);
void     S32ToBuf(uint8_t *buf, int32_t data);
int32_t  BufToS32(uint8_t *buf);
void     U16ToBuf(uint8_t *buf, uint16_t data);
uint16_t BufToU16(uint8_t *buf);
void     FloatToBuf(uint8_t *buf, float data);
float    BufToFloat(uint8_t *buf);

uint32_t CalCrc32(uint32_t *data, uint32_t size);
uint8_t  CalSum(uint8_t *buf, uint32_t len);
uint32_t FindMinPow2(uint32_t d);

int32_t MyRound(double x);
#ifdef __cplusplus
}
#endif

#endif  // _UTILITY_H
