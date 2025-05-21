#include "Global.h"

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
  struct GlobalState *state = appstate;
  switch (event->type) {
    case SDL_EVENT_QUIT:
      return SDL_APP_SUCCESS;
      break;
  }
  return SDL_APP_CONTINUE;
}
