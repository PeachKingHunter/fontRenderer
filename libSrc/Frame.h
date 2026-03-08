#ifndef FRAME_H
#define FRAME_H

// Internal Lib
#include "fontManager/FontLoader.h"
#include "fontManager/FontRender.h"

// External Lib
#include <GL/gl.h>

// Lib C
#include <stdlib.h>
#include <string.h>

// DEFINES
#define MOUVEMENT_TYPE_CUR 0
#define MOUVEMENT_TYPE_ABS 1

// Structure
typedef struct frame {
  // Transform
  float posX;
  float posY;
  float sizeX;
  float sizeY;

  // Colors
  float backgroundColor[3];
  float borderColor[3];
} Frame;

// Creation / Destruction
Frame *createFrame(float posX, float posY, float sizeX, float sizeY);
void freeFrame(Frame *frame);

// Change color
void changeFrameBorderColor(Frame *frame, float r, float g, float b);
void changeFrameBackgroundColor(Frame *frame, float r, float g, float b);

// Transform
void movePosXFrame(Frame *frame, int moveInX, int movementType);
void movePosYFrame(Frame *frame, int moveInY, int movementType);
void getTransformFrame(Frame *frame, float *posX, float *posY, float *sizeX,
                       float *sizeY); // Give data to pointers

// Render
void renderFrame(Frame *frame);

#endif
