#include "TextLabel.h"

// Imported Global variables
extern float camPos[2];
extern float aspectRatio[2];

TextLabel *createTextLabel(char *text, Font *font, float posX, float posY,
                           float sizeX, float sizeY) {
  // text entry is empty
  if (text == NULL)
    text = "";

  // Create the TextLabel
  TextLabel *textLabel = (TextLabel *)malloc(sizeof(TextLabel));
  if (textLabel == NULL)
    return NULL;

  // inherits
  textLabel->frame = createFrame(posX, posY, sizeX, sizeY);
  if (textLabel->frame == NULL) {
    free(textLabel);
    return NULL;
  }

  // Copy to textlabel
  textLabel->font = font;

  // Change text of the textLabel
  textLabel->text = NULL;
  int res = changeTextOfTextLabel(textLabel, text);
  if (res == -1) {
    free(textLabel->frame);
    free(textLabel);
    return NULL;
  }

  return textLabel;
}

int changeTextOfTextLabel(TextLabel *textLabel, char *text) {
  // Verify entries
  if (textLabel == NULL || text == NULL)
    return -1;

  // float totalXTextSize = 0; // TODODOODOODODOODODOODODOODODOODODOODO

  // ----- TEMP for one line -------

  // Remove last line if exist
  if (textLabel->text != NULL)
    free(textLabel->text);

  // Copy new line
  textLabel->text = strdup(text);

  // Calcul the text's size
  float size = textLabel->frame->sizeX;
  int nbLetters = strlen(text);
  textLabel->size = size * 370 / nbLetters;

  // float sizeX = size / 16;
  // float sizeY = size / 9;
  return 0;
}

void changeTextLabelBorderColor(TextLabel *textLabel, float r, float g,
                                float b) {
  // Verif entry
  if (textLabel == NULL)
    return;

  changeFrameBorderColor(textLabel->frame, r, g, b);
}

void changeTextLabelBackgroundColor(TextLabel *textLabel, float r, float g,
                                    float b) {
  // Verif entry
  if (textLabel == NULL)
    return;

  changeFrameBackgroundColor(textLabel->frame, r, g, b);
}

void getTransformTextLabel(TextLabel *textLabel, float *posX, float *posY,
                           float *sizeX, float *sizeY) {
  // Verif entry
  if (textLabel == NULL)
    return;

  getTransformFrame(textLabel->frame, posX, posY, sizeX, sizeY);
}

void renderTextLabel(TextLabel *textLabel) {
  // Verif entry
  if (textLabel == NULL)
    return;

  // Format
  int width = 16;
  int height = 9;
  glfwGetWindowSize(textLabel->font->window, &width, &height);
  float format = min(width / aspectRatio[0] -
                         (width % (int)aspectRatio[0]) / aspectRatio[0],
                     height / aspectRatio[1] -
                         (height % (int)aspectRatio[1]) / aspectRatio[1]);

  // Variables for calculs
  float tmp = 16;
  float posX = textLabel->frame->posX * (format / width) * tmp;
  float posY = textLabel->frame->posY * (format / height) * tmp;

  // Render frame
  renderFrame(textLabel->frame);

  // Camera offset
  posX += camPos[0];
  posY += camPos[1];

  // Render text
  int size = textLabel->size;
  char *text = textLabel->text;
  if (text == NULL)
    return;
  renderText(textLabel->font, text, posX, posY, size);
}

void freeTextLabel(TextLabel *textLabel) {
  // Verif entry
  if (textLabel == NULL)
    return;

  // Delete components
  if (textLabel->text != NULL)
    free(textLabel->text);

  freeFrame(textLabel->frame);

  // Free main struct
  free(textLabel);
}
