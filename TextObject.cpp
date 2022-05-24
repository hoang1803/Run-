#include "TextObject.h"

TextObject::TextObject()
{
    content = "";

    textColor.r = 255;
    textColor.b = 255;
    textColor.g = 255;

    texture = NULL;
}

TextObject::~TextObject()
{
    ;
}

void TextObject::loadFromRenderText(TTF_Font* font, SDL_Renderer* renderer)
{
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, content.c_str(), textColor);
    if(textSurface == NULL)
        SDLBaseFunc::logTTFError(std::cout, "TTF_RenderText_Solid", 1);
    texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    width = textSurface -> w;
    height = textSurface -> h;


    if(texture == NULL)
        SDLBaseFunc::logSDLError(std::cout, "SDL_CreateTextureFromSurface", 1);
    SDL_FreeSurface(textSurface);
}

void TextObject::Free()
{
    if(texture != NULL)
    {
        SDL_DestroyTexture(texture);
        texture = NULL;
    }
}

void TextObject::setColor(Uint8 red, Uint8 blue, Uint8 green)
{
    textColor.r = red;
    textColor.b = blue;
    textColor.g = green;
}

void TextObject::setColor(int type)
{
    switch(type)
    {
    case RED_TEXT:
        setColor(255, 0, 0);
        break;
    case WHITE_TEXT:
        setColor(255, 255, 255);
        break;
    case BLACK_TEXT:
        setColor(0, 0, 0);
        break;
    }
}


void TextObject::renderText(SDL_Renderer* renderer, int x_pos, int y_pos, SDL_Rect* src,
                            double angle, SDL_Point* center, SDL_RendererFlip flipType)
{
    SDL_Rect des = {x_pos, y_pos, width, height};
    if(src != NULL)
    {
        des.w = src -> w;
        des.h = src -> h;
    }

    SDL_RenderCopyEx(renderer, texture, src, &des, angle, center, flipType);
}
