#ifndef QMAPPING_GLOBAL_H
#define QMAPPING_GLOBAL_H

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_opengles2.h>
#include <cglm/cglm.h>
#include <apr.h>

#define VW 640
#define VH 480

struct GlobalState {
  SDL_Window *win;
  SDL_GLContext ctx;
  GLuint VBO, shader, mvpPos;
};

#define I 1.0f

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
