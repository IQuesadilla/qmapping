#ifndef QMAPPING_GLOBAL_H
#define QMAPPING_GLOBAL_H

#include <SDL3/SDL.h>
//#include <SDL3/SDL_opengles2.h>
#include <GL/glew.h>
#include <cglm/cglm.h>
//#include <apr.h>

#define VW 640
#define VH 480
#define I 1.0f

static const float Steps[][3][3] = {
  {{-.5, 0.0, -.5}, {0.0, 0.0, -.5}, {0.5, 0.0, -.5}},
  {{0.5, 0.0, -.5}, {0.5, 0.0, 0.0}, {0.5, 0.0, 0.5}},
  {{0.5, 0.0, 0.5}, {0.0, 0.0, 0.5}, {-.5, 0.0, 0.5}},
  {{-.5, 0.0, 0.5}, {-.5, 0.0, 0.0}, {-.5, 0.0, -.5}},
};

struct GlobalState {
  SDL_Window *win;
  SDL_GLContext ctx;
  GLuint VBO, shader, mvpPos;
  SDL_Time pframe, dT;
  float dA;
  int iA;
};

static const float vertData[] = {
    -I, I,-I,
    -I, I, I,
     I, I,-I,
     I, I,-I,
    -I, I, I,
     I, I, I,

     I, I,-I,
     I,-I,-I,
    -I, I,-I,
    -I, I,-I,
     I,-I,-I,
    -I,-I,-I,

    -I, I,-I,
    -I,-I,-I,
    -I, I, I,
    -I, I, I,
    -I,-I,-I,
    -I,-I, I,

    -I, I, I,
    -I,-I, I,
     I, I, I,
     I, I, I,
    -I,-I, I,
     I, -I,I,

     I, I, I,
     I,-I, I,
     I, I,-I,
     I, I,-I,
     I,-I, I,
     I,-I,-I,

     I,-I,-I,
     I,-I, I,
    -I,-I,-I,
    -I,-I,-I,
     I,-I, I,
    -I,-I, I,
};

#endif
