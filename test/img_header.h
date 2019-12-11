#ifndef IMG_HEADER_H
#define IMG_HEADER_H

typedef struct
{
    int top;
    int left;
    int width;
    int height;
    //background: transparent url('img/MX500 W.png') 0% 0% no-repeat padding-box;
    const char * url;
    SDL_Color color;
    bool opacity: 1;
} image_s;

typedef struct
{
    SDL_Rect rect;
    SDL_Color color;
    bool opacity: 1;
} rectangle_s;

typedef struct
{
    SDL_Rect rect;
    SDL_Color color;
    unsigned char radius;
    bool opacity: 1;
} rounded_rectangle_s;

#endif // IMG_HEADER_H
