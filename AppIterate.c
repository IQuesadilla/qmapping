#include "Global.h"

SDL_AppResult SDL_AppIterate(void *appstate) {
  struct GlobalState *state = appstate;
  
  SDL_Time cframe;
  SDL_GetCurrentTime(&cframe);
  state->dT = cframe - state->pframe;
  state->pframe = cframe;

  glClearColor(0.f, 0.f, 0.f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  mat4 model, view, persp, mvp;
  glm_mat4_zero(mvp);
  glm_mat4_identity(model);
  glm_mat4_zero(view);
  glm_mat4_zero(persp);
  
  glm_perspective(glm_rad(90), ((float)VW * 2) / ((float)VH), 0.0001f, 1.f, persp);

  state->dA += 0.000000001f * ((float)(state->dT));

  vec3 rot, pos, up, center;
  versor quat;
  rot[0] = 0.f;
  rot[1] = 0.f;
  rot[2] = 0.f;
  glm_vec3_zero(center);
  pos[0] = 0.5f; //-1.f;
  pos[1] = 0.f;
  pos[2] = 0.f;
  up[0] = pos[0];
  up[1] = pos[1] + 10.f;
  up[2] = pos[2];

  //glm_translate_z(model, -5.f);
  //glm_rotate_y(model, state->rot, model);
  glm_scale_uni(model, 0.1f);

  //glm_euler_yxz_quat(rot, quat);
  //glm_quat_look(pos, quat, view);
  //glm_mat4_mulv3(model, pos, 1.f, pos);
  glm_lookat(pos, rot, up, view);

  glm_mat4_mul(view, model, mvp);
  glm_mat4_mul(persp, mvp, mvp);

  //glm_bezier();
  glUniformMatrix4fv(state->mvpPos, 1, GL_FALSE, &(mvp[0][0]));
  glDrawArrays(GL_TRIANGLES, 0, 6*2*3);

  SDL_GL_SwapWindow(state->win);
  return SDL_APP_CONTINUE;
}
