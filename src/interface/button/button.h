#ifndef BUTTON_H
#define BUTTON_H

#include <SDL.h>
#include <SDL_ttf.h>
#include "../../logs/log.h"

typedef struct Button {
    int x, y;
    int width, height;
    const char* label;
} Button;


Button* init_button(int x, int y, int width, int height, const char* label);
void render_button(SDL_Renderer* renderer, TTF_Font* font, Button* button, SDL_Color textColor, SDL_Color buttonColor);
void free_button(Button* button);

#endif