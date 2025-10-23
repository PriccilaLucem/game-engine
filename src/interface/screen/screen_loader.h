#ifndef SCREEN_LOADER_H
#define SCREEN_LOADER_H

#include <SDL.h>
#include <SDL_ttf.h>
#include "tinyxml2.h"
#include <array>
#include <string>
#include <map>
#include <stdbool.h>

#define MAX_UI_ELEMENTS 128
#define MAX_ID_LEN 64
#define MAX_TEXT_LEN 256

extern SDL_Color g_background_color;

typedef enum {
    UI_LABEL = 0,
    UI_BUTTON = 1
} UIElementType;

struct UIElement;
typedef void (*UI_OnClick)(struct UIElement* element);

struct UIElement {
    UIElementType type;
    char id[MAX_ID_LEN];
    SDL_Rect rect;
    char text[MAX_TEXT_LEN];
    SDL_Color color;
    bool hovered;
    bool pressed;
    UI_OnClick on_click;
    int font_size;
    char align[16];

    // Texturas e caminhos (para futura expansão)
    SDL_Texture* bg_texture;
    SDL_Texture* icon_texture;
    char bg_path[256];
    char icon_path[256];
    int icon_size;
    int icon_offset;
};

// =====================
// Funções públicas
// =====================
void UI_Init(SDL_Renderer* renderer);
void UI_Quit();
void UI_Clear();

void UI_LoadLayout(const char* filename);
void UI_Render(SDL_Renderer* renderer, TTF_Font* default_font);
void UI_HandleEvent(SDL_Event* e);
void add_element(const UIElement& e);

UIElement* UI_FindById(const char* id);
void UI_SetOnClick(const char* id, UI_OnClick callback);

#endif
