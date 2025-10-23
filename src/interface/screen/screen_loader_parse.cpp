#include "screen_loader.h"
#include <cstring>
#include <cstdio>

extern std::array<UIElement, MAX_UI_ELEMENTS> elements;
extern int element_count;
extern SDL_Color g_background_color;
extern void add_element(const UIElement& e);

using namespace tinyxml2;

void parse_ui_element(XMLElement* node) {
    static int current_y = 0;
    static int spacing = 0;
    static int margin_top = 0;

    for (XMLElement* el = node->FirstChildElement(); el; el = el->NextSiblingElement()) {
        const char* name = el->Name();

        if (strcmp(name, "window") == 0) {
            const char* bg = el->Attribute("bg_color");
            if (bg && bg[0] == '#') {
                unsigned r, g, b;
                if (sscanf(bg + 1, "%02x%02x%02x", &r, &g, &b) == 3)
                    g_background_color = { (Uint8)r, (Uint8)g, (Uint8)b, 255 };
            }
            parse_ui_element(el);
        }
        else if (strcmp(name, "vbox") == 0) {
            spacing = el->IntAttribute("spacing", 20);
            margin_top = el->IntAttribute("margin_top", 0);
            current_y = margin_top;
            parse_ui_element(el);
        }
        else if (strcmp(name, "hbox") == 0) {
            parse_ui_element(el);
        }
        else if (strcmp(name, "label") == 0 || strcmp(name, "button") == 0) {
            UIElement ue{};
            memset(&ue, 0, sizeof(UIElement));
            ue.color = {255,255,255,255};
            strncpy(ue.align, "center", sizeof(ue.align)-1);

            const char* id = el->Attribute("id");
            const char* text = el->Attribute("text");
            const char* color_str = el->Attribute("color");
            const char* align_str = el->Attribute("text_align");

            int w = el->IntAttribute("w", 300);
            int h = el->IntAttribute("h", 60);
            int x = (1280 - w) / 2;
            int y = current_y;
            int font_size = el->IntAttribute("font_size", 24);

            if (color_str && color_str[0] == '#') {
                unsigned r, g, b;
                if (sscanf(color_str + 1, "%02x%02x%02x", &r, &g, &b) == 3)
                    ue.color = { (Uint8)r, (Uint8)g, (Uint8)b, 255 };
            }

            if (align_str) strncpy(ue.align, align_str, sizeof(ue.align)-1);
            if (id) strncpy(ue.id, id, MAX_ID_LEN-1);
            if (text) strncpy(ue.text, text, MAX_TEXT_LEN-1);

            ue.font_size = font_size;
            ue.rect = {x, y, w, h};
            ue.type = (strcmp(name, "label") == 0) ? UI_LABEL : UI_BUTTON;

            add_element(ue);
            current_y += h + spacing;
        }
    }
}
