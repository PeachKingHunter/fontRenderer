#include "FileReader.h"

// ---- CONVERT ENDIAN ---- //
// Unsigned
uint16_t toBigEndian16(uint16_t num) {
  return ((num >> 8) & 0x00FF) | ((num << 8) & 0xFF00);
}
uint32_t toBigEndian32(uint32_t num) {
  return ((num >> 24) & 0x000000FF) | ((num >> 8) & 0x0000FF00) |
         ((num << 8) & 0x00FF0000) | ((num << 24) & 0xFF000000);
}

// Signed
int16_t toBigEndianS16(int16_t num) {
  uint16_t temp = ((uint16_t)num >> 8) | ((uint16_t)num << 8);
  return (int16_t)temp;
}

int32_t toBigEndianS32(int32_t num) {
  return ((num >> 24) & 0x000000FF) | ((num >> 8) & 0x0000FF00) |
         ((num << 8) & 0x00FF0000) | ((num << 24) & 0xFF000000);
}

// ---- READ FILES ---- //
// Read n characters
char *readChar(FILE *file, int len) {
  // Verif entry
  if (file == NULL)
    return NULL;

  char *str = (char *)malloc(sizeof(char) * (len + 1));
  if (str == NULL)
    return NULL;

  fread(str, sizeof(char), len, file);
  str[len] = '\0';
  return str;
}

// Unsigned
uint32_t readU32(FILE *file) {
  uint32_t num;
  fread(&num, 4, 1, file);
  return toBigEndian32(num);
}

uint16_t readU16(FILE *file) {
  uint16_t num;
  fread(&num, 2, 1, file);
  return toBigEndian16(num);
}

uint8_t readU8(FILE *file) {
  uint8_t num;
  fread(&num, 1, 1, file);
  return num;
}

// Signed
int16_t readS16(FILE *file) {
  int16_t num;
  fread(&num, 2, 1, file);
  return toBigEndianS16(num);
}
