#ifndef ImageLabel_H
#define ImageLabel_H

// Internal Lib
#include "Frame.h"

// External Lib
#include <GL/gl.h>
#include <GLFW/glfw3.h>

// Lib C
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure
typedef struct imageLabel {
  // Inherits from Frame
  Frame *frame;

  // Image in the ImageLabel
  unsigned char *imgData;
  int32_t imgSize[2];

  GLuint textureID;

} ImageLabel;

// Creation & Destruction
ImageLabel *createImageLabel(const char *imagePath, float posX, float posY,
                             float sizeX, float sizeY);
void freeImageLabel(ImageLabel *imageLabel);

// Others
int loadImage(ImageLabel *imageLabel, const char *imagePath);
void changeImageLabelBorderColor(ImageLabel *imageLabel, float r, float g,
                                 float b);
void changeImageLabelBackgroundColor(ImageLabel *imageLabel, float r, float g,
                                     float b);

// Transform
void movePosXImageLabel(ImageLabel *imageLabel, int moveInX, int movementType);
void movePosYImageLabel(ImageLabel *imageLabel, int moveInY, int movementType);
void getTransformImageLabel(ImageLabel *imageLabel, float *posX, float *posY,
                            float *sizeX, float *sizeY);

// Render
void renderImageLabel(ImageLabel *imageLabel);

#endif
