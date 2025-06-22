#include "Global.h"

SDL_AppResult SDL_AppIterate(void *appstate) {
  struct GlobalState *state = appstate;
  
  SDL_Time cframe;
  SDL_GetCurrentTime(&cframe);
  state->dT = cframe - state->pframe;
  state->pframe = cframe;

  //glBindFramebuffer(GL_FRAMEBUFFER, state->FB);
  glClearColor(0.f, 0.f, 0.f, 0.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  mat4 model, view, persp, mvp;
  glm_mat4_zero(mvp);
  glm_mat4_identity(model);
  glm_mat4_zero(view);
  glm_mat4_zero(persp);
  
  glm_perspective(glm_rad(90), ((float)VW) / ((float)VH), 0.0001f, 1.f, persp);

  state->dA += 0.000000001f * ((float)(state->dT));
  if (state->dA > 1.f) {
    state->dA = 0.f;
    state->iA = (state->iA + 1) % (sizeof(Steps) / sizeof(Steps[0]));
  }
  int iA = state->iA;
  int nA = (iA + 1) % (sizeof(Steps) / sizeof(Steps[0]));

  vec3 rot, pos, up, center;
  versor quat;
  rot[0] = 0.f;
  rot[1] = 0.f;
  rot[2] = 0.f;
  glm_vec3_zero(center);
  pos[0] = glm_bezier(state->dA,
                      Steps[iA][1][0],
                      Steps[iA][1][0] + Steps[iA][2][0],
                      Steps[nA][1][0] + Steps[nA][0][0],
                      Steps[nA][1][0]);
  pos[1] = glm_bezier(state->dA,
                      Steps[iA][1][1],
                      Steps[iA][1][1] + Steps[iA][2][1],
                      Steps[nA][1][1] + Steps[nA][0][1],
                      Steps[nA][1][1]);
  pos[2] = glm_bezier(state->dA,
                      Steps[iA][1][2],
                      Steps[iA][1][2] + Steps[iA][2][2],
                      Steps[nA][1][2] + Steps[nA][0][2],
                      Steps[nA][1][2]);
  up[0] = pos[0];
  up[1] = pos[1] + 10.f;
  up[2] = pos[2];

  //glm_translate_z(model, -5.f);
  //glm_rotate_y(model, state->rot, model);
  glm_scale_uni(model, 0.2f);

  //glm_euler_yxz_quat(rot, quat);
  //glm_quat_look(pos, quat, view);
  //glm_mat4_mulv3(model, pos, 1.f, pos);
  glm_lookat(pos, rot, up, view);

  glm_mat4_mul(view, model, mvp);
  glm_mat4_mul(persp, mvp, mvp);

  //glm_bezier();
  glUniformMatrix4fv(state->mvpPos, 1, GL_FALSE, &(mvp[0][0]));

  /*
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  glClearColor(0.f, 0.f, 0.f, 0.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  */

  glViewport(0, 0, VW, VH);
  glDrawArrays(GL_TRIANGLES, 0, 6*2*3);

  char *pixels = SDL_malloc(VW * VH);
  while (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE);
  glReadPixels(0, 0, VW, VH, GL_LUMINANCE, GL_UNSIGNED_BYTE, pixels);

  int total = 0;
  for (int k = 0; k < VW * VH; ++k)
    total += pixels[k];
  SDL_Log("Total: %d", total);
  SDL_free(pixels);

  SDL_GL_SwapWindow(state->win);
  return SDL_APP_CONTINUE;
}
