#include "Global.h"

SDL_AppResult SDL_AppIterate(void *appstate) {
  struct GlobalState *state = appstate;

  glClearColor(0.1f, 0.1f, 0.1f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  mat4 model, view, persp, mvp;
  glm_mat4_identity(mvp);
  glm_mat4_identity(model);
  glm_mat4_identity(view);
  glm_mat4_identity(persp);
  
  glm_perspective(glm_rad(90), ((float)VW) / ((float)VH), 0.1f, 100.f, persp);

  vec3 rot, pos;
  versor quat;
  rot[0] = 0.f;
  rot[1] = 0.f;
  rot[2] = 0.f;
  glm_euler_yxz_quat(rot, quat);
  glm_quat_look(pos, quat, view);

  glm_translate_z(model, -50.f);

  glm_mat4_mul(view, model, mvp);
  glm_mat4_mul(persp, mvp, mvp);

  //glm_bezier();
  glUniformMatrix4fv(state->mvpPos, 1, GL_FALSE, (float*)mvp);;
  glDrawArrays(GL_TRIANGLES, 0, 6*2*3);

  SDL_GL_SwapWindow(state->win);
  return SDL_APP_CONTINUE;
}
