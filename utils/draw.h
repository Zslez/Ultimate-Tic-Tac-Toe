/**
 * @file draw.h
 * @author Cristiano Sansò (cristiano.sanso.04@gmail.com)
 * @brief This file contains functions to draw shapes on screen
 * @copyright Copyright (c) 2023
 */



void SetColor(COLORTYPE rgb, int alpha = 255) {
    SDL_SetRenderDrawColor(RENDERER, rgb[0], rgb[1], rgb[2], alpha);
}


void ColorBackground(COLORTYPE bgcolor) {
    SetColor(bgcolor);
    SDL_RenderClear(RENDERER);
}


void DrawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, COLORTYPE color) {
    SDL_Color col = SDL_Color{(Uint8) color[0], (Uint8) color[1], (Uint8) color[2]};

    std::vector<SDL_Vertex> verts = {
        {SDL_FPoint{x1, y1}, col, SDL_FPoint{0}},
        {SDL_FPoint{x2, y2}, col, SDL_FPoint{0}},
        {SDL_FPoint{x3, y3}, col, SDL_FPoint{0}}
    };

    SDL_RenderGeometry(RENDERER, nullptr, verts.data(), verts.size(), nullptr, 0);
}


void DrawTriangleAngle(float A, SDL_Point C, SDL_Point P1, SDL_Point P2, SDL_Point P3, COLORTYPE color) {
    SDL_Color col = SDL_Color{(Uint8) color[0], (Uint8) color[1], (Uint8) color[2]};

    P1.x = (P1.x - C.x) * cos(A) - (P1.y - C.y) * sin(A) + P1.x;
    P1.y = (P1.x - C.x) * cos(A) + (P1.y - C.y) * sin(A) + P1.y;
    P2.x = (P2.x - C.x) * cos(A) - (P2.y - C.y) * sin(A) + P2.x;
    P2.y = (P2.x - C.x) * cos(A) + (P2.y - C.y) * sin(A) + P2.y;
    P3.x = (P3.x - C.x) * cos(A) - (P3.y - C.y) * sin(A) + P3.x;
    P3.y = (P3.x - C.x) * cos(A) + (P3.y - C.y) * sin(A) + P3.y;

    std::vector<SDL_Vertex> verts = {
        {SDL_FPoint{(float) P1.x, (float) P1.y}, col, SDL_FPoint{0}},
        {SDL_FPoint{(float) P2.x, (float) P2.y}, col, SDL_FPoint{0}},
        {SDL_FPoint{(float) P3.x, (float) P3.y}, col, SDL_FPoint{0}}
    };

    SDL_RenderGeometry(RENDERER, nullptr, verts.data(), verts.size(), nullptr, 0);
}


SDL_Rect DrawRectFromOrigin(int x, int y, int w, int h, COLORTYPE color, bool fill = false) {
    SetColor(color);

    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;

    if (fill) SDL_RenderFillRect(RENDERER, &rect);
    else SDL_RenderDrawRect(RENDERER, &rect);

    return rect;
}


SDL_Rect DrawRectFromCenter(int x, int y, int w, int h, COLORTYPE color, bool fill = false) {
    SetColor(color);

    SDL_Rect rect;
    rect.x = WIDTH / 2 - w / 2 + x;
    rect.y = HEIGHT / 2 - h / 2 + y;
    rect.w = w;
    rect.h = h;

    if (fill) SDL_RenderFillRect(RENDERER, &rect);
    else SDL_RenderDrawRect(RENDERER, &rect);

    return rect;
}


void DisplayTextFromOrigin(const char *text, SDL_Color color, TTF_Font *font, int x, int y, bool reverse = false) {
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture * texture = SDL_CreateTextureFromSurface(RENDERER, surface);

    int w = 0;
    int h = 0;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);

    if (reverse) x -= w;

    SDL_Rect rect = {x, y, w, h};

    SDL_RenderCopy(RENDERER, texture, NULL, &rect);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}


void DisplayTextFromCenter(const char *text, SDL_Color color, TTF_Font *font, int x, int y) {
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(RENDERER, surface);

    int w = 0;
    int h = 0;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);

    SDL_Rect dstrect = {WIDTH / 2 - w / 2 + x, HEIGHT / 2 - h / 2 + y, w, h};

    SDL_RenderCopy(RENDERER, texture, NULL, &dstrect);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}


void DisplayCenteredText(const char *text, SDL_Color color, TTF_Font *font, int x, int y, int alpha = 255) {
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(RENDERER, surface);

    if (alpha != 255) {
        SDL_SetTextureAlphaMod(texture, alpha);
    }

    int w = 0;
    int h = 0;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);

    SDL_Rect dstrect = {x - w / 2, y - h / 2, w, h};

    SDL_RenderCopy(RENDERER, texture, NULL, &dstrect);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}



COLORTYPE HalveColor(COLORTYPE color) {
    return {color[0] / 2, color[1] / 2, color[2] / 2};
}





void DrawCircle(int x, int y, int radius) {
    int offsetx, offsety, d;

    offsetx = 0;
    offsety = radius;
    d = radius - 1;

    while (offsety >= offsetx) {
        SDL_RenderDrawLine(RENDERER, x - offsety, y + offsetx, x + offsety, y + offsetx);
        SDL_RenderDrawLine(RENDERER, x - offsetx, y + offsety, x + offsetx, y + offsety);
        SDL_RenderDrawLine(RENDERER, x - offsetx, y - offsety, x + offsetx, y - offsety);
        SDL_RenderDrawLine(RENDERER, x - offsety, y - offsetx, x + offsety, y - offsetx);

        if (d >= 2 * offsetx) {
            d -= 2 * offsetx++ + 1;
        } else if (d < 2 * (radius - offsety)) {
            d += 2 * offsety-- - 1;
        } else {
            d += 2 * (offsety-- - offsetx++ - 1);
        }
    }
}



void DrawCircleColor(COLORTYPE color, int x, int y, int radius) {
    SetColor(color);
    DrawCircle(x, y, radius);
}







/*
I copied lines used to draw circles from the pygame module
thinking it would have been faster but I don't even know how to use these functions 😳
*/

/*
static void drawhorzline(SDL_Surface *surf, Uint32 color, int x1, int y1, int x2) {
    Uint8 *pixel, *end;

    if (x1 == x2) {
        return;
    }

    pixel = ((Uint8 *)surf->pixels) + surf->pitch * y1;
    if (x1 < x2) {
        end = pixel + x2 * surf->format->BytesPerPixel;
        pixel += x1 * surf->format->BytesPerPixel;
    }
    else {
        end = pixel + x1 * surf->format->BytesPerPixel;
        pixel += x2 * surf->format->BytesPerPixel;
    }
    switch (surf->format->BytesPerPixel) {
        case 1:
            for (; pixel <= end; ++pixel) {
                *pixel = (Uint8)color;
            }
            break;
        case 2:
            for (; pixel <= end; pixel += 2) {
                *(Uint16 *)pixel = (Uint16)color;
            }
            break;
        case 3:
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
            color <<= 8;
#endif
            for (; pixel <= end; pixel += 3) {
                memcpy(pixel, &color, 3 * sizeof(Uint8));
            }
            break;
        default:
            for (; pixel <= end; pixel += 4) {
                *(Uint32 *)pixel = color;
            }
            break;
    }
}


static int set_at(SDL_Surface *surf, int x, int y, Uint32 color) {
    SDL_PixelFormat *format = surf->format;
    Uint8 *pixels = (Uint8 *)surf->pixels;
    Uint8 *byte_buf, rgb[4];

    if (x < surf->clip_rect.x || x >= surf->clip_rect.x + surf->clip_rect.w ||
        y < surf->clip_rect.y || y >= surf->clip_rect.y + surf->clip_rect.h)
        return 0;

    switch (format->BytesPerPixel) {
        case 1:
            *((Uint8 *)pixels + y * surf->pitch + x) = (Uint8)color;
            break;
        case 2:
            *((Uint16 *)(pixels + y * surf->pitch) + x) = (Uint16)color;
            break;
        case 4:
            *((Uint32 *)(pixels + y * surf->pitch) + x) = color;
            break;
        default:
            SDL_GetRGB(color, format, rgb, rgb + 1, rgb + 2);
            byte_buf = (Uint8 *)(pixels + y * surf->pitch) + x * 3;
#if (SDL_BYTEORDER == SDL_LIL_ENDIAN)
            *(byte_buf + (format->Rshift >> 3)) = rgb[0];
            *(byte_buf + (format->Gshift >> 3)) = rgb[1];
            *(byte_buf + (format->Bshift >> 3)) = rgb[2];
#else
            *(byte_buf + 2 - (format->Rshift >> 3)) = rgb[0];
            *(byte_buf + 2 - (format->Gshift >> 3)) = rgb[1];
            *(byte_buf + 2 - (format->Bshift >> 3)) = rgb[2];
#endif
            break;
    }
    return 1;
}



static void drawhorzlineclipbounding(SDL_Surface *surf, Uint32 color, int x1, int y1, int x2) {
    if (y1 < surf->clip_rect.y || y1 >= surf->clip_rect.y + surf->clip_rect.h)
        return;

    if (x2 < x1) {
        int temp = x1;
        x1 = x2;
        x2 = temp;
    }

    x1 = std::max(x1, surf->clip_rect.x);
    x2 = std::min(x2, surf->clip_rect.x + surf->clip_rect.w - 1);

    if (x2 < surf->clip_rect.x || x1 >= surf->clip_rect.x + surf->clip_rect.w)
        return;

    if (x1 == x2) {
        return;
    }

    drawhorzline(surf, color, x1, y1, x2);
}



void DrawCircleFilled(COLORTYPE col, int x0, int y0, int radius) {
    int f = 1 - radius;
    int ddF_x = 0;
    int ddF_y = -2 * radius;
    int x = 0;
    int y = radius;

    //Uint32 color = (255 << 24) + (col[0] << 16) + (col[1] << 8)  + std::col);
    Uint32 color = 0xffffffff;

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }

        x++;
        ddF_x += 2;
        f += ddF_x + 1;

        if (f >= 0) {
            drawhorzlineclipbounding(TRUESURF, color, x0 - x, y0 + y - 1, x0 + x - 1);
            drawhorzlineclipbounding(TRUESURF, color, x0 - x, y0 - y, x0 + x - 1);
        }

        drawhorzlineclipbounding(TRUESURF, color, x0 - y, y0 + x - 1, x0 + y - 1);
        drawhorzlineclipbounding(TRUESURF, color, x0 - y, y0 - x, x0 + y - 1);
    }
}
*/
