#include "Frame.h"

// Imported Global variables
extern int screenSize[2];
extern float camPos[2];

Frame *createFrame(float posX, float posY, float sizeX, float sizeY,
                   Font *font) {
  // Verif entry
  if (font == NULL)
    return NULL;

  // Create the Frame
  Frame *frame = (Frame *)malloc(sizeof(Frame));
  if (frame == NULL)
    return NULL;

  int width = screenSize[0];
  int height = screenSize[1];
  // Copy argument to textLabel's var
  frame->posX = posX / width * 2.0f - 1;
  frame->posY = -posY / width * 2.0f + 9 / 16.f;
  frame->sizeX = (sizeX / width * 2);
  frame->sizeY = (sizeY / width * 2);

  frame->font = font;

  // Default color
  frame->backgroundColor[0] = -1;
  frame->borderColor[0] = 1;
  frame->borderColor[1] = 1;
  frame->borderColor[2] = 1;

  return frame;
}

void changeFrameBorderColor(Frame *frame, float r, float g, float b) {
  // Verif entry
  if (frame == NULL)
    return;

  // Change color
  frame->borderColor[0] = r;
  frame->borderColor[1] = g;
  frame->borderColor[2] = b;
}

void changeFrameBackgroundColor(Frame *frame, float r, float g, float b) {
  // Verif entry
  if (frame == NULL)
    return;

  // Change color
  frame->backgroundColor[0] = r;
  frame->backgroundColor[1] = g;
  frame->backgroundColor[2] = b;
}

void getTransformFrame(Frame *frame, float *posX, float *posY, float *sizeX,
                       float *sizeY) {
  // Verif entry
  if (frame == NULL)
    return;

  *posX = frame->posX;
  *posY = frame->posY;
  *sizeX = frame->sizeX;
  *sizeY = frame->sizeY;
}

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
  glfwGetWindowSize(frame->font->window, &width, &height);
  float format =
      min(width / 16. - (width % 16) / 16., height / 9. - (height % 9) / 9.);

  // Calcul positions / sizes
  int tmp = 16;
  float sizeX = frame->sizeX * (format / width) * tmp;
  float sizeY = frame->sizeY * (format / height) * tmp;
  float posX = frame->posX * (format / width) * tmp;
  float posY = frame->posY * (format / height) * tmp;

  // Camera offset
  posX += camPos[0];
  posY += camPos[1];

  // Display it
  if (frame->backgroundColor[0] != -1)
    renderFilledSquare(posX, posY, sizeX, sizeY, frame->backgroundColor);
  if (frame->borderColor[0] != -1)
    renderEmptySquare(posX, posY, sizeX, sizeY, frame->borderColor);
}

void freeFrame(Frame *frame) {
  // Verif entry
  if (frame == NULL)
    return;

  // Free main struct
  free(frame);
}
