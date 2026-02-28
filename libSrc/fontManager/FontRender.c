#include "FontRender.h"
#include <GLFW/glfw3.h>
#include <stdio.h>

// Render a simple glyph
float renderSimpleGlyph(SimpleGlyph *glyph, float offsetX, float offsetY,
                       float sizeX, float sizeY) {
  // Verif Entry
  if (glyph == NULL)
    return -1;

  // Variables
  int n = glyph->nbPointContours[glyph->nbContours - 1] + 1;
  int indexContours = 0;
  int startContourIndex = 0;
  float posX1 = 0;
  float posY1 = 0;
  float posX2 = 0;
  float posY2 = 0;

  // Render Loop
  for (int i = 0; i < n; i++) {
    // Calcul points's pos
    if (i == glyph->nbPointContours[indexContours]) {
      posX1 = offsetX + (float)glyph->posX[i] * sizeX;
      posY1 = offsetY + (float)glyph->posY[i] * sizeY;
      posX2 = offsetX + (float)glyph->posX[startContourIndex] * sizeX;
      posY2 = offsetY + (float)glyph->posY[startContourIndex] * sizeY;

      startContourIndex = i + 1;
      indexContours++;

    } else {
      posX1 = offsetX + (float)glyph->posX[i] * sizeX;
      posY1 = offsetY + (float)glyph->posY[i] * sizeY;
      posX2 = offsetX + (float)glyph->posX[i + 1] * sizeX;
      posY2 = offsetY + (float)glyph->posY[i + 1] * sizeY;
    }

    // Render
    glBegin(GL_LINES);
    glColor3f(1.0f, 0, 0);
    glVertex3f(posX1, posY1, 0);
    glVertex3f(posX2, posY2, 0);
    glEnd();
  }

  return (glyph->xMax - glyph->xMin) * sizeX;
}

float min(float a, float b) {
  if (a < b)
    return a;
  return b;
}

float max(float a, float b) {
  if (a < b)
    return b;
  return a;
}

// Render a text
void renderText(void *font, char *text, float posX, float posY, float size) {
  // Verif Entries
  if (font == NULL || text == NULL)
    return;

  // Size of text in format 16x9
  int width = 16;
  int height = 9;
  glfwGetWindowSize(((Font *) font)->window, &width, &height);
  float format = min(width/16. - (width % 16)/16., height/9. - (height % 9)/9.);

  size /= 10000;
  float sizeX = size * (format / width);
  float sizeY = size * (format / height);

  // Variables
  Font *theFont = (Font *)font;
  int strLen = strlen(text);

  // Get text size
  float totalSize = 0;
  for (int i = 0; i < strLen; i++) {
    // printf("val: %d et %c\n",text[i], text[i]);
    int index = theFont->unicodeToIndexTab[(int)text[i]];
    if (index == -1 || text[i] == ' ')
      continue;

    SimpleGlyph *simpleGlyph = theFont->simpleGlyphs[index];
    totalSize += (float)(simpleGlyph->xMax - simpleGlyph->xMin);
  }
  totalSize *= sizeX;

  // Render Loop for all characters
  float offset = 0;
  for (int i = 0; i < strLen; i++) {
    // printf("val: %d et %c\n",text[i], text[i]);
    int index = theFont->unicodeToIndexTab[(int)text[i]];

    if (index == -1)
      continue;

    if (text[i] == ' ') {
      offset += 400 * sizeX;
      continue;
    }

    SimpleGlyph *simpleGlyph = theFont->simpleGlyphs[index];
    offset += renderSimpleGlyph(simpleGlyph, offset + posX, 0 + posY - sizeY*1000, sizeX, sizeY);
    offset += 0.002f;
  }
}
