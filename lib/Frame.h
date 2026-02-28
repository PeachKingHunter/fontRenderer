#ifndef FRAME_H
#define FRAME_H

#include "fontManager/FontLoader.h"
#include "fontManager/FontRender.h"
#include <GL/gl.h>
#include <stdlib.h>
#include <string.h>

typedef struct frame {
  // Transform
  float posX;
  float posY;
  float sizeX;
  float sizeY;

  // Colors
  float backgroundColor[3];
  float borderColor[3];

  // For formatting 19x9
  Font *font;
} Frame;


Frame *createFrame(float posX, float posY, float sizeX, float sizeY, Font *font);
void changeFrameBorderColor(Frame *frame, float r, float g, float b);
void changeFrameBackgroundColor(Frame *frame, float r, float g, float b);
// void renderEmptySquare(float posX, float posY, float sizeX, float sizeY,
// float color[3]); void renderFilledSquare(float posX, float posY, float sizeX,
// float sizeY, float color[3]);
void getTransformFrame(Frame *frame, float *posX, float *posY, float *sizeX, float *sizeY);
void renderFrame(Frame *frame);
void freeFrame(Frame *frame);


#endif
