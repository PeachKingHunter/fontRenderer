#ifndef FontLoader_H
#define FontLoader_H

#include "FileReader.h"

#include "GLFW/glfw3.h"
#include <GL/gl.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct simpleGlyph {
  int *posX;
  int *posY;
  uint16_t *nbPointContours;
  int nbContours;

  int xMin;
  int yMin;
  int xMax;
  int yMax;

} SimpleGlyph;

typedef struct font {
  SimpleGlyph **simpleGlyphs;
  int simpleGlyphsLen;
  int *unicodeToIndexTab;
  GLFWwindow *window;

} Font;

Font *loadFont(const char *fontPath);
void freeFont(void *font);

#endif
