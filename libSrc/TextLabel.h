#ifndef TextLabel_H
#define TextLabel_H

#include "Frame.h"
#include "fontManager/FontLoader.h"
#include "fontManager/FontRender.h"

#include <GL/gl.h>
#include <stdlib.h>
#include <string.h>

typedef struct textLabel {
  // Inherits from Frame
  Frame *frame;

  // Text in the TextLabel
  char *text;
  float *startLineOffsetX;
  float size;
  Font *font;
} TextLabel;

TextLabel *createTextLabel(char *text, Font *font, float posX, float posY, float sizeX, float sizeY);
int changeTextOfTextLabel(TextLabel *textLabel, char *text);
void changeTextLabelBorderColor(TextLabel *textLabel, float r, float g, float b);
void changeTextLabelBackgroundColor(TextLabel *textLabel, float r, float g, float b);
void getTransformTextLabel(TextLabel *textLabel, float *posX, float *posY, float *sizeX, float *sizeY);
void renderTextLabel(TextLabel *textLabel);
void freeTextLabel(TextLabel *textLabel);

#endif
