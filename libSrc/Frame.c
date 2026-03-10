#include "Frame.h"

// Imported Global variables
extern int resolution[2];
extern float aspectRatio[2];
extern float camPos[2];
extern GLFWwindow *window;

// Creation / Destruction
Frame *createFrame(float posX, float posY, float sizeX, float sizeY) {
  // Create the Frame
  Frame *frame = (Frame *)malloc(sizeof(Frame));
  if (frame == NULL) {
    printf("Error fn: createFrame\n");
    return NULL;
  }

  int width = resolution[0];
  // Copy argument to textLabel's var
  frame->posX = posX / width * 2.0f - 1;
  frame->posY = -posY / width * 2.0f + aspectRatio[1] / aspectRatio[0];
  frame->sizeX = (sizeX / width * 2);
  frame->sizeY = (sizeY / width * 2);

  // Default color
  frame->backgroundColor[0] = -1;
  frame->borderColor[0] = 1;
  frame->borderColor[1] = 1;
  frame->borderColor[2] = 1;

  return frame;
}

void freeFrame(Frame *frame) {
  // Verif entry
  if (frame == NULL) {
    printf("Error fn: freeFrame\n");
    return;
  }

  // Free main struct
  free(frame);
}

// Color
void changeFrameBorderColor(Frame *frame, float r, float g, float b) {
  // Verif entry
  if (frame == NULL) {
    printf("Error fn: changeFrameBorderColor\n");
    return;
  }

  // Change color
  frame->borderColor[0] = r;
  frame->borderColor[1] = g;
  frame->borderColor[2] = b;
}

void changeFrameBackgroundColor(Frame *frame, float r, float g, float b) {
  // Verif entry
  if (frame == NULL) {
    printf("Error fn: changeFrameBackgroundColor\n");
    return;
  }

  // Change color
  frame->backgroundColor[0] = r;
  frame->backgroundColor[1] = g;
  frame->backgroundColor[2] = b;
}

// Transform
void movePosXFrame(Frame *frame, int moveInX, int movementType) {
  // Verif entry
  if (frame == NULL) {
    printf("Error fn: movePosXFrame\n");
    return;
  }

  float width = resolution[0];
  float moveInXF = moveInX / width * 2.0f;
  if (movementType == MOUVEMENT_TYPE_CUR) {
    frame->posX += moveInXF;

  } else if (movementType == MOUVEMENT_TYPE_ABS) {
    frame->posX = moveInXF;
  }
}

void movePosYFrame(Frame *frame, int moveInY, int movementType) {
  // Verif entry
  if (frame == NULL) {
    printf("Error fn: movePosYFrame\n");
    return;
  }

  float width = resolution[0];
  float moveInYF = -(float)moveInY / width * 2.0f;
  if (movementType == MOUVEMENT_TYPE_CUR) {
    frame->posY += moveInYF;

  } else if (movementType == MOUVEMENT_TYPE_ABS) {
    frame->posY = moveInYF;
  }
}

void getTransformFrame(Frame *frame, float *posX, float *posY, float *sizeX,
                       float *sizeY) {
  // Verif entry
  if (frame == NULL) {
       printf("Error fn: getTransformFrame\n");
 return;
  }

  *posX = frame->posX;
  *posY = frame->posY;
  *sizeX = frame->sizeX;
  *sizeY = frame->sizeY;
}

// Render
void renderFilledSquare(float posX, float posY, float sizeX, float sizeY,
                        float color[3]) {
  // Is transparent ?
  if (color == NULL)
    return;

  // Render border for debug
  glBegin(GL_POLYGON);
  glColor3f(color[0], color[1], color[2]);
  glVertex2f(posX, posY);
  glVertex2f(posX + sizeX, posY);
  glVertex2f(posX + sizeX, posY - sizeY);
  glVertex2f(posX, posY - sizeY);
  glEnd();
}

void renderEmptySquare(float posX, float posY, float sizeX, float sizeY,
                       float color[3]) {
  // Is transparent ?
  if (color == NULL)
    return;

  // Render border for debug
  glBegin(GL_LINES);
  glColor3f(color[0], color[1], color[2]);
  glVertex3f(posX, posY, 0);
  glVertex3f(posX + sizeX, posY, 0);
  glEnd();

  glBegin(GL_LINES);
  glColor3f(color[0], color[1], color[2]);
  glVertex3f(posX + sizeX, posY, 0);
  glVertex3f(posX + sizeX, posY - sizeY, 0);
  glEnd();

  glBegin(GL_LINES);
  glColor3f(color[0], color[1], color[2]);
  glVertex3f(posX + sizeX, posY - sizeY, 0);
  glVertex3f(posX, posY - sizeY, 0);
  glEnd();

  glBegin(GL_LINES);
  glColor3f(color[0], color[1], color[2]);
  glVertex3f(posX, posY - sizeY, 0);
  glVertex3f(posX, posY, 0);
  glEnd();
}

void renderFrame(Frame *frame) {
  // Format
  int width = 16;
  int height = 9;
  glfwGetWindowSize(window, &width, &height);
  float format = min(width / aspectRatio[0] -
                         (width % (int)aspectRatio[0]) / aspectRatio[0],
                     height / aspectRatio[1] -
                         (height % (int)aspectRatio[1]) / aspectRatio[1]);

  // Calcul positions / sizes
  float sizeX = frame->sizeX * (format / width) * aspectRatio[0];
  float sizeY = frame->sizeY * (format / height) * aspectRatio[0];
  float posX = (camPos[0] + frame->posX) * (format / width) * aspectRatio[0];
  float posY = (camPos[1] + frame->posY) * (format / height) * aspectRatio[0];

  // Display it
  if (frame->backgroundColor[0] != -1)
    renderFilledSquare(posX, posY, sizeX, sizeY, frame->backgroundColor);
  if (frame->borderColor[0] != -1)
    renderEmptySquare(posX, posY, sizeX, sizeY, frame->borderColor);
}
