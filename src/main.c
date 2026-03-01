// #include "../lib/mainUOG.h"
#include "../libSrc/mainUOG.h" // Temp because I'm creating the libSrc
#include <stdio.h>

#define FRAME_TIME 1/60.

void testButtonClick();

int main() {
  int windowWidth = 1600;
  int windowHeight = 900;

  Window *window = initOpenGL(windowWidth, windowHeight);

  // Load font
  Font *font = loadFont("jbmReg.ttf", window);

  // Create textLabels and Frames
  Frame *frame0 = createFrame(0, 0, 1920, 1080, font);
  TextLabel *textLabel0 = createTextLabel("Ceci est un texte de teste !!", font,
                                          100, 400, 300, 300);

  TextLabel *textLabel1 =
      createTextLabel("Ce un texte deste !!", font, 1000, 700, 700, 320);
  changeTextLabelBorderColor(textLabel1, 1, 0, 0);
  changeTextLabelBackgroundColor(textLabel1, 0.2f, 0.1f, 0.1f);
  Button *button1 =
      createButton(textLabel1, BUTTON_TYPE_TEXTLABEL, testButtonClick);

  // Main Loop
  while (!glfwWindowShouldClose(window)) {

    double *deltaTime = getDeltaTime();
    if (*deltaTime >= FRAME_TIME) {
      *deltaTime -= FRAME_TIME;
      // printf("deltaTime %f\n", *deltaTime);

      // Render
      startRender(window, 0.1f, 0.1f, 0.1f, 0.1f);

      renderFrame(frame0);
      renderTextLabel(textLabel0);
      renderButton(button1);

      finishRender(window);
    }

    listenEvents();
  }

  // Exit the program
  freeFont(font);
  freeFrame(frame0);
  freeTextLabel(textLabel0);
  // freeTextLabel(textLabel1);
  freeButton(button1);

  exitOpenGL(window);
  return 0;
}

void testButtonClick() {
  printf("Button Clicked\n");
  moveCamX(20);
  moveCamY(50);
}
