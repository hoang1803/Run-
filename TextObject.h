#ifndef TEXT_OBJECT_H_


#define TEXT_OBJECT_H_


#include "BaseFunc.h"

class TextObject
{
public:
    TextObject();
    ~TextObject();

    enum TextColor
    {
        RED_TEXT = 0,
        WHITE_TEXT = 1,
        BLACK_TEXT = 2,
    };


    void loadFromFile(const std::string& path);
    void loadFromRenderText(TTF_Font* font, SDL_Renderer* renderer);
    void Free();

    void setColor(Uint8 red, Uint8 blue, Uint8 green);
    void setColor(int type);

    void renderText(SDL_Renderer* renderer, int x_pos, int y_pos, SDL_Rect* src = NULL,
                    double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flipType = SDL_FLIP_NONE);

    int getWidth() {return width;}
    int getHeight() {return height;}

    void setText(const std::string& text_) {content = text_;}
    std::string getText() {return content;}


private:
    std::string content;
    SDL_Color textColor;
    SDL_Texture* texture;
    int width;
    int height;
};






#endif // TEXT_OBJECT_H_
