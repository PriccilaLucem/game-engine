#include "text.h"

Text* init_text(const char* content, int x, int y, int font_size, const char* font_path){
    Text* text = (Text*)malloc(sizeof(Text));
    if (!text){
        log_message(LOG_LEVEL_ERROR, "Failed to allocate memory for Text", __FILE__);
        return NULL;
    }
    text->x = x;
    text->y = y;
    text->content = content;
    text->font_size = font_size;
    text->font = TTF_OpenFont(font_path, font_size);
    if (!text->font){
        log_message(LOG_LEVEL_ERROR, TTF_GetError(), __FILE__);
        free(text);
        return NULL;
    }
    return text;
}

void render_text(Text *text, SDL_Renderer *renderer, SDL_Color color){
    SDL_Surface* text_surface = TTF_RenderText_Solid(text->font, text->content, color);
    if (!text_surface){
        log_message(LOG_LEVEL_ERROR, TTF_GetError(), __FILE__);
        return;
    }
    SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    SDL_FreeSurface(text_surface);
    if (!text_texture){
        log_message(LOG_LEVEL_ERROR, SDL_GetError(), __FILE__);
        return;
    }

    int text_width, text_height;
    TTF_SizeText(text->font, text->content, &text_width, &text_height);
    SDL_Rect render_quad = { text->x, text->y, text_width, text_height };
    SDL_RenderCopy(renderer, text_texture, NULL, &render_quad);
    SDL_DestroyTexture(text_texture);
}

void free_text(Text* text){
    if (text){
        if (text->font){
            TTF_CloseFont(text->font);
        }
        free(text);
    }
}