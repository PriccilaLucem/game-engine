#include "screen_loader.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <string>

using namespace tinyxml2;

std::array<UIElement, MAX_UI_ELEMENTS> elements;
int element_count = 0;
static std::map<std::string,int> id_index_map;

// cache de fontes
static std::map<int, TTF_Font*> font_cache;
SDL_Color g_background_color = {20,20,20,255};
static int screen_width = 1280;
static int screen_height = 720;

// Funções internas
static TTF_Font* get_font(int size);
extern void parse_ui_element(XMLElement* node);
extern void render_text(SDL_Renderer*, TTF_Font*, const char*, SDL_Color, const SDL_Rect&, const char*);
extern void UI_RenderElements(SDL_Renderer*, TTF_Font*);

void UI_Init(SDL_Renderer* renderer) {
    (void)renderer;
    element_count = 0;
    id_index_map.clear();
}

void UI_Quit() {
    element_count = 0;
    id_index_map.clear();

    for (auto& f : font_cache)
        TTF_CloseFont(f.second);
    font_cache.clear();
}

void UI_Clear() {
    element_count = 0;
    id_index_map.clear();
}

void add_element(const UIElement& e) {
    if (element_count < MAX_UI_ELEMENTS) {
        elements[element_count] = e;
        id_index_map[std::string(e.id)] = element_count;
        element_count++;
    }
}

UIElement* UI_FindById(const char* id) {
    if (!id) return nullptr;
    auto it = id_index_map.find(std::string(id));
    if (it == id_index_map.end()) return nullptr;
    return &elements[it->second];
}

void UI_SetOnClick(const char* id, UI_OnClick callback) {
    UIElement* el = UI_FindById(id);
    if (el) el->on_click = callback;
}

void UI_LoadLayout(const char* filename) {
    if (!filename) return;
    XMLDocument doc;
    XMLError r = doc.LoadFile(filename);
    if (r != XML_SUCCESS) {
        printf("[UI] Falha ao carregar UI: %s (error=%d)\n", filename, (int)r);
        return;
    }

    XMLElement* root = doc.FirstChildElement("ui");
    if (!root) {
        printf("[UI] XML não contém <ui> root\n");
        return;
    }

    UI_Clear();
    parse_ui_element(root);
}

void UI_Render(SDL_Renderer* renderer, TTF_Font* default_font) {
    if (!renderer) return;

    SDL_SetRenderDrawColor(renderer, g_background_color.r, g_background_color.g, g_background_color.b, 255);
    SDL_RenderClear(renderer);

    UI_RenderElements(renderer, default_font);
}

void UI_HandleEvent(SDL_Event* ev) {
    if (!ev) return;

    if (ev->type == SDL_MOUSEMOTION) {
        int mx = ev->motion.x, my = ev->motion.y;
        for (int i = 0; i < element_count; ++i) {
            SDL_Point p = {mx, my};
            elements[i].hovered = SDL_PointInRect(&p, &elements[i].rect);
        }
    } 
    else if (ev->type == SDL_MOUSEBUTTONDOWN && ev->button.button == SDL_BUTTON_LEFT) {
        int mx = ev->button.x, my = ev->button.y;
        for (int i = 0; i < element_count; ++i) {
            SDL_Point p = {mx, my};
            if (SDL_PointInRect(&p, &elements[i].rect))
                elements[i].pressed = true;
        }
    } 
    else if (ev->type == SDL_MOUSEBUTTONUP && ev->button.button == SDL_BUTTON_LEFT) {
        int mx = ev->button.x, my = ev->button.y;
        for (int i = 0; i < element_count; ++i) {
            SDL_Point p = {mx, my};
            bool was_pressed = elements[i].pressed;
            elements[i].pressed = false;
            if (was_pressed && SDL_PointInRect(&p, &elements[i].rect))
                if (elements[i].on_click) elements[i].on_click(&elements[i]);
        }
    }
}

static TTF_Font* get_font(int size) {
    auto it = font_cache.find(size);
    if (it != font_cache.end()) return it->second;

    TTF_Font* f = TTF_OpenFont("C:/Windows/Fonts/arial.ttf", size);
    if (!f) {
        printf("[UI] Falha ao carregar fonte: %s\n", TTF_GetError());
        return nullptr;
    }
    font_cache[size] = f;
    return f;
}
