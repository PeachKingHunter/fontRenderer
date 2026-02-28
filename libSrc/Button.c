#include "Button.h"

// Global var
List *buttonsList = NULL;

// WARNING COMPONENT IS FREE BY BUTTON WHEN NEEDED
Button *createButton(void *component, int componentType, void(*onClicFunc)()) {
  // Verif entry
  if (component == NULL || onClicFunc == NULL)
    return NULL;

  Button *button = (Button *)malloc(sizeof(Button));
  if (button == NULL)
    return NULL;

  button->component = component;
  button->componentType = componentType;
  button->onClickFunc = onClicFunc;

  addToList(&buttonsList, button);

  return button;
}

void clickOnButton(Button *button, float mousePosX, float mousePosY) {
  // Verif entry
  if (button == NULL)
    return;

  // Get transform
  float posX, posY, sizeX, sizeY;
  if (button->componentType == BUTTON_TYPE_FRAME)
    getTransformFrame(button->component, &posX, &posY, &sizeX, &sizeY);

  if (button->componentType == BUTTON_TYPE_TEXTLABEL)
    getTransformTextLabel(button->component, &posX, &posY, &sizeX, &sizeY);

  // Verif if mouseClick in it
  if (mousePosX < posX)
    return;
  
  if (mousePosY > posY)
    return;

  if (mousePosX > posX + sizeX)
    return;

  if (mousePosY < posY - sizeY)
    return;

  // Call onClickFunc
  (*(button->onClickFunc))();
}

void renderButton(Button *button){
  // Verif entry
  if (button == NULL)
    return;

  if (button->componentType == BUTTON_TYPE_TEXTLABEL)
    renderTextLabel(button->component);

  if (button->componentType == BUTTON_TYPE_FRAME)
    renderFrame(button->component);
}

// WARNING COMPONENT IS FREE BY BUTTON
void freeButton(Button *button) {
  // Verif entry
  if (button == NULL)
    return;

  // Remove from list
  removeFromList(&buttonsList, button);

  if (button->componentType == BUTTON_TYPE_TEXTLABEL)
    freeTextLabel(button->component);

  if (button->componentType == BUTTON_TYPE_FRAME)
    freeFrame(button->component);

  free(button);
}
