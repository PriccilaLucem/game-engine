#ifndef MAIN_SCREEN_H
#define MAIN_SCREEN_H

#include <SDL.h>
#include "../../../logs/log.h"
#include "tinyxml2.h"

typedef enum {
    UI_LABEL,
    UI_BUTTON
} UIElementType;

typedef struct UIElement {
    UIElementType type;
    char id[64];
    SDL_Rect rect;
    char text[128];
    SDL_Color color;
    int hovered;
    void (*on_click)(struct UIElement* self);
} UIElement;

void UI_Init(SDL_Renderer* renderer);
void UI_LoadLayout(const char* filename);
void UI_Render(SDL_Renderer* renderer);
void UI_HandleEvent(SDL_Event* e);
void UI_Quit();

#endif
