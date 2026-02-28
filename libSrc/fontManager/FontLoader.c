#include "FontLoader.h"
#include "FileReader.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Obtain a certain tab in the .ttf (head, glyph, cmap, ...)
int getTabPos(FILE *file, const char *name) {
  if (file == NULL)
    return -1;

  // Get nb of tables
  fseek(file, 4, SEEK_SET);
  int nbTabs = readU16(file);
  fseek(file, 6, SEEK_CUR);

  // Interate Throught all tables
  for (int i = 0; i < nbTabs; i++) {
    // Read the table directory
    char *str = readChar(file, 4);
    readU32(file);
    uint32_t offset = readU32(file);
    readU32(file);

    // If Glyph table
    if (strcmp(str, name) == 0) {
      free(str);
      return offset;
    }
    free(str);
  }
  return -1;
}

// Read a flag in an interger (One bit)
int readFlag(uint8_t flag, int index) { return (flag >> index) & 1; }

// Obtain an array of the points's position on one axis for a glyph
int *getPosArray(FILE *file, int size, uint8_t *flags, int flagIndexSize,
                 int flagIndexShort) {
  int *pos = (int *)malloc(sizeof(int) * size);
  if (pos == NULL) {
    printf("Error getPosArray fonction");
    return NULL;
  }

  // For 'size' positions
  for (int i = 0; i < size; i++) {
    int xSize = readFlag(flags[i], flagIndexSize);

    if (xSize == 1) {
      // 1 byte
      pos[i] = readU8(file);
      if (readFlag(flags[i], flagIndexShort) == 0)
        pos[i] *= -1;

    } else {
      // 2 bytes
      if (readFlag(flags[i], flagIndexShort) == 1)
        pos[i] = 0;
      else
        pos[i] = readS16(file);
    }

    if (i > 0) {
      pos[i] += pos[i - 1];
    }
  }

  return pos;
}

// Get the how much glyphs are store in the ttf
int getNumberOfGlyphs(FILE *file) {
  // Verif entry
  if (file == NULL)
    return -1;

  // Table maxp
  uint32_t pos = getTabPos(file, "maxp");
  fseek(file, pos, SEEK_SET);

  // Skip version
  fseek(file, 4, SEEK_CUR);

  // get and return number of Glyphs
  int numGlyphs = (int)readU16(file);
  // printf("numGlyphs: %d\n", numGlyphs);
  return numGlyphs;
}

// Read a simple glyph
SimpleGlyph *readSimpleGlyph(FILE *file, int inFilePos) {
  // Verif entry
  if (file == NULL || inFilePos < 0)
    return NULL;

  // Create glyph struct
  SimpleGlyph *simpleGlyph = (SimpleGlyph *)malloc(sizeof(SimpleGlyph));
  if (simpleGlyph == NULL)
    return NULL;

  fseek(file, inFilePos, SEEK_SET);

  // Get Glyph's description
  simpleGlyph->nbContours = readS16(file);
  if (simpleGlyph->nbContours < 0) { // Verif if it's a simple glyph
    free(simpleGlyph);
    return NULL;
  }

  // printf("nbContours: %d\n");
  simpleGlyph->xMin = readS16(file);
  simpleGlyph->yMin = readS16(file);
  simpleGlyph->xMax = readS16(file);
  simpleGlyph->yMax = readS16(file);

  // Get glyph
  simpleGlyph->nbPointContours =
      malloc(sizeof(uint16_t) * simpleGlyph->nbContours);
  if (simpleGlyph->nbPointContours == NULL) {
    free(simpleGlyph);
    return NULL;
  }

  for (int i = 0; i < simpleGlyph->nbContours; i++) {
    simpleGlyph->nbPointContours[i] = readU16(file);
  }

  // Skip instructions
  uint16_t instruLength = readU16(file);
  fseek(file, instruLength, SEEK_CUR);

  // Get flags
  int n = (simpleGlyph->nbPointContours[simpleGlyph->nbContours - 1] +
           1); // 0 is in and nbPointContours also

  uint8_t *flags = (uint8_t *)malloc(sizeof(uint8_t) * n);
  if (flags == NULL) {
    free(simpleGlyph->nbPointContours);
    free(simpleGlyph);
    return NULL;
  }

  int flagIndex = 0;
  while (flagIndex < n) {
    uint8_t flag = readU8(file);
    flags[flagIndex++] = flag;

    if (flag & 0x08) { // Repeat flag
      uint8_t repeatCount = readU8(file);
      for (int j = 0; j < repeatCount && flagIndex < n; ++j) {
        flags[flagIndex++] = flag;
      }
    }
  }

  // Var for points
  simpleGlyph->posX = getPosArray(file, n, flags, 1, 4);
  simpleGlyph->posY = getPosArray(file, n, flags, 2, 5);

  free(flags);
  return simpleGlyph;
}

// free the structure of an simple glyph
void freeSimpleGlyph(SimpleGlyph *simpleGlyph) {
  if (simpleGlyph == NULL)
    return;

  free(simpleGlyph->posX);
  free(simpleGlyph->posY);
  free(simpleGlyph->nbPointContours);
  free(simpleGlyph);
}


int *getGlyphsEquivalences(FILE *file, int numGlyphs) {
  if (file == NULL)
    return NULL;

  // Glyph Unicode Equivalence
  uint32_t cmapPos = getTabPos(file, "cmap");
  if (cmapPos == -1)
    return NULL;
  fseek(file, cmapPos, SEEK_SET);

  /*uint16_t version =*/readU16(file);
  uint16_t nbSubtables = readU16(file);

  // Get all avaibles format
  uint32_t equivalenceTabOffset = UINT32_MAX;
  for (int i = 0; i < nbSubtables; i++) {
    uint16_t plateformID = readU16(file);
    uint16_t plateformSpeID = readU16(file);
    uint32_t offset = readU32(file);
    if (plateformID == 0 && plateformSpeID == 4)
      equivalenceTabOffset = offset;
  }

  if (equivalenceTabOffset == UINT32_MAX)
    return NULL;

  // Create the tab
  int *glyphsEquivalences = (int *)malloc(sizeof(int) * numGlyphs);
  if (glyphsEquivalences == NULL)
    return NULL;
  // Set default value to the tab
  for (int i = 0; i < numGlyphs; i++) {
    glyphsEquivalences[i] = -1;
  }

  // Move to the format
  fseek(file, cmapPos + equivalenceTabOffset, SEEK_SET);
  uint16_t format = readU16(file);
  printf("Format: %d\n", format);

  if (format == 12) {
    // Useless
    readU16(file);
    readU32(file);
    readU32(file);

    // Nb Groups
    uint32_t nbGroups = readU32(file);
    printf("nbGroups: %d\n", nbGroups);

    // For each groups, take index and unicode equivalence
    for (int i = 0; i < nbGroups; i++) {
      uint32_t startCharCode = readU32(file);
      uint32_t endCharCode = readU32(file);
      uint32_t startGlyphCode = readU32(file);

      int numChars = (int)(endCharCode - startCharCode) + 1;
      for (int offset = 0; offset < numChars; offset++) {
        uint32_t charCode = startCharCode + offset;
        uint32_t glyphIndex = startGlyphCode + offset;

        // Add into the tab
        // printf("index: %d, char: /%c|\n", (int)glyphIndex, (int)charCode);
        glyphsEquivalences[glyphIndex] = charCode;
      }
    }

  } else if (format == 4) {
    // TODO
    return NULL;

  } else {
    return NULL;
  }

  return glyphsEquivalences;
}

// Load Font
Font *loadFont(const char *fontPath, GLFWwindow *window) {
  if (fontPath == NULL)
    return NULL;

  // ---- General informations ----
  // Open the file
  FILE *file = fopen("jbmReg.ttf", "rb");
  if (file == NULL)
    return NULL;

  // Number of glyphs
  int numGlyphs = getNumberOfGlyphs(file);
  printf("nb glyphs: %d\n", numGlyphs);

  // Head get Loca format
  uint32_t headPos = getTabPos(file, "head"); // Table head
  fseek(file, headPos, SEEK_SET);
  fseek(file, 50, SEEK_CUR); // Skip useless data
  uint16_t locaFormat = readU16(file);
  // printf("format of loca: %d\n", locaFormat);

  // Get glyph's tab pos
  uint32_t glyphTabPos = getTabPos(file, "glyf");

  // Get glyphs's pos with loca Tab
  uint32_t locaPos = getTabPos(file, "loca"); // Table head

  // ---- Create the tab for all glyphs Pos ----
  fseek(file, locaPos, SEEK_SET);
  int *glyphsPos = (int *)malloc(sizeof(int) * numGlyphs);
  if (glyphsPos == NULL) {
    fclose(file);
    return NULL;
  }

  for (int i = 0; i < numGlyphs; i++) {
    if (locaFormat == 0)
      glyphsPos[i] = (int)readU16(file) * 2;
    else
      glyphsPos[i] = (int)readU32(file);
    glyphsPos[i] += glyphTabPos;
  }

  // ---- Create the font ----
  Font *font = (Font *)malloc(sizeof(Font));
  if (font == NULL) {
    fclose(file);
    free(glyphsPos);
    return NULL;
  }

  // Tab for all simples glyphs
  font->simpleGlyphsLen = 0;
  font->simpleGlyphs =
      (SimpleGlyph **)malloc(sizeof(SimpleGlyph *) * numGlyphs);
  if (font->simpleGlyphs == NULL) {
    fclose(file);
    free(glyphsPos);
    return NULL;
  }

  // "Unicode to index"'s equivalence tab
  font->unicodeToIndexTab = (int *)malloc(sizeof(int) * numGlyphs);
  if (font->unicodeToIndexTab == NULL) {
    free(font->simpleGlyphs);
    fclose(file);
    free(glyphsPos);
    return NULL;
  }
  for (int i = 0; i < numGlyphs; i++) {
    font->unicodeToIndexTab[i] = -1;
  }

  // Equivalence Tab
  int *equivalenceTab1 = getGlyphsEquivalences(file, numGlyphs);
  if (equivalenceTab1 == NULL) {
    free(font->simpleGlyphs);
    free(glyphsPos);
    fclose(file);
    return NULL;
  }

  // Loop throught all glyphs
  int index = 0;
  for (int i = 0; i < numGlyphs; i++) {
    long theGlyphPos = glyphsPos[i];

    SimpleGlyph *simpleGlyph = readSimpleGlyph(file, theGlyphPos);
    if (simpleGlyph == NULL)
      continue;

    font->simpleGlyphs[index] = simpleGlyph;
    if (equivalenceTab1[i] > 0 && equivalenceTab1[i] < numGlyphs) {
      font->unicodeToIndexTab[equivalenceTab1[i]] = index;
      // printf("index: %d is |%c|\n", index, equivalenceTab1[i]);
    }
    index++;
  }
  font->simpleGlyphsLen = index;
  font->window = window;

  //TODO REALLOC with index: font->simpleGlyphs =(SimpleGlyph **)malloc(sizeof(SimpleGlyph *) * numGlyphs);

  // Free used datas
  free(glyphsPos);
  free(equivalenceTab1);
  fclose(file);
  return font;
}

void freeFont(void *font) {
  // Get the struc if not NULL
  if (font == NULL)
    return;

  Font *theFont = (Font *)font;

  // Freing simples glyphs
  for (int i = 0; i < theFont->simpleGlyphsLen; i++) {
    freeSimpleGlyph(theFont->simpleGlyphs[i]);
  }

  // Free the structure
  free(theFont->simpleGlyphs);
  free(theFont->unicodeToIndexTab);
  free(theFont);
}
