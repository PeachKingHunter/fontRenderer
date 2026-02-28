#include "mainUOG.h"
#include <sys/time.h>

// Imported Global variables
extern List *buttonsList;

// Variables globales
static float mousePos[2] = {0, 0};
float camPos[2] = {0, 0};
static int screenSize[2] = {1920, 1080};

static double deltaTime = 0;
static double lastTime = 0;

static void cursorPosCallback(GLFWwindow *window, double xpos, double ypos) {
  if (window == NULL)
    return;

  // Format
  int width = 16;
  int height = 9;
  glfwGetWindowSize(window, &width, &height);

  // Calcul normalized pos
  float posX = xpos / width * 2.0f - 1;
  float posY = -ypos / height * 2.0f + 1;

  mousePos[0] = posX;
  mousePos[1] = posY;
}

static void mouseButtonCallback(GLFWwindow *window, int button, int action,
                                int mods) {
  if (button == GLFW_MOUSE_BUTTON_LEFT) {
    if (action == GLFW_RELEASE) {
      // printf("mouse <%f, %f>\n", mousePos[0], mousePos[1]);

      // Format
      int width = 16;
      int height = 9;
      glfwGetWindowSize(window, &width, &height);
      float format = min(width / 16. - (width % 16) / 16.,
                         height / 9. - (height % 9) / 9.);

      // TODO -> Reajust pos of mouse for button clicking
      // Variables for calculs
      float tmp = 16;
      float posX = mousePos[0] / (format / width) / tmp;
      float posY = mousePos[1] / (format / height) / tmp;

      // Camera offset
      posX -= camPos[0];
      posY -= camPos[1];

      // Click on all buttons
      List *tmpHead = buttonsList;
      while (tmpHead != NULL) {
        clickOnButton(tmpHead->data, posX, posY);
        tmpHead = tmpHead->next;
      }
    }
  }
}

Window *initOpenGL(int windowWidth, int windowHeight) {
  // Init GLFW
  if (!glfwInit()) {
    printf("Error Init");
    return NULL;
  }

  // Create the window
  GLFWwindow *window =
      glfwCreateWindow(windowWidth, windowHeight, "Ma Fenetre", NULL, NULL);
  if (window == NULL) {
    printf("Error Window Creation");
    glfwTerminate();
    return NULL;
  }
  glfwMakeContextCurrent(window);

  // Set callbacks
  glfwSetCursorPosCallback(window, cursorPosCallback);
  glfwSetMouseButtonCallback(window, mouseButtonCallback);

  // Init time for delta time calculation
  lastTime = getCurrentTime();

  return window;
}

void startRender(Window *window, float bgColorR, float bgColorG,
                   float bgColorB, float bgAlpha) {
  // Verif entry
  if (window == NULL)
    return;

  // Varialbes
  int windowWidth;
  int windowHeight;

  // Clear the buffer
  glClearColor(bgColorR, bgColorG, bgColorB, bgAlpha);
  glClear(GL_COLOR_BUFFER_BIT);

  // Update Viewport
  glfwGetWindowSize(window, &windowWidth, &windowHeight);
  glViewport(0, 0, windowWidth, windowHeight);
}

void listenEvents() { glfwPollEvents(); }

void finishRender(Window *window) {
  // Verif entry
  if (window == NULL)
    return;

  // Swap buffer for display next render
  glFlush();
  glfwSwapBuffers(window);
}

void exitOpenGL(Window *window) {
  if (window != NULL)
    glfwDestroyWindow(window);
  glfwTerminate();
}

void moveCamX(int nbPixel) { camPos[0] += (float)nbPixel / screenSize[0]; }
void moveCamY(int nbPixel) { camPos[1] -= (float)nbPixel / screenSize[1]; }

double getCurrentTime() {
  struct timeval tv;
  gettimeofday(&tv, NULL);

  return tv.tv_sec + tv.tv_usec / 1000000.;
}

double *getDeltaTime() {
  double currentTime = getCurrentTime();
  deltaTime += currentTime - lastTime;
  lastTime = currentTime;

  return &deltaTime;
}
