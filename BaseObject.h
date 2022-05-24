#ifndef BASE_OBJECT_H_

#define BASE_OBJECT_H_

#include "BaseFunc.h"

class BObject
{
public:
    BObject();
    ~BObject();

    void free();
    void setCoordinates(int x, int y);
    void setSize(int w, int h);

    virtual void loadImage(const std::string &path, SDL_Renderer* &renderer);
    void render(SDL_Renderer* &renderer);

    int getWidth();
    int getHeight();

    bool isItem(int state);


protected:
    SDL_Texture* Object;
    SDL_Rect rect;

    std::vector <int> items;


};


#endif // BASE_OBJECT_H_
