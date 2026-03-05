#ifndef ImageLabel_H
#define ImageLabel_H

#include "Frame.h"

#include <stdio.h>
#include <GL/gl.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct imageLabel {
  // Inherits from Frame
  Frame *frame;

  // Image in the ImageLabel
  unsigned char *imgData;
  int32_t imgSize[2];
  
} ImageLabel;

ImageLabel *createImageLabel(const char *imagePath, Font *font, float posX, float posY, float sizeX, float sizeY);
int loadImage(ImageLabel *imageLabel, const char *imagePath);
void changeImageLabelBorderColor(ImageLabel *imageLabel, float r, float g, float b);
void changeImageLabelBackgroundColor(ImageLabel *imageLabel, float r, float g, float b);
void getTransformImageLabel(ImageLabel *imageLabel, float *posX, float *posY, float *sizeX, float *sizeY);
void renderImageLabel(ImageLabel *imageLabel);
void freeImageLabel(ImageLabel *imageLabel);

#endif
