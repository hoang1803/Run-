#ifndef BOSS_OBJECT_H_


#define BOSS_OBJECT_H_


#include "BaseFunc.h"
#include "BaseObject.h"
#include "MainObject.h"
#include "ThreatObject.h"





class BossObject : public BObject
{
public:
    BossObject(const int& delayTime_ = 1);
    ~BossObject();

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

    int checkCollision(SDL_Rect rect_);

    int getDifficult()
    {
        return stateBoss.Difficulty;
    }

    bool getIsActive()
    {
        return is_active;
    }

    std::string getPath()
    {
        std::string ans = "Image//monsters//";
        ans += "DemonSlime//";
        return ans;
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
    threatObject::AnimationAction animation;
    InputAction inputType;

    int delayCounter;
    int delayTime;

    State stateBoss;

};












#endif // BOSS_OBJECT_H_
