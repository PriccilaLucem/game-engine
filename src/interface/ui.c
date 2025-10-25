#include "./ui.h"
#include <stdbool.h>
#include <stdio.h>

static UIState current_state = UI_MAIN_MENU;
static UIState last_state = UI_CHOOSE_FOLDER;
static bool ui_loaded = false;
static void setup_main_menu_callbacks();

static void load_ui_for_state(UIState state, SDL_Renderer* renderer) {
    UI_Clear();
    UI_Init(renderer);


    switch (state) {
        case UI_MAIN_MENU:
            UI_LoadLayout("assets/ui/main_menu.xml");
            setup_main_menu_callbacks();
            break;
        case UI_CHOOSE_FOLDER:
            UI_LoadLayout("assets/ui/choose_folder.xml");
            break; 

        default:
            printf("[UI] Estado de UI desconhecido: %d\n", (int)state);
            break;
    }
}

void UI_SetState(UIState state) {
    if (state != current_state) {
        current_state = state;
        ui_loaded = false;
    }
}

UIState UI_GetState(void) {
    return current_state;
}

void UI_RenderCurrent(SDL_Renderer* renderer, TTF_Font* font) {
    if (!ui_loaded) {
        load_ui_for_state(current_state, renderer);
        ui_loaded = true;
    }

    UI_Render(renderer, font);
}

static void setup_main_menu_callbacks() {
    UI_SetOnClick("new_project", [](UIElement* e) {
        open_file_dialog();
    });

    UI_SetOnClick("load_project", [](UIElement* e) {
        open_folder_dialog();
});
}