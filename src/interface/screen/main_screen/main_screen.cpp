#include "./main_screen.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_UI_ELEMENTS 128
static UIElement elements[MAX_UI_ELEMENTS];
static int element_count = 0;

void UI_Init(SDL_Renderer* renderer) {
    element_count = 0;
}

void UI_Quit() {
    element_count = 0;
}

static void add_element(UIElement e) {
    if (element_count < MAX_UI_ELEMENTS) {
        elements[element_count++] = e;
    }
}

void UI_LoadLayout(const char* filename) {
    using namespace tinyxml2;
    XMLDocument doc;
    if (doc.LoadFile(filename) != XML_SUCCESS) {
        printf("Falha ao carregar UI: %s\n", filename);
        return;
    }

    XMLElement* root = doc.FirstChildElement("ui");
    for (XMLElement* el = root->FirstChildElement(); el; el = el->NextSiblingElement()) {
        UIElement e;
        memset(&e, 0, sizeof(UIElement));
        
        const char* id = el->Attribute("id");
        const char* text = el->Attribute("text");
        int x = el->IntAttribute("x");
        int y = el->IntAttribute("y");
        int w = el->IntAttribute("w", 100);
        int h = el->IntAttribute("h", 30);

        strcpy(e.id, id ? id : "");
        strcpy(e.text, text ? text : "");
        e.rect = {x, y, w, h};
        e.color = {255, 255, 255, 255};

        if (strcmp(el->Name(), "label") == 0)
            e.type = UI_LABEL;
        else if (strcmp(el->Name(), "button") == 0)
            e.type = UI_BUTTON;

        add_element(e);
    }
}

void UI_Render(SDL_Renderer* renderer) {
    for (int i = 0; i < element_count; i++) {
        UIElement* e = &elements[i];
        if (e->type == UI_BUTTON) {
            SDL_SetRenderDrawColor(renderer, e->hovered ? 200 : 100, 100, 255, 255);
            SDL_RenderFillRect(renderer, &e->rect);
        } else if (e->type == UI_LABEL) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderDrawRect(renderer, &e->rect);
        }
    }
}

void UI_HandleEvent(SDL_Event* e) {
    if (e->type == SDL_MOUSEMOTION) {
        int mx = e->motion.x;
        int my = e->motion.y;
        for (int i = 0; i < element_count; i++) {
            UIElement* el = &elements[i];
            SDL_Point point = {mx, my};
            el->hovered = SDL_PointInRect(&point, &el->rect);
        }
    } else if (e->type == SDL_MOUSEBUTTONDOWN) {
        int mx = e->button.x;
        int my = e->button.y;
        for (int i = 0; i < element_count; i++) {
            UIElement* el = &elements[i];
            SDL_Point point = {mx, my};
            if (SDL_PointInRect(&point, &el->rect)) {
                if (el->on_click) el->on_click(el);
            }
        }
    }
}
