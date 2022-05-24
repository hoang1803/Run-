#include "BaseFunc.h"
#include "BaseObject.h"
#include "GameMap.h"
#include "MainObject.h"
#include "Timer.h"
#include "ThreatObject.h"
#include "BossObject.h"
#include "TextObject.h"

BObject background[3];

ThreatObject Skeleton, Mushroom, Goblin, Scorpian, Bringer_Of_Death, DemonAxeRed, Huntress;

BossObject DemonSlime;

std::vector <ThreatObject> Monster, currentMonster;

int countMonsterWillSpawn[NUMBER_MONSTER];

std::vector <int> spawnOrder;

std::queue <ThreatObject> save;

void setMonster();

void spawnMonster(int numMonster, const int& leftBorder, const int& rightBorder, const int& difficult);

void filter(Uint32 &score);

int limitPerMonster[NUMBER_MONSTER];

void clearAll();

TTF_Font* fontTime = NULL;

void loadFont(TTF_Font* &font, const std::string& path, const int& sizeFont);

bool StartMenu();

bool ChooseDifficult(int& decision);

bool loadEndGame(const std::string& message);

int main(int argc, char* argv[])
{
    SDLBaseFunc::initSDL(gWindow, gRenderer);

    background[0].loadImage("BG//BG1.png", gRenderer);
    background[1].loadImage("BG//BG2.png", gRenderer);
    background[2].loadImage("BG//BG3.png", gRenderer);
    for(int i = 0; i < 3; i++)
        background[i].setSize(SCREEN_WIDTH, SCREEN_HEIGHT);



    //set map game
    GameMap mapGame;
    mapGame.LoadMap("map//map01.dat");
    mapGame.LoadTiles(gRenderer);


    //set player
    MainObject Player;


    //set Sound

    soundClick = Mix_LoadWAV("Sound//Click.wav");
    if(soundClick == NULL)
    {
        SDLBaseFunc::logMIXError(std::cout, "Mix_LoadWAV", 1);
    }

    soundGameOver = Mix_LoadWAV("Sound//gameOver.wav");
    if(soundGameOver == NULL)
    {
        SDLBaseFunc::logMIXError(std::cout, "Mix_LoadWAV", 1);
    }


    soundGameWin = Mix_LoadWAV("Sound//gameWin.wav");
    if(soundGameWin == NULL)
    {
        SDLBaseFunc::logMIXError(std::cout, "Mix_LoadWAV", 1);
    }


    //set font

    loadFont(fontTime, "font//Unicode//tahoma.ttf", 15);



    //set Time text
    TextObject timeGame;
    timeGame.setColor(TextObject::WHITE_TEXT);

    //set Score text
    TextObject scoreGame;
    scoreGame.setColor(TextObject::WHITE_TEXT);
    Uint32 score_value = 0;

    //set Attack text
    TextObject attackGame;
    timeGame.setColor(TextObject::WHITE_TEXT);

    //set HeathPoint text
    TextObject HPGame;
    HPGame.setColor(TextObject::WHITE_TEXT);

    //set Background for text

    BObject bgForText;
    bgForText.loadImage("Image//Black.png", gRenderer);
    bgForText.setSize(SCREEN_WIDTH, 25);
    bgForText.setCoordinates(0, 0);

    Timer fpsTime;

    setMonster();

    bool isQuit = StartMenu();
    bool isLocked = false;

    int timeDelaySpawn = 500;
    int counter = 10;

    int counter2 = 5;

    int decision = -1;

    Uint32 timeStart = 0;
    while(!isQuit)
    {

        int endGame = 0;

        if(decision == -1)
        {
            Player = MainObject();
            Player.setClips();
            isQuit = ChooseDifficult(decision);
            if(isQuit)
                break ;
            Player.setState({1500, 50, DEFAULT_PLAYER_SPEED, decision});

            //set boss
            DemonSlime = BossObject((4 - Player.getState().Difficulty) * 2);
            DemonSlime.setClip(true, (12 + Player.getState().Difficulty) * 600, 200, 3, HARD);
            DemonSlime.setSizeThreat(112, 160);
            DemonSlime.setPosition(13288, 0);
            timeStart = SDL_GetTicks() / 1000;
            isLocked = false;
            counter = 1;
            mapGame = GameMap();
            mapGame.LoadMap("map//map01.dat");
            mapGame.LoadTiles(gRenderer);

            score_value = 0;
        }

        fpsTime.start();
        while(SDL_PollEvent(&g_event))
        {
            if(g_event.type == SDL_QUIT)
            {
                isQuit = true;
                break ;
            }
            Player.HandleInputAction(g_event, gRenderer);
        }
        for(int i = 0; i < 3; i++)
            background[i].render(gRenderer);
        Map map_data = mapGame.getGameMap();

        if(map_data.start_x + SCREEN_WIDTH >= map_data.max_x)
        {
            isLocked = true;
            counter = 2000;
        }

        Player.setMapXY(map_data.start_x, map_data.start_y);
        Player.HandleMove(map_data, isLocked);
        Player.Show(gRenderer);

        counter--;
        if(counter == 0)
        {
            spawnMonster(5, map_data.start_x, map_data.start_x + SCREEN_WIDTH - 200, Player.getState().Difficulty);
            counter = timeDelaySpawn;
        }

        if(!save.empty())
        {
            counter2--;
            if(counter2 == 0)
            {
                counter2 = 10;
                currentMonster.push_back(save.front());
                save.pop();
            }
        }
        else
            counter2 = 10;

        if(isLocked)
        {
            DemonSlime.setMapXY(map_data.start_x, map_data.start_y);
            DemonSlime.ImproveMoveType(Player, map_data, HARD);
            DemonSlime.HandleMove(map_data);
            DemonSlime.Show(gRenderer);
        }

        for(auto &p : currentMonster)
        {
            p.setMapXY(map_data.start_x, map_data.start_y);
            p.ImproveMoveType(Player, map_data, Player.getState().Difficulty);
            p.HandleMove(map_data);
            p.Show(gRenderer);
        }
        filter(score_value);
        mapGame.setMap(map_data);
        mapGame.DrawMap(gRenderer);


        bgForText.render(gRenderer);


        // Show Game Time

        std::string str_time = "Time: ";
        Uint32 timeValue = SDL_GetTicks() / 1000 - timeStart;
        Uint32 timeRemain = (Player.getState().Difficulty + 1) * 100 + 300 - timeValue;


        if(timeRemain <= 0)
        {
            endGame = -1;

//            if(MessageBox(NULL, "Game Over", "Info", MB_OK | MB_ICONSTOP) == IDOK)
//            {
//                isQuit = true;
//                break ;
//            }
        }
        else
        {
            str_time += std::to_string(timeRemain);
            timeGame.setText(str_time);
            timeGame.loadFromRenderText(fontTime, gRenderer);
            timeGame.renderText(gRenderer, SCREEN_WIDTH * 0.8, 5);
        }


        // Show Game Score

        if(DemonSlime.getIsActive() == false)
        {
            score_value += (Player.getState().Difficulty + 1) * 500;
            endGame = 1;
//            isQuit = true;
//            std::cout << "You win";
        }



        std::string str_score = "Score: ";
        str_score += std::to_string(score_value);
        scoreGame.setText(str_score);
        scoreGame.loadFromRenderText(fontTime, gRenderer);
        scoreGame.renderText(gRenderer, SCREEN_WIDTH * 0.6 - 50, 5);


        // Show Player Attack

        std::string str_attack = "ATK: ";
        str_attack += std::to_string(Player.getState().Attack);
        attackGame.setText(str_attack);
        attackGame.loadFromRenderText(fontTime, gRenderer);
        attackGame.renderText(gRenderer, SCREEN_WIDTH * 0.4 - 50, 5);


        // Show Player HealthPoint

        std::string str_HP = "HP: ";
        str_HP += std::to_string(Player.getState().HealthPoint);
        HPGame.setText(str_HP);
        HPGame.loadFromRenderText(fontTime, gRenderer);
        HPGame.renderText(gRenderer, SCREEN_WIDTH * 0.2 - 50, 5);



        SDL_RenderPresent(gRenderer);



        if(Player.getIsDead())
        {
            clearAll();
        }
        if(Player.getIsActive() == false)
        {
            endGame = -1;
        }


        int realTime = fpsTime.getTicks();
        int timeEachFrame = 1000 / FRAME_PER_SECOND; //ms

        if(realTime < timeEachFrame)
        {
            SDL_Delay(timeEachFrame - realTime);
        }

        if(endGame)
        {
            clearAll();
            Mix_PlayChannel(-1, (endGame == -1) ? soundGameOver : soundGameWin, 0);
            isQuit = loadEndGame((endGame == -1) ? "Game Over!" : "Victory!");
            decision = -1;
        }
    }

//    SDLBaseFunc::waitUntilKeyPressed();
    SDLBaseFunc::quitSDL(gWindow, gRenderer);
    return 0;
}
void clearAll()
{
    currentMonster.clear();
    while(!save.empty())
        save.pop();
}

void setMonster()
{
    //set Skeleton

    Skeleton = ThreatObject(SKELETON, 2);
    Skeleton.setClip(false, 100, 10, 3, EASY);
    Skeleton.setSizeThreat(55, 55);
    Monster.push_back(Skeleton);
    limitPerMonster[SKELETON] = 1e6;

    Mushroom = ThreatObject(MUSHROOM, 2);
    Mushroom.setClip(false, 200, 20, 4, EASY);
    Mushroom.setSizeThreat(55, 55);
    Monster.push_back(Mushroom);
    limitPerMonster[MUSHROOM] = 1e6;

    Goblin = ThreatObject(GOBLIN, 1);
    Goblin.setClip(false, 200, 20, 4, MEDIUM);
    Goblin.setSizeThreat(55, 55);
    Monster.push_back(Goblin);
    limitPerMonster[GOBLIN] = 1e6;

    Scorpian = ThreatObject(SCORPIAN, 3);
    Scorpian.setClip(false, 200, 30, 3, MEDIUM);
    Scorpian.setSizeThreat(44, 44);
    Monster.push_back(Scorpian);
    limitPerMonster[SCORPIAN] = 10;

    Huntress = ThreatObject(HUNTRESS, 1);
    Huntress.setClip(false, 200, 70, 5, MEDIUM);
    Huntress. setSizeThreat(55, 50);
    Monster.push_back(Huntress);
    limitPerMonster[HUNTRESS] = 10;

    DemonAxeRed = ThreatObject(DEMON_AXE_RED, 8);
    DemonAxeRed.setClip(false, 1200, 150, 3, HARD);
    DemonAxeRed.setSizeThreat(50, 70);
    Monster.push_back(DemonAxeRed);
    limitPerMonster[DEMON_AXE_RED] = 5;

    Bringer_Of_Death = ThreatObject(BRINGER_OF_DEATH, 2);
    Bringer_Of_Death.setClip(true, 800, 100, 6, HARD);
    Bringer_Of_Death.setSizeThreat(130, 80);
    Monster.push_back(Bringer_Of_Death);
    limitPerMonster[BRINGER_OF_DEATH] = 2;
}

void spawnMonster(int numMonster, const int& leftBorder, const int& rightBorder, const int& difficult)
{
    for(; numMonster > 0; numMonster--)
        countMonsterWillSpawn[SDLBaseFunc::randInt(1, NUMBER_MONSTER) - 1]++;
    int remain = 0;
    spawnOrder.clear();
    for(int i = NUMBER_MONSTER - 1; i >= 0; i--)
    {

        if(countMonsterWillSpawn[i] && Monster[i].getDifficult() > difficult)
        {
            countMonsterWillSpawn[i - 1] += countMonsterWillSpawn[i];
            countMonsterWillSpawn[i] = 0;
            continue ;
        }
        if(countMonsterWillSpawn[i] > limitPerMonster[i])
        {
            remain += countMonsterWillSpawn[i] - limitPerMonster[i];
            countMonsterWillSpawn[i] = limitPerMonster[i];
        }

        for(; countMonsterWillSpawn[i] > 0; countMonsterWillSpawn[i]--)
            spawnOrder.push_back(i);


    }

    for(; remain > 0; remain--)
        spawnOrder.push_back(SDLBaseFunc::randInt(1, 2) - 1);
    std::random_shuffle(spawnOrder.begin(), spawnOrder.end());
    for(auto p : spawnOrder)
    {
        ThreatObject cur = Monster[p];
        cur.setPosition(SDLBaseFunc::randInt(leftBorder, rightBorder), 0);
        save.push(cur);
    }
}

void filter(Uint32 &score)
{
    if(currentMonster.empty())
        return ;
    std::vector <ThreatObject> remain;
    for(auto p : currentMonster)
        if(p.getIsActive())
            remain.push_back(p);
        else
            score += (p.getDifficult() + 1) * 2;
    std::swap(remain, currentMonster);
}


void loadFont(TTF_Font* &font, const std::string& path, const int& sizeFont)
{
    font = TTF_OpenFont(path.c_str(), sizeFont);
    if(font == NULL)
    {
        SDLBaseFunc::logTTFError(std::cout, "loadFont", 1);
    }
}

bool StartMenu()
{

    TTF_Font* font = NULL;

    TTF_Font* fontGameName = NULL;

    loadFont(font, "font//Unicode//arialbd.ttf", 20);

    loadFont(fontGameName, "font//Unicode//verdanab.ttf", 50);
    TextObject gameName;
    gameName.setText("Run!!!");
    gameName.setColor(TextObject::WHITE_TEXT);
    gameName.loadFromRenderText(fontGameName, gRenderer);

    TextObject textMenu[2];
    SDL_Rect pos_arr[2];

    pos_arr[0].x = 380;
    pos_arr[0].y = 280;

    textMenu[0].setText("Play Now");
    textMenu[0].setColor(TextObject::WHITE_TEXT);
    textMenu[0].loadFromRenderText(font, gRenderer);


    pos_arr[1].x = 400;
    pos_arr[1].y = 360;

    textMenu[1].setText("Exit");
    textMenu[1].setColor(TextObject::WHITE_TEXT);
    textMenu[1].loadFromRenderText(font, gRenderer);

    SDL_Event l_event;

    int xMouse = 0;
    int yMouse = 0;

    while(true)
    {

        while(SDL_PollEvent(&l_event))
        {
            switch(l_event.type)
            {
            case SDL_QUIT:
            {
                TTF_CloseFont(font);
                TTF_CloseFont(fontGameName);
                font = NULL;
                fontGameName = NULL;
                return true;
            }

            case SDL_MOUSEMOTION:
            {
                xMouse = l_event.motion.x;
                yMouse = l_event.motion.y;

                for(int i = 0; i < 2; i++)
                {
                    if(SDLBaseFunc::checkFocusWithRect(xMouse, yMouse,
                {pos_arr[i].x, pos_arr[i].y, textMenu[i].getWidth(), textMenu[i].getHeight()}))
                    {

                        textMenu[i].setColor(TextObject::RED_TEXT);
                    }
                    else
                    {
                        textMenu[i].setColor(TextObject::WHITE_TEXT);
                    }

                    textMenu[i].loadFromRenderText(font, gRenderer);
                }
            }
            break ;

            case SDL_MOUSEBUTTONDOWN:
            {
                xMouse = l_event.motion.x;
                yMouse = l_event.motion.y;

                for(int i = 0; i < 2; i++)
                {
                    if(SDLBaseFunc::checkFocusWithRect(xMouse, yMouse,
                {pos_arr[i].x, pos_arr[i].y, textMenu[i].getWidth(), textMenu[i].getHeight()}))
                    {
                        Mix_PlayChannel(-1, soundClick, 0);
                        TTF_CloseFont(font);
                        TTF_CloseFont(fontGameName);
                        font = NULL;
                        fontGameName = NULL;
                        return i;
                    }
                }
            }
            break ;

            case SDL_KEYDOWN:
                if(l_event.key.keysym.sym == SDLK_ESCAPE)
                {
                    TTF_CloseFont(font);
                    TTF_CloseFont(fontGameName);
                    font = NULL;
                    fontGameName = NULL;
                    return true;
                }
                break ;
            default:
                break ;

            }
        }


        for(int i = 0; i < 3; i++)
            background[i].render(gRenderer);

        gameName.renderText(gRenderer, 362, 30);

        for(int i = 0; i < 2; i++)
            textMenu[i].renderText(gRenderer, pos_arr[i].x, pos_arr[i].y);


        SDL_RenderPresent(gRenderer);
    }

    TTF_CloseFont(font);
    TTF_CloseFont(fontGameName);
    return true;
}


bool ChooseDifficult(int& decision)
{

    TTF_Font* font2 = NULL;

    TTF_Font* fontGameDif = NULL;

    loadFont(font2, "font//Unicode//arialbd.ttf", 20);

    loadFont(fontGameDif, "font//Unicode//verdanab.ttf", 50);


    TextObject textDifficult[3];
    SDL_Rect pos_arr[3];

    pos_arr[0].x = 100;
    pos_arr[0].y = 360;

    textDifficult[0].setText("EASY");
    textDifficult[0].setColor(TextObject::WHITE_TEXT);
    textDifficult[0].loadFromRenderText(font2, gRenderer);


    pos_arr[1].x = 400;
    pos_arr[1].y = 360;

    textDifficult[1].setText("MEDIUM");
    textDifficult[1].setColor(TextObject::WHITE_TEXT);
    textDifficult[1].loadFromRenderText(font2, gRenderer);

    pos_arr[2].x = 700;
    pos_arr[2].y = 360;

    textDifficult[2].setText("HARD");
    textDifficult[2].setColor(TextObject::WHITE_TEXT);
    textDifficult[2].loadFromRenderText(font2, gRenderer);

    SDL_Event l_event;

    int xMouse = 0;
    int yMouse = 0;

    while(true)
    {

        while(SDL_PollEvent(&l_event))
        {
            switch(l_event.type)
            {
            case SDL_QUIT:
                return true;
            case SDL_MOUSEMOTION:
            {
                xMouse = l_event.motion.x;
                yMouse = l_event.motion.y;

                for(int i = 0; i < 3; i++)
                {
                    if(SDLBaseFunc::checkFocusWithRect(xMouse, yMouse,
                {pos_arr[i].x, pos_arr[i].y, textDifficult[i].getWidth(), textDifficult[i].getHeight()}))
                    {

                        textDifficult[i].setColor(TextObject::RED_TEXT);
                        textDifficult[i].loadFromRenderText(font2, gRenderer);
                    }
                    else
                    {
                        textDifficult[i].setColor(TextObject::WHITE_TEXT);
                        textDifficult[i].loadFromRenderText(font2, gRenderer);
                    }


                }
            }
            break ;

            case SDL_MOUSEBUTTONDOWN:
            {
                xMouse = l_event.motion.x;
                yMouse = l_event.motion.y;

                for(int i = 0; i < 3; i++)
                {
                    if(SDLBaseFunc::checkFocusWithRect(xMouse, yMouse,
                {pos_arr[i].x, pos_arr[i].y, textDifficult[i].getWidth(), textDifficult[i].getHeight()}))
                    {
                        Mix_PlayChannel(-1, soundClick, 0);
                        decision = i;
                        TTF_CloseFont(font2);
                        TTF_CloseFont(fontGameDif);
                        font2 = NULL;
                        fontGameDif = NULL;
                        return false;
                    }
                }
            }
            break ;

            case SDL_KEYDOWN:
                if(l_event.key.keysym.sym == SDLK_ESCAPE)
                    return true;
                break ;
            default:
                break ;

            }
        }


        for(int i = 0; i < 3; i++)
            background[i].render(gRenderer);



        for(int i = 0; i < 3; i++)
            textDifficult[i].renderText(gRenderer, pos_arr[i].x, pos_arr[i].y);

        SDL_RenderPresent(gRenderer);
    }

    return true;
}


bool loadEndGame(const std::string& message)
{
    TTF_Font* l_font = NULL;

    loadFont(l_font, "font//Unicode//arialbd.ttf", 20);

    TTF_Font* font3 = NULL;

    loadFont(font3, "font//Unicode//arialbd.ttf", 50);

    TextObject textMessage;
    textMessage.setText(message);
    textMessage.setColor(TextObject::WHITE_TEXT);
    textMessage.loadFromRenderText(font3, gRenderer);

    TextObject textContinue[2];
    SDL_Rect pos_arr[2];

    pos_arr[0].x = 100;
    pos_arr[0].y = 360;

    textContinue[0].setText("Restart");
    textContinue[0].setColor(TextObject::WHITE_TEXT);
    textContinue[0].loadFromRenderText(l_font, gRenderer);


    pos_arr[1].x = 700;
    pos_arr[1].y = 360;

    textContinue[1].setText("Exit");
    textContinue[1].setColor(TextObject::WHITE_TEXT);
    textContinue[1].loadFromRenderText(l_font, gRenderer);

    SDL_Event l_event;

    int xMouse = 0;
    int yMouse = 0;

    while(true)
    {

        while(SDL_PollEvent(&l_event))
        {
            switch(l_event.type)
            {
            case SDL_QUIT:
                return true;
            case SDL_MOUSEMOTION:
            {
                xMouse = l_event.motion.x;
                yMouse = l_event.motion.y;

                for(int i = 0; i < 2; i++)
                {
                    if(SDLBaseFunc::checkFocusWithRect(xMouse, yMouse,
                {pos_arr[i].x, pos_arr[i].y, textContinue[i].getWidth(), textContinue[i].getHeight()}))
                    {

                        textContinue[i].setColor(TextObject::RED_TEXT);
                        textContinue[i].loadFromRenderText(l_font, gRenderer);
                    }
                    else
                    {
                        textContinue[i].setColor(TextObject::WHITE_TEXT);
                        textContinue[i].loadFromRenderText(l_font, gRenderer);
                    }


                }
            }
            break ;

            case SDL_MOUSEBUTTONDOWN:
            {
                xMouse = l_event.motion.x;
                yMouse = l_event.motion.y;

                for(int i = 0; i < 2; i++)
                {
                    if(SDLBaseFunc::checkFocusWithRect(xMouse, yMouse,
                {pos_arr[i].x, pos_arr[i].y, textContinue[i].getWidth(), textContinue[i].getHeight()}))
                    {
                        Mix_PlayChannel(-1, soundClick, 0);
                        TTF_CloseFont(font3);
                        TTF_CloseFont(l_font);
                        font3 = NULL;
                        l_font = NULL;
                        return i;
                    }
                }
            }
            break ;

            case SDL_KEYDOWN:
                if(l_event.key.keysym.sym == SDLK_ESCAPE)
                    return true;
                break ;
            default:
                break ;

            }
        }


        for(int i = 0; i < 3; i++)
            background[i].render(gRenderer);


        textMessage.renderText(gRenderer, 300, 50);

        for(int i = 0; i < 2; i++)
            textContinue[i].renderText(gRenderer, pos_arr[i].x, pos_arr[i].y);

        SDL_RenderPresent(gRenderer);
    }

    return true;
}
