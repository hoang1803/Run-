#ifndef BASE_FUNCTION_H_

#define BASE_FUNCTION_H_



#include <iostream>
#include <windows.h>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <fstream>
#include <vector>
#include <algorithm>
#include <queue>
#include <SDL_ttf.h>
#include <math.h>


#define all(x) x.begin(), x.end()

static SDL_Window* gWindow = NULL;
static SDL_Renderer* gRenderer = NULL;
static SDL_Event g_event;

static Mix_Chunk* soundClick = NULL;
static Mix_Chunk* soundGameWin = NULL;
static Mix_Chunk* soundGameOver = NULL;

const int SCREEN_WIDTH = 880;
const int SCREEN_HEIGHT = 440;
const int SCREEN_BPP = 32;
const std::string WINDOW_TITLE = "Run!";

const int FRAME_PER_SECOND = 20;

#define TILE_SIZE 44

#define MAX_MAP_X 400
#define MAX_MAP_Y 10

#define BLANK_TILE 0
#define STATE_MONEY 10
#define STATE_HEALTH_POINT_1 5
#define STATE_HEALTH_POINT_2 6
#define STATE_ATTACK 10

#define NUMBER_MONSTER 7

enum WalkType
{
    WALK_RIGHT = 0,
    WALK_LEFT = 1,
};

enum DIFFICULTY
{
    EASY = 0,
    MEDIUM = 1,
    HARD = 2,
};
struct State
{
    int HealthPoint;
    int Attack;
    double Speed;
    int Difficulty;
    State(const int& HealthPoint_ = 0, const int& Attack_ = 0, const double& Speed_ = 0, const int& Difficulty_ = 0)
    {
        HealthPoint = HealthPoint_;
        Attack = Attack_;
        Speed = Speed_;
        Difficulty = Difficulty_;
    }
};
struct Map
{
    int start_x;
    int start_y;

    int max_x;
    int max_y;

    int tile[MAX_MAP_Y][MAX_MAP_X];
    std::string file_name;
};


struct InputAction
{
    int left_;

    int right_;

    int up_;

    int down_;

    int jump_;

    int attack_;
};

enum TypeAction
{
    RUN = 1,
    JUMP = 2,
    FALL = 3,
    UP = 4,
    DOWN = 5,
    ATTACK = 6,
    IDLE = 7,
    DEAD = 8,
};

namespace SDLBaseFunc
{

void logSDLError(std::ostream& os,
                 const std::string &msg, bool fatal = false);

void logIMGError(std::ostream& os,
                 const std::string &msg, bool fatal = false);

void logMIXError(std::ostream& os,
                 const std::string &msg, bool fatal = false);

void logTTFError(std::ostream& os,
                 const std::string &msg, bool fatal = false);

void initSDL(SDL_Window* &window, SDL_Renderer* &renderer);

void quitSDL(SDL_Window* window, SDL_Renderer* renderer);

void waitUntilKeyPressed();

int randInt(int l, int r);

bool checkFocusWithRect(const int& x, const int& y, SDL_Rect rect);


}

#endif // BASE_FUNCTION_H_

