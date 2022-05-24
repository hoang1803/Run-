#ifndef MAIN_OBJECT_H_

#define MAIN_OBJECT_H_

#include "BaseFunc.h"
#include "BaseObject.h"

#define GRAVITY_SPEED 0.8
#define MAX_FALL_SPEED 10
#define DEFAULT_PLAYER_SPEED 7
#define PLAYER_JUMP 15
#define WIDTH_CHARACTER 46
#define HEIGHT_CHARACTER 62


namespace mainObject
{

struct Action
{

    int numFrames;
    std::string path;
    int typeAction;
    Action(const int& numFrames_ = 0, const std::string &path_ = "", const int& typeAction_ = -1)
    {
        numFrames = numFrames_;
        path = path_;
        typeAction = typeAction_;
    }
    std::string getFrame(int& frame)
    {
        if(frame <= 0)
        {
            std::cout << "Error: Frame must be positive!!!";
            SDL_Quit();
            exit(-1);
        }
        if(frame > numFrames)
            frame = 1;
        return std::to_string(frame) + ".png";
    }
};

struct AnimationAction
{
    Action Run;

    Action Jump;

    Action Fall;

    Action Up;

    Action Down;

    Action Attack;

    Action Idle;

    Action Dead;

    Action currentAnimation;
};
}

class MainObject : public BObject
{
public:
    MainObject();
    ~MainObject();

    void setClips();
    void setMapXY(const int& _map_x, const int& _map_y)
    {
        map_x = _map_x, map_y = _map_y;
    }

    void loadImage(const std::string& path, SDL_Renderer* renderer);
    void Show(SDL_Renderer* renderer);

    void HandleInputAction(SDL_Event events, SDL_Renderer* renderer);
    void HandleMove(Map& map_data, bool isLocked);
    void checkToMap(Map& map_data);
    void centerEntityOnMap(Map& map_data, bool isLocked);

    void printPosition()
    {
        std::cout << x_pos << " " << y_pos << std::endl;
    }

    double get_pos_x()
    {
        return x_pos;
    }
    double get_pos_y()
    {
        return y_pos;
    }

    SDL_Rect getRect()
    {
        return rect;
    }

    mainObject::AnimationAction getAnimation()
    {
        return animation;
    }

    int getStatus()
    {
        return status_;
    }

    int getFrame()
    {
        return frame_;
    }

    int getWidthFrame()
    {
        return width_frame;
    }

    State getState()
    {
        return stateMain;
    }
    void setState(const State& stateMain_)
    {
        stateMain = stateMain_;
    }

    InputAction getInputType()
    {
        return inputType;
    }

    void setInputType(const InputAction& inputType_)
    {
        inputType = inputType_;
    }

    double get_x_val()
    {
        return x_val;
    }
    double get_y_val()
    {
        return y_val;
    }

    void set_x_val(double x)
    {
        x_val = x;
    }
    void set_y_val(double y)
    {
        y_val = y;
    }
    int getHurtByBoss()
    {
        return isHurtByBoss;
    }
    void setIsHurtByBoss(const int& isHurtByBoss_)
    {
        isHurtByBoss = isHurtByBoss_;
    }

    void increaseHealthPoint()
    {
        stateMain.HealthPoint = std::min(stateMain.HealthPoint + 100, 1500);
    }

    void increaseAttack()
    {
        stateMain.Attack += 20;
    }

    bool getIsDead()
    {
        return is_dead;
    }
    bool getIsActive()
    {
        return is_active;
    }

private:
    double x_pos;
    double y_pos;

    double x_val;
    double y_val;

    InputAction inputType;
    mainObject::AnimationAction animation;
    int frame_;
    bool is_idle;
    bool is_fall;
    int idle_;
    int status_;
    SDL_RendererFlip flipType;

    bool is_active;
    bool is_dead;

    bool onGround;

    int map_x;
    int map_y;

    int width_frame;
    int height_frame;

    State stateMain;

    int comeBackTime;

    int isHurtByBoss;

    int delayCounter;
    int delayTime;


};


#endif // MAIN_OBJECT_H_
