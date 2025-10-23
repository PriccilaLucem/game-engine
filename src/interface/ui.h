#ifndef UI_H
#define UI_H

#include <SDL.h>
#include <SDL_ttf.h>
#include "./screen/screen_loader.h"  

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    UI_MAIN_MENU,
    UI_OPTIONS
} UIState;

void UI_SetState(UIState state);
UIState UI_GetState(void);
void UI_RenderCurrent(SDL_Renderer* renderer, TTF_Font* font);

#ifdef __cplusplus
}
#endif

#endif 