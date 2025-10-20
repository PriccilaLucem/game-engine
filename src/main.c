#include "logs/log.h"
#include <windows.h>
#include "./initSDL.h"

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow) {
    log_message(LOG_LEVEL_INFO, "Starting SDL2 application...", __FILE__);

    // Initialize SDL and TTF
    SDLContext* context = init_sdl();
    init_window(context, "SDL2 Window", 800, 600);
    init_renderer(context);

    // Main loop
    main_loop(context);

    // Cleanup (correct order)
    free_sdl(context);
    close_sdl();

    log_message(LOG_LEVEL_INFO, "Application closed successfully.", __FILE__);
    return 0;
}


