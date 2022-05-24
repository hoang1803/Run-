#include "BaseObject.h"

BObject::BObject()
{
    Object = NULL;
    rect = {0, 0, 0, 0};
    items = {BLANK_TILE, STATE_MONEY, STATE_ATTACK, STATE_HEALTH_POINT_1, STATE_HEALTH_POINT_2};

}

BObject::~BObject()
{
    BObject::free();
}

void BObject::free()
{
    if(Object != NULL)
    {
        SDL_DestroyTexture(Object);
        Object = NULL;
        rect = {0, 0, 0, 0};
    }
}

void BObject::setCoordinates(int x, int y) // Đặt tọa độ
{
    rect.x = x;
    rect.y = y;
}

void BObject::setSize(int w, int h) // Đặt w: chiều dài, h: chiều cao
{
    rect.w = w;
    rect.h = h;
}

void BObject::loadImage(const std::string &path, SDL_Renderer* &renderer)
{
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if(loadedSurface == NULL)
        SDLBaseFunc::logIMGError(std::cout, "LoadImage", 1);
    free();
    Object = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    if(Object == NULL)
        SDLBaseFunc::logSDLError(std::cout, "CreateTexture", 1);
    BObject::setSize(loadedSurface -> w, loadedSurface -> h);
    SDL_FreeSurface(loadedSurface);
}

void BObject::render(SDL_Renderer* &renderer)
{
    SDL_RenderCopy(renderer, Object, NULL, &rect);
}

int BObject::getWidth()
{
    return rect.w;
}

int BObject::getHeight()
{
    return rect.h;
}

bool BObject::isItem(int state)
{
    for(auto p : items)
        if(state == p)
            return true;
    return false;
}
