#include "screen_loader.h"
#include <string>
#include <cstdio>

extern std::array<UIElement, MAX_UI_ELEMENTS> elements;
extern int element_count;
extern SDL_Color g_background_color;

void render_text(SDL_Renderer* renderer, TTF_Font* font, const char* text,
                 SDL_Color color, const SDL_Rect& rect, const char* align)
{
    if (!font || !text || text[0] == '\0') return;

    SDL_Surface* surf = TTF_RenderUTF8_Blended(font, text, color);
    if (!surf) return;
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
    if (!tex) { SDL_FreeSurface(surf); return; }

    SDL_Rect dst;
    dst.w = surf->w;
    dst.h = surf->h;

    if (align && strcmp(align, "left") == 0)
        dst.x = rect.x + 20;
    else if (align && strcmp(align, "right") == 0)
        dst.x = rect.x + rect.w - dst.w - 20;
    else
        dst.x = rect.x + (rect.w - dst.w) / 2;

    dst.y = rect.y + (rect.h - dst.h) / 2;

    SDL_RenderCopy(renderer, tex, NULL, &dst);
    SDL_DestroyTexture(tex);
    SDL_FreeSurface(surf);
}

void UI_RenderElements(SDL_Renderer* renderer, TTF_Font* default_font) {
    for (int i = 0; i < element_count; ++i) {
        UIElement& e = elements[i];
        int font_size = e.font_size > 0 ? e.font_size : 24;

        if (e.type == UI_BUTTON) {
            if (e.pressed) SDL_SetRenderDrawColor(renderer, 70, 130, 255, 255);
            else if (e.hovered) SDL_SetRenderDrawColor(renderer, 100, 100, 220, 255);
            else SDL_SetRenderDrawColor(renderer, 70, 70, 110, 255);

            SDL_RenderFillRect(renderer, &e.rect);
            SDL_SetRenderDrawColor(renderer, 30, 30, 40, 255);
            SDL_RenderDrawRect(renderer, &e.rect);

            render_text(renderer, default_font, e.text, e.color, e.rect, e.align);
        } 
        else if (e.type == UI_LABEL) {
            render_text(renderer, default_font, e.text, e.color, e.rect, e.align);
        }
    }
}
