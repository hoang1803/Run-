#ifndef THREAT_OBJECT_H_


#define THREAT_OBJECT_H_


#include "BaseFunc.h"
#include "BaseObject.h"
#include "MainObject.h"


#define FRAME_SKELETON 7
#define FRAME_MUSHROOM 8
#define FRAME_GOBLIN 7
#define FRAME_SCORPIAN 3
#define FRAME_BRINGER_OF_DEATH 6
#define FRAME_DEMON_AXE_RED 4
#define FRAME_HUNTRESS 4



#define THREAT_GRAVITY_SPEED 0.8
#define THREAT_MAX_SPEED 20

namespace threatObject
{
struct Action
{

    int numFrames;
    std::string path;
    int typeAction;
    bool is_individual;
    Action(const int& numFrames_ = 0, const std::string &path_ = "", const int& typeAction_ = -1, bool is_individual_ = 0)
    {
        numFrames = numFrames_;
        path = path_;
        typeAction = typeAction_;
        is_individual = is_individual_;
    }
    void checkNumFrame(int& frame)
    {
        if(frame < 0)
        {
            std::cout << "In checkNumFrame, " << "Error: Frame must be positive!!!";
            SDL_Quit();
            exit(-1);
        }
        if(frame > numFrames)
            frame = 1;
    }
    std::string getIndividualFrame(int& frame)
    {
        if(!is_individual)
        {
            std::cout << "In getIndividualFrame, " << "Error: This Frame is not exist!!!";
            SDL_Quit();
            exit(-1);
        }
        checkNumFrame(frame);
        return std::to_string(frame) + ".png";
    }
    SDL_Rect getFrameInSprite(int &frame, int width_frame, int height_frame)
    {
        if(is_individual)
        {
            std::cout << "In getFrameInSprite, " << "Error: This Sprite is not exist!!!";
            SDL_Quit();
            exit(-1);
        }
        checkNumFrame(frame);
        SDL_Rect cur;
        cur.x = width_frame * (frame - 1);
        cur.y = 0;
        cur.w = width_frame;
        cur.h = height_frame;
        return cur;
    }
    void setEx(const std::string& name)
    {
        path += name;
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


enum MONSTERS
{
    ZOMBIE = 0,
    SKELETON = 1,
    MUSHROOM = 2,
    GOBLIN = 3,
    SCORPIAN = 4,
    HUNTRESS = 5,
    DEMON_AXE_RED = 6,
    BRINGER_OF_DEATH = 7,
};


class ThreatObject : public BObject
{
public:
    ThreatObject(const int& Monster = -1, const int& delayTime_ = 1);
    ~ThreatObject();
    void loadImage(const std::string& path, SDL_Renderer* renderer);
    void setClip(const bool& is_individual, const int& HealthPoint_ = 0, const int& Attack_ = 0, const double& Speed_ = 0, const int& Difficulty_ = 0);

    void setMapXY(const int& _map_x, const int& _map_y)
    {
        map_x = _map_x, map_y = _map_y;
    }

    void setSizeFrame(const int& w, const int& h)
    {
        width_frame = w;
        height_frame = h;
    }

    void setSizeThreat(const int &w, const int &h)
    {
        width_threat = w;
        height_threat = h;
    }

    void setPosition(const double& x_pos_, const double& y_pos_)
    {
        x_pos = x_pos_;
        y_pos = y_pos_;
    }

    void HandleMove(Map& map_data);
    void checkToMap(Map& map_data);

    void Show(SDL_Renderer* renderer);
    void ImproveMoveType(MainObject& Player, Map& map_data, const int& difficult = EASY);
    std::string getPath()
    {
        std::string ans = "Image//monsters//";
        switch(typeMonster)
        {
        case ZOMBIE:
            ans += "Zombie//";
            break ;
        case SKELETON:
            ans += "Skeleton//";
            break ;
        case MUSHROOM:
            ans += "Mushroom//";
            break ;
        case GOBLIN:
            ans += "Goblin//";
            break ;
        case HUNTRESS:
            ans += "Huntress//";
            break ;
        case BRINGER_OF_DEATH:
            ans += "Bringer-Of-Death//";
            break ;
        case DEMON_AXE_RED:
            ans += "DemonAxeRed//";
            break ;
        case SCORPIAN:
            ans += "Scorpian//";
            break ;
        default:
        {
            std::cout << "In getPath (Threat), " << "Error: Invalid typeMonster";
            SDL_Quit();
            exit(-1);
        }
        break ;
        }
        return ans;
    }

    int checkCollision(SDL_Rect rect_);

    int getDifficult()
    {
        return stateThreat.Difficulty;
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

    SDL_RendererFlip flipType;
    int status_;
    int frame_;

    int width_frame;
    int height_frame;

    int width_threat;
    int height_threat;

    bool onGround;
    bool is_fall;

    int map_x;
    int map_y;

    bool is_dead;
    bool is_active;
    int typeMonster;
    threatObject::AnimationAction animation;
    InputAction inputType;

    int delayCounter;
    int delayTime;

    int timeComeback;

    State stateThreat;
    bool isHurt;
};



#endif // THREAT_OBJECT_H_
