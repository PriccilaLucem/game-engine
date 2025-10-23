#include "./initSDL.h"
#include "./interface/ui.h"
#include "./logs/log.h"

SDLContext* init_sdl(void) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        log_message(LOG_LEVEL_ERROR, SDL_GetError(), __FILE__);
        exit(1);
    }
    if (TTF_Init() != 0) {
        log_message(LOG_LEVEL_ERROR, TTF_GetError(), __FILE__);
        exit(1);
    }
    
    log_message(LOG_LEVEL_INFO, "SDL and TTF initialized successfully.", __FILE__);

    SDLContext* context = (SDLContext*) malloc(sizeof(SDLContext));
    if (!context) {
        log_message(LOG_LEVEL_ERROR, "Failed to allocate SDLContext", __FILE__);
        exit(1);
    }

    context->window = NULL;
    context->renderer = NULL;
    return context;
}


void init_window(SDLContext* context, const char* title, int width, int height){
    context->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    if (context->window == NULL) {
        log_message(LOG_LEVEL_ERROR, SDL_GetError(), __FILE__);
        exit(1);
    }
    log_message(LOG_LEVEL_INFO, "SDL Window created successfully.", __FILE__);
}

void init_renderer(SDLContext* context){
    context->renderer = SDL_CreateRenderer(context->window, -1, SDL_RENDERER_ACCELERATED);
    if (context->renderer == NULL) {
        log_message(LOG_LEVEL_ERROR, SDL_GetError(), __FILE__);
        exit(1);
    }
    log_message(LOG_LEVEL_INFO, "SDL Renderer created successfully.", __FILE__);
}

void free_sdl(SDLContext* context) {
    if (context) {
        if (context->renderer) {
            SDL_DestroyRenderer(context->renderer);
            log_message(LOG_LEVEL_INFO, "SDL Renderer destroyed successfully.", __FILE__);
        }
        if (context->window) {
            SDL_DestroyWindow(context->window);
            log_message(LOG_LEVEL_INFO, "SDL Window destroyed successfully.", __FILE__);
        }
        // UI cleanup handled by screen manager
        free(context); 
    }
}

void close_sdl(void) {
    TTF_Quit();
    SDL_Quit();
    log_message(LOG_LEVEL_INFO, "SDL and TTF quit successfully.", __FILE__);
}


void main_loop(SDLContext* context){
    SDL_Event event;
    
    // Load font
    TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/arial.ttf", 24);
    if (!font) {
        log_message(LOG_LEVEL_ERROR, "Failed to load font", __FILE__);
        return;
    }

    UI_Init(context->renderer);
    UI_SetState(UI_MAIN_MENU);

    UI_LoadLayout("assets/ui/main_menu.xml");  // <-- adicione aqui (ou o nome do seu XML)
    
    int running = 1;
    
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
            UI_HandleEvent(&event);
        }
        UI_RenderCurrent(context->renderer, font);
        SDL_RenderPresent(context->renderer);
    }

    UI_Quit(); // 💡 limpa a memória da UI
    TTF_CloseFont(font);
}
