#ifndef UI_H
#define UI_H

#include <SDL.h>
#include <SDL_ttf.h>
#include "./button/button.h"
#include "./text/text.h"



typedef struct UI {
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;
    Button* button;
    Text* title;
} UI;

UI* init_UI(SDL_Renderer* renderer, SDL_Window* window, const char* title, int width, int height, const char* font_path, int font_size, const char* button_label);
void render_UI(UI* ui, SDL_Color text_color, SDL_Color button_color);
void free_UI(UI* ui);

#endif