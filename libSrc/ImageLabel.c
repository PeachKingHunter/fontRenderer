#include "ImageLabel.h"

// Imported Global variables
extern float camPos[2];
extern float aspectRatio[2];
extern GLFWwindow *window;

ImageLabel *createImageLabel(const char *imagePath, float posX, float posY,
                             float sizeX, float sizeY) {
  // Create the ImageLabel
  ImageLabel *imageLabel = (ImageLabel *)malloc(sizeof(ImageLabel));
  if (imageLabel == NULL) {
    printf("Error fn: createImageLabel\n");
    return NULL;
  }

  // inherits
  imageLabel->frame = createFrame(posX, posY, sizeX, sizeY);
  if (imageLabel->frame == NULL) {
    printf("Error fn: createImageLabel\n");
    free(imageLabel);
    return NULL;
  }

  // Load image
  if (loadImage(imageLabel, imagePath) == -1) {
    imageLabel->imgData = NULL;
    freeImageLabel(imageLabel);
    printf("Error fn: createImageLabel\n");
    return NULL;
  }

  glGenTextures(1, &(imageLabel->textureID));

  return imageLabel;
}

int loadImage(ImageLabel *imageLabel, const char *imagePath) {
  FILE *file = fopen(imagePath, "r");
  if (file == NULL) {
    printf("Error fn: loadImage() -> file open\n");
    return -1;
  }

  // Get image size
  fseek(file, 18, SEEK_SET); // width pos in BMP
  int32_t imgSize[2];
  fread(imgSize, 4, 2, file);
  printf("size: %d %d\n", imgSize[0], imgSize[1]);

  // Get nbBytesPerPixel
  fseek(file, 28, SEEK_SET);
  int16_t nbBytesPerPixel;
  fread(&nbBytesPerPixel, 2, 1, file);
  nbBytesPerPixel /= 8;
  printf("Number of bytes per pixel: %d\n", nbBytesPerPixel);

  // Get data position
  fseek(file, 10, SEEK_SET);
  int32_t dataPos;
  fread(&dataPos, 4, 1, file);
  printf("Data starting position: %d\n", dataPos);

  // Get pos of datas
  int nbPixels = imgSize[0] * imgSize[1] * nbBytesPerPixel;
  fseek(file, dataPos, SEEK_SET);
  unsigned char *data =
      (unsigned char *)malloc(sizeof(unsigned char) * nbPixels);
  if (data == NULL) {
    fclose(file);
    printf("Error fn: loadImage() -> data malloc\n");
    return -1;
  }

  // Read datas
  for (int y = 0; y < imgSize[1]; y++) {
    for (int x = 0; x < imgSize[0]; x++) {
      int pos = (imgSize[0] - x) * 3 + 3 * y * imgSize[0];
      pos = 3 * imgSize[1] * imgSize[0] - pos;
      fread(data + pos, 1, nbBytesPerPixel, file);
    }
    int pos = ftell(file);
    if (pos % 4 != 0 && (imgSize[0] * nbBytesPerPixel) % 4 != 0) {
      fseek(file, 4 - pos % 4, SEEK_CUR);
    }
  }

  // Copy collected informations to imageLabel
  imageLabel->imgData = data;
  imageLabel->imgSize[0] = imgSize[0];
  imageLabel->imgSize[1] = imgSize[1];

  // End of the function
  fclose(file);
  return 1;
}

void changeImageLabelBorderColor(ImageLabel *imageLabel, float r, float g,
                                 float b) {
  // Verif entry
  if (imageLabel == NULL) {
    printf("Error fn: changeImageLabelBorderColor\n");
    return;
  }

  changeFrameBorderColor(imageLabel->frame, r, g, b);
}

void changeImageLabelBackgroundColor(ImageLabel *imageLabel, float r, float g,
                                     float b) {
  // Verif entry
  if (imageLabel == NULL) {
    printf("Error fn: changeImageLabelBackgroundColor\n");
    return;
  }

  changeFrameBackgroundColor(imageLabel->frame, r, g, b);
}

// Transform
void movePosXImageLabel(ImageLabel *imageLabel, int moveInX, int movementType) {
  movePosXFrame(imageLabel->frame, moveInX, movementType);
}

void movePosYImageLabel(ImageLabel *imageLabel, int moveInY, int movementType) {
  movePosYFrame(imageLabel->frame, moveInY, movementType);
}

void getTransformImageLabel(ImageLabel *imageLabel, float *posX, float *posY,
                            float *sizeX, float *sizeY) {
  // Verif entry
  if (imageLabel == NULL) {
    printf("Error fn: getTransformImageLabel\n");
    return;
  }

  getTransformFrame(imageLabel->frame, posX, posY, sizeX, sizeY);
}

void renderImageLabel(ImageLabel *imageLabel) {
  // Verif entry
  if (imageLabel == NULL) {
       printf("Error fn: renderImageLabel\n");
 return;
  }

  // Format
  int width = 16;
  int height = 9;
  glfwGetWindowSize(window, &width, &height);
  float format = min(width / aspectRatio[0] -
                         (width % (int)aspectRatio[0]) / aspectRatio[0],
                     height / aspectRatio[1] -
                         (height % (int)aspectRatio[1]) / aspectRatio[1]);

  // Variables for calculs
  float tmp = aspectRatio[0];
  float sizeX = imageLabel->frame->sizeX * (format / width) * aspectRatio[0];
  float sizeY = imageLabel->frame->sizeY * (format / height) * aspectRatio[0];
  float posX = (camPos[0] + imageLabel->frame->posX) * (format / width) * tmp;
  float posY = (camPos[1] + imageLabel->frame->posY) * (format / height) * tmp;

  // ---- Render image ----
  // Create Texture

  // Setting Texture
  glBindTexture(GL_TEXTURE_2D, imageLabel->textureID);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageLabel->imgSize[0],
               imageLabel->imgSize[1], 0, GL_BGR, GL_UNSIGNED_BYTE,
               imageLabel->imgData);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  // Render texture
  glEnable(GL_TEXTURE_2D);

  glBegin(GL_QUADS);
  glColor3f(1, 1, 1);
  glTexCoord2f(0.0f, 0.0f);
  glVertex2f(posX, posY);

  glTexCoord2f(1.0f, 0.0f);
  glVertex2f(posX + sizeX, posY);

  glTexCoord2f(1.0f, 1.0f);
  glVertex2f(posX + sizeX, posY - sizeY);

  glTexCoord2f(0.0f, 1.0f);
  glVertex2f(posX, posY - sizeY);
  glEnd();

  glDisable(GL_TEXTURE_2D);

  // ---- Render frame ----
  renderFrame(imageLabel->frame);
}

void freeImageLabel(ImageLabel *imageLabel) {
  // Verif entry
  if (imageLabel == NULL)
    return;

  // Delete components
  if (imageLabel->imgData != NULL)
    free(imageLabel->imgData);

  if (imageLabel->frame != NULL)
    freeFrame(imageLabel->frame);

  // Free main struct
  free(imageLabel);
}
