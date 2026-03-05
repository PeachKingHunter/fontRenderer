#ifndef MainUOG_H
#define MainUOG_H

#include "GLFW/glfw3.h"
#include <GL/gl.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/time.h>

#include "Button.h"
#include "fontManager/FontLoader.h"

typedef GLFWwindow Window;

// Mouse
// static void cursorPosCallback(GLFWwindow *window, double xpos, double ypos);
// static void mouseButtonCallback(GLFWwindow *window, int button, int action,
//                                 int mods);

// Init / Loop / End
int initOpenGL(int windowWidth, int windowHeight);
int shouldNotStopMainLoop();
void startRender(float bgColorR, float bgColorG, float bgColorB, float bgAlpha);
void listenEvents();
void finishRender();
void exitOpenGL();

// Move Cam
void moveCamX(int nbPixel);
void moveCamY(int nbPixel); 

// Other (time curently)
double getCurrentTime();
double *getDeltaTime();

#endif
