#ifndef FileReader_H
#define FileReader_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

uint16_t toBigEndian16(uint16_t num);
uint32_t toBigEndian32(uint32_t num);

int16_t toBigEndianS16(int16_t num);
int32_t toBigEndianS32(int32_t num);

char *readChar(FILE *file, int len);

uint32_t readU32(FILE *file);
uint16_t readU16(FILE *file);
uint8_t readU8(FILE *file);
int16_t readS16(FILE *file);

#endif
