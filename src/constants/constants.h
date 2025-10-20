#ifndef COSNTANTS_H
#define COSNTANTS_H

#define MAIN_FONT_PATH "assets/fonts/Luckiest_Guy,Playfair_Display/Luckiest_Guy/LuckiestGuy-Regular.ttf"
#define TITLE_SIZE 30
#define BUTTON_FONT_SIZE 24
#define _POSIX_C_SOURCE 200112L
#define POOL_ALLOC(pool, Type) ((Type*)dynamic_pool_alloc(pool))
#define POOL_FREE(pool, ptr) dynamic_pool_free(pool, ptr)

#endif