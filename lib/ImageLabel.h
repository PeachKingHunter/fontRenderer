#ifndef ImageLabel_H
#define ImageLabel_H

// Internal Lib
#include "Frame.h"

// External Lib
#include <GLFW/glfw3.h>
#include <GL/gl.h>

// Lib C
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

// Structure 
typedef struct imageLabel {
  // Inherits from Frame
  Frame *frame;

  // Image in the ImageLabel
  unsigned char *imgData;
  int32_t imgSize[2];
  
} ImageLabel;

// Methods
ImageLabel *createImageLabel(const char *imagePath, float posX, float posY, float sizeX, float sizeY);
int loadImage(ImageLabel *imageLabel, const char *imagePath);
void changeImageLabelBorderColor(ImageLabel *imageLabel, float r, float g, float b);
void changeImageLabelBackgroundColor(ImageLabel *imageLabel, float r, float g, float b);
void getTransformImageLabel(ImageLabel *imageLabel, float *posX, float *posY, float *sizeX, float *sizeY);
void renderImageLabel(ImageLabel *imageLabel);
void freeImageLabel(ImageLabel *imageLabel);

#endif
