#ifndef Button_H
#define Button_H

#include "Frame.h"
#include "List.h"
#include "TextLabel.h"
#include "ImageLabel.h"

#include <GL/gl.h>
#include <stdlib.h>

#define BUTTON_TYPE_TEXTLABEL 0
#define BUTTON_TYPE_FRAME 1
#define BUTTON_TYPE_IMAGELABEL 2 // TODO LATER

typedef struct button {
  // Inherits a graphic component
  void *component;
  int componentType;

  void (*onClickFunc)();
} Button;

Button *createButton(void *component, int componentType, void (*onClicFunc)());
void clickOnButton(Button *button, float posX, float posY);
void renderButton(Button *button);
void freeButton(Button *button);

#endif
