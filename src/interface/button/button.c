#include "button.h"

Button* init_button(int x, int y, int width, int height, const char* label) {
    Button* button = (Button*)malloc(sizeof(Button));
    if (!button) {
        log_message(LOG_LEVEL_ERROR, "Failed to allocate memory for Button", __FILE__);
        return NULL;
    }
    button->x = x;
    button->y = y;
    button->width = width;
    button->height = height;
    button->label = label;
    return button;
}

void render_button(SDL_Renderer* renderer, TTF_Font* font, Button* Button, SDL_Color text_color, SDL_Color button_color) {
    // Draw button rectangle
    SDL_Rect rect = { Button->x, Button->y, Button->width, Button->height };
    SDL_SetRenderDrawColor(renderer, button_color.r, button_color.g, button_color.b, button_color.a);
    SDL_RenderFillRect(renderer, &rect);

    // Render button label
    SDL_Surface* text_surface = TTF_RenderText_Solid(font, Button->label, text_color);
    if (!text_surface) {
        log_message(LOG_LEVEL_ERROR, TTF_GetError(), __FILE__);
        return;
    }
    SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    SDL_FreeSurface(text_surface);
    if (!text_texture) {
        log_message(LOG_LEVEL_ERROR, SDL_GetError(), __FILE__);
        return;
    }

    int text_width, text_height;
    TTF_SizeText(font, Button->label, &text_width, &text_height);
    SDL_Rect text_rect = { Button->x + (Button->width - text_width) / 2, Button->y + (Button->height - text_height) / 2, text_width, text_height };
    SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);
    SDL_DestroyTexture(text_texture);
}

void free_button(Button* button){
    if (button){
        if(button->label){
            free((void*)button->label);
        }
        free(button);
    }
}