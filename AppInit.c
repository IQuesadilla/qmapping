#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL_main.h>
#include "Global.h"

bool checkCompileErrors(GLuint shader, char *type, bool isProgram)
{
    GLint success = true;
    GLchar infoLog[1024];
    if(!isProgram)
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            SDL_Log("ERROR::SHADER_COMPILATION_ERROR of type: %s -> %s", type, infoLog);
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            SDL_Log("ERROR::SHADER_LINKING_ERROR of type: %s -> %s", type, infoLog);
        }
    }
    return !success;
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
  *appstate = NULL;

  SDL_Log("Preparing to init!\r\n");
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("SDL Init: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }
  struct GlobalState *state = SDL_malloc(sizeof(struct GlobalState));

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_ES,
                      SDL_GL_CONTEXT_PROFILE_ES);   

  state->win = SDL_CreateWindow(",,", VW * 2, VH, SDL_WINDOW_OPENGL);
  if (state->win == NULL) {
    SDL_Log("Window Create: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }
  state->ctx = SDL_GL_CreateContext(state->win);

  glewInit();
  
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_CULL_FACE);

  glGenBuffers(1, &state->VBO);
  glBindBuffer(GL_ARRAY_BUFFER, state->VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertData), vertData, GL_STATIC_DRAW);

  size_t vertsz, fragsz;
  char *vertsrc = SDL_LoadFile("shaders/basic.vert.glsl", &vertsz);
  char *fragsrc = SDL_LoadFile("shaders/basic.frag.glsl", &fragsz);

  GLuint vertsh = glCreateShader(GL_VERTEX_SHADER);
  const char *cvertsrc = vertsrc;
  const GLint cvertsz = vertsz;
  glShaderSource(vertsh, 1, &cvertsrc, &cvertsz);
  glCompileShader(vertsh);
  checkCompileErrors(vertsh, "VERTEX", false);

  GLuint fragsh = glCreateShader(GL_FRAGMENT_SHADER);
  const char *cfragsrc = fragsrc;
  const GLint cfragsz = fragsz;
  glShaderSource(fragsh, 1, &cfragsrc, &cfragsz);
  glCompileShader(fragsh);
  checkCompileErrors(vertsh, "FRAGMENT", false);

  state->shader = glCreateProgram();
  glAttachShader(state->shader, vertsh);
  glAttachShader(state->shader, fragsh);
  glLinkProgram(state->shader);
  checkCompileErrors(vertsh, "PROGRAM", true);

  glDeleteShader(vertsh);
  glDeleteShader(fragsh);

  SDL_free(vertsrc);
  SDL_free(fragsrc);

  glUseProgram(state->shader);

  GLint aPosLoc = glGetAttribLocation(state->shader, "aPos");
  glVertexAttribPointer(aPosLoc, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
  glEnableVertexAttribArray(aPosLoc);
  state->mvpPos = glGetUniformLocation(state->shader, "mvp");
  state->dA = 0.f;
  state->iA = 0;
  SDL_GetCurrentTime(&state->pframe);

  *appstate = state;
  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
}
