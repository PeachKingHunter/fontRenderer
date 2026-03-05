// #include "../lib/mainUOG.h"
#include "../libSrc/mainUOG.h" // Temp because I'm creating the libSrc

#define FRAME_TIME 1 / 60.

void testButtonClick();

int main() {
  int windowWidth = 1920;
  int windowHeight = 1080;

  initOpenGL(windowWidth, windowHeight);

  // Load font (After initOpenGL)
  Font *font = loadFont("jbmReg.ttf");

  // Create textLabels and Frames
  Frame *frame0 = createFrame(0, 0, 1920, 1080);
  TextLabel *textLabel0 = createTextLabel("Ceci est un texte de teste !!", font,
                                          100, 400, 300, 300);

  // Warning the imageTest.bmp is not on github
  // -> In this case, it don't appear (the ImageLabel)
  ImageLabel *imageLabel0 =
      createImageLabel("imageTest.bmp", 577, 477, 333, 222);

  TextLabel *textLabel1 =
      createTextLabel("Ce un texte deste !!", font, 1000, 700, 700, 320);
  changeTextLabelBorderColor(textLabel1, 1, 0, 0);
  changeTextLabelBackgroundColor(textLabel1, 0.2f, 0.1f, 0.1f);
  Button *button1 =
      createButton(textLabel1, BUTTON_TYPE_TEXTLABEL, testButtonClick);

  // Main Loop
  while (shouldNotStopMainLoop()) {

    double *deltaTime = getDeltaTime();
    if (*deltaTime >= FRAME_TIME) {
      *deltaTime -= FRAME_TIME;
      // printf("deltaTime %f\n", *deltaTime);

      // Render
      startRender(0.1f, 0.1f, 0.1f, 0.1f);

      renderFrame(frame0);
      renderTextLabel(textLabel0);
      renderButton(button1);
      renderImageLabel(imageLabel0);

      finishRender();
    }

    listenEvents();
  }

  // Exit the program
  freeFont(font);
  freeFrame(frame0);
  freeTextLabel(textLabel0);
  // freeTextLabel(textLabel1);
  freeButton(button1);
  freeImageLabel(imageLabel0);

  exitOpenGL();
  return 0;
}

void testButtonClick() {
  printf("Button Clicked\n");
  moveCamX(20);
  moveCamY(50);
}
