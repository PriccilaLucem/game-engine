#ifndef TEXT_H
#define TEXT_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <stdlib.h>
#include "../../logs/log.h"

typedef struct Text {
    int x, y;
    const char* content;
    int font_size;
    TTF_Font* font;
} Text;

Text* init_text(const char* content, int x, int y, int font_size, const char* font_path);
void render_text(Text* text, SDL_Renderer* renderer, SDL_Color color);
void free_text(Text* text);


#endif

