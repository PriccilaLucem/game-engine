#include "./ui.h"
#include <stdlib.h>

UI* init_UI(SDL_Renderer* renderer, SDL_Window* window, const char* title, int width, int height, const char* font_path, int font_size, const char* button_label){
    UI* ui = (UI*)malloc(sizeof(UI));
    if(!ui){
        log_message(LOG_LEVEL_ERROR, "Failed to allocate memory for UI", __FILE__);
        exit(1);
    }
    ui->window = window;
    ui->renderer = renderer;
    ui->font = TTF_OpenFont(font_path, font_size);
    
    if(!ui->font){
        log_message(LOG_LEVEL_ERROR, TTF_GetError(), __FILE__);
        free(ui);
        exit(1);
    }
    ui->title = init_text(title, 50, 20, font_size, font_path);
    if(!ui->title){
        TTF_CloseFont(ui->font);
        free(ui);
        exit(1);
    }
    ui->button = init_button(50, 100, 200, 50, button_label);
    if(!ui->button){
        free_text(ui->title);
        TTF_CloseFont(ui->font);
        free(ui);
        exit(1);
    }
    return ui;
}

void render_UI(UI* ui, SDL_Color text_color, SDL_Color button_color){
    if(ui->title){
        render_text(ui->title, ui->renderer, text_color);
    }
    if(ui->button){
        render_button(ui->renderer, ui->font, ui->button, text_color, button_color);
    }
}

void free_UI(UI* ui){
    if(ui){
        if(ui->button){
            free_button(ui->button);
        }
        if(ui->title){
            free_text(ui->title);
        }
        if(ui->font){
            TTF_CloseFont(ui->font);
        }
        free(ui);
    }
}