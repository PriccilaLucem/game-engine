#ifndef INITSDL_H
#define INITSDL_H

#include <SDL_ttf.h>
#include <SDL.h>
#include "logs/log.h"
#include <stdlib.h>
#include "./constants/constants.h"

typedef struct SDLContext {
    SDL_Window* window;
    SDL_Renderer* renderer;
} SDLContext;

SDLContext* init_sdl(void);
void init_window(SDLContext* context, const char* title, int width, int height);
void init_renderer(SDLContext* context);
void close_sdl(void);
void free_sdl(SDLContext* context);
void main_loop(SDLContext* context);


#endif