#include "MainObject.h"


MainObject::MainObject()
{
    x_pos = 0;
    y_pos = 0;

    x_val = 0;
    y_val = 0;

    inputType.left_ = inputType.right_ = inputType.up_ =
            inputType.down_ = inputType.jump_ = inputType.attack_ = 0;
    frame_ = 0;
    idle_ = 0;
    is_idle = true;
    is_fall = true;
    status_ = WALK_RIGHT;
    flipType = SDL_FLIP_NONE;

    onGround = false;

    map_x = 0;
    map_y = 0;

    width_frame = 0;
    height_frame = 0;

    comeBackTime = 0;
    isHurtByBoss = 0;

    items = {BLANK_TILE, STATE_MONEY, STATE_ATTACK, STATE_HEALTH_POINT_1, STATE_HEALTH_POINT_2};

    is_active = true;
    is_dead = false;

    delayCounter = 1;
    delayTime = 5;
}

MainObject::~MainObject()
{
    ;
}

void MainObject::setClips()
{
    //setRun
    animation.Run = mainObject::Action(8, "Image//characters//Run//Warrior_Run_", RUN);

    //setUp


    //setDown


    //setJump
    animation.Jump = mainObject::Action(3, "Image//characters//Jump//Warrior_Jump_", JUMP);

    //setFall
    animation.Fall = mainObject::Action(3, "Image//characters//Fall//Warrior_Fall_", FALL);

    //setAttack
    animation.Attack = mainObject::Action(12, "Image//characters//Attack//Warrior_Attack_", ATTACK);

    //setIdle
    animation.Idle = mainObject::Action(6, "Image//characters//Idle//Warrior_Idle_", IDLE);

    //setDead
    animation.Dead = mainObject::Action(6, "Image//characters//Death//Warrior_Death_", DEAD);



    animation.currentAnimation = animation.Idle;
}

void MainObject::loadImage(const std::string& path, SDL_Renderer* renderer)
{
    BObject::loadImage(path, renderer);
    width_frame = 98;
    height_frame = 64;
}
void MainObject::Show(SDL_Renderer* renderer)
{
    if(status_ == WALK_LEFT)
        flipType = SDL_FLIP_HORIZONTAL;
    else
        flipType = SDL_FLIP_NONE;
    if(stateMain.HealthPoint <= 0)
    {
        stateMain.HealthPoint = 0;
        is_dead = true;
    }
    if(inputType.left_ == 1 || inputType.right_ == 1 || inputType.attack_ != 0 || inputType.jump_ == 1 || is_fall == true || !onGround || is_dead)
    {
        is_idle = false;
    }
    else
    {
        is_idle = true;
    }
    if(is_dead)
    {
        if(animation.currentAnimation.typeAction != DEAD)
        {
            animation.currentAnimation = animation.Dead;
            frame_ = 1;
            delayCounter = 0;
        }
        else
        {
            delayCounter--;
            if(delayCounter <= 0)
            {
                frame_++;
                delayCounter = delayTime;
                if(frame_ > animation.Dead.numFrames)
                {
                    is_active = false;
                    return ;
                }
            }
        }
    }
    else if(inputType.attack_)
    {
        if(animation.currentAnimation.typeAction != ATTACK)
        {
            animation.currentAnimation = animation.Attack;
            frame_ = 0;
        }
        ++frame_;
        inputType.attack_--;
    }
    else
    {
        if(onGround)
            is_fall = false;
        if(is_fall)
        {
            if(animation.currentAnimation.typeAction != FALL)
            {
                animation.currentAnimation = animation.Fall;
                frame_ = 0;
            }
            ++frame_;
        }
        else if(!onGround)
        {
            if(animation.currentAnimation.typeAction != JUMP)
            {
                animation.currentAnimation = animation.Jump;
                frame_ = 0;
            }
            ++frame_;
        }
        else
        {
            if(is_idle)
            {
                if(animation.currentAnimation.typeAction != IDLE)
                {
                    animation.currentAnimation = animation.Idle;
                    idle_ = 0;
                }
                idle_++;
            }
            else
            {
                if(animation.currentAnimation.typeAction != RUN)
                {
                    animation.currentAnimation = animation.Run;
                    frame_ = 0;
                }
                frame_++;
            }
        }
    }

    loadImage(animation.currentAnimation.path + animation.currentAnimation.getFrame((is_idle == true) ? idle_ : frame_), renderer);

    rect.x = x_pos - map_x - (width_frame - WIDTH_CHARACTER) / 2;
    rect.y = y_pos - map_y;

//    if(rect.x < 0)
//        rect.x = 0;

    SDL_Rect renderQuad = {rect.x, rect.y, width_frame, height_frame};

//    std::cout << rect.x << " " << rect.y << " " << is_fall << std::endl;
//    SDL_RenderCopy(renderer, Object, NULL, &renderQuad);
    SDL_RenderCopyEx(renderer, Object, NULL, &renderQuad, NULL, NULL, flipType);
}
void MainObject::HandleInputAction(SDL_Event events, SDL_Renderer* renderer)
{
    if(events.type == SDL_KEYDOWN)
    {
        switch(events.key.keysym.sym)
        {
        case SDLK_d:
        {
            status_ = WALK_RIGHT;
            inputType.right_ = 1;
            inputType.left_ = 0;
            inputType.attack_ = 0;
        }
        break ;
        case SDLK_a:
        {
            status_ = WALK_LEFT;
            inputType.right_ = 0;
            inputType.left_ = 1;
            inputType.attack_ = 0;
        }
        break ;
        case SDLK_w:
        {
            inputType.jump_ = 1;
            inputType.attack_ = 0;
        }
        break ;
        case SDLK_j:
        {
            inputType.attack_ += 6;
            while(inputType.attack_ > 24)
                inputType.attack_ -= 6;
        }
        break ;
        default:
            break ;
        }
    }
    else if(events.type == SDL_KEYUP)
    {
        switch(events.key.keysym.sym)
        {
        case SDLK_d:
        {
            inputType.right_ = 0;
        }
        break ;
        case SDLK_a:
        {
            inputType.left_ = 0;
        }
        break ;
        case SDLK_j:
        {
//            inputType.attack_ = 0;
        }
        break ;
        default:
            break ;
        }
    }

}
void MainObject::HandleMove(Map& map_data, bool isLocked)
{
    if(!is_active || is_dead)
        return ;
    x_val = 0;
    y_val += GRAVITY_SPEED;
    if(y_val > MAX_FALL_SPEED)
        y_val = MAX_FALL_SPEED;
    if(isHurtByBoss)
    {
        inputType.attack_ = inputType.right_ = inputType.left_ = inputType.jump_;
    }
    if(inputType.attack_)
        stateMain.Speed = DEFAULT_PLAYER_SPEED / 2;
    else
        stateMain.Speed = DEFAULT_PLAYER_SPEED;
    if(inputType.left_ == 1)
    {
        x_val -= stateMain.Speed;
    }
    else if(inputType.right_ == 1)
    {
        x_val += stateMain.Speed;
    }
    if(inputType.jump_ == 1)
    {
        if(onGround)
        {
            y_val = -PLAYER_JUMP;
            is_fall = false;
        }
        onGround = false;
        inputType.jump_ = 0;
    }
    switch(isHurtByBoss)
    {
    case -1: // Left
    {
        x_val = -20;
        status_ = WALK_RIGHT;
    }
    break ;
    case 1: // Right
    {
        x_val = 20;
        status_ = WALK_LEFT;
    }
    break ;

    case 2: //Up
    {
        y_val = -10;
    }
    break ;
    default:
        break ;

    }

    isHurtByBoss = 0;
    if(isLocked)
    {
        if(x_val >= 0)
            x_val = std::max(0.0, std::min(x_val, 1.0 * SCREEN_WIDTH - (rect.x + WIDTH_CHARACTER)));
        else
        {
            if(rect.x <=  - (width_frame - WIDTH_CHARACTER) / 2)
                x_val = 0;
        }
    }
    checkToMap(map_data);
    centerEntityOnMap(map_data, isLocked);
}
void MainObject::checkToMap(Map& map_data)
{
    if(is_dead)
        return ;

    int x1 = 0;
    int x2 = 0;

    int y1 = 0;
    int y2 = 0;

    // check chieu ngang
    int height_min = std::min(HEIGHT_CHARACTER, TILE_SIZE);

    x1 = (x_pos + x_val) / TILE_SIZE;
    x2 = (x_pos + x_val + WIDTH_CHARACTER - 1) / TILE_SIZE; // neu ko du thi cham vao bien --> tru di 1

    y1 = y_pos / TILE_SIZE;
    y2 = (y_pos + height_min - 1) / TILE_SIZE;

    if(x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
    {
        if(x_val >= 0)
        {
            int &val1 = map_data.tile[y1][x2];
            int &val2 = map_data.tile[y2][x2];

            if(isItem(val1))
            {
                switch(val1)
                {
                case STATE_ATTACK:
                    increaseAttack();
                    break ;
                case STATE_HEALTH_POINT_1:
                    increaseHealthPoint();
                    break ;
                case STATE_HEALTH_POINT_2:
                {
                    increaseHealthPoint();
                    increaseHealthPoint();
                }
                break ;
                }
                val1 = BLANK_TILE;
            }

            if(isItem(val2))
            {
                switch(val2)
                {
                case STATE_ATTACK:
                    increaseAttack();
                    break ;
                case STATE_HEALTH_POINT_1:
                    increaseHealthPoint();
                    break ;
                case STATE_HEALTH_POINT_2:
                {
                    increaseHealthPoint();
                    increaseHealthPoint();
                }
                break ;
                }
                val2 = BLANK_TILE;
            }

            if(!isItem(val1) || !isItem(val2))
            {
                x_pos = x2 * TILE_SIZE - WIDTH_CHARACTER;
                x_val = 0;
            }
        }
        else if(x_val < 0)
        {

            int &val1 = map_data.tile[y1][x1];
            int &val2 = map_data.tile[y2][x1];

            if(isItem(val1))
            {
                switch(val1)
                {
                case STATE_ATTACK:
                    increaseAttack();
                    break ;
                case STATE_HEALTH_POINT_1:
                    increaseHealthPoint();
                    break ;
                case STATE_HEALTH_POINT_2:
                {
                    increaseHealthPoint();
                    increaseHealthPoint();
                }
                break ;
                }
                val1 = BLANK_TILE;
            }

            if(isItem(val2))
            {
                switch(val2)
                {
                case STATE_ATTACK:
                    increaseAttack();
                    break ;
                case STATE_HEALTH_POINT_1:
                    increaseHealthPoint();
                    break ;
                case STATE_HEALTH_POINT_2:
                {
                    increaseHealthPoint();
                    increaseHealthPoint();
                }
                break ;
                }
                val2 = BLANK_TILE;
            }

            if(!isItem(val1) || !isItem(val2))
            {
                x_pos = (x1 + 1) * TILE_SIZE;
                x_val = 0;
            }
        }

    }


    // check chieu cao

    int width_min = std::min(WIDTH_CHARACTER, TILE_SIZE);

    x1 = (x_pos) / TILE_SIZE;
    x2 = (x_pos + width_min - 1) / TILE_SIZE; // neu ko du thi cham vao bien --> tru di 1

    y1 = (y_pos + y_val) / TILE_SIZE;
    y2 = (y_pos + y_val + HEIGHT_CHARACTER - 1) / TILE_SIZE;

    if(x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
    {
        if(y_val >= 0)
        {

            int &val1 = map_data.tile[y2][x1];
            int &val2 = map_data.tile[y2][x2];

            if(isItem(val1))
            {
                switch(val1)
                {
                case STATE_ATTACK:
                    increaseAttack();
                    break ;
                case STATE_HEALTH_POINT_1:
                    increaseHealthPoint();
                    break ;
                case STATE_HEALTH_POINT_2:
                {
                    increaseHealthPoint();
                    increaseHealthPoint();
                }
                break ;
                }
                val1 = BLANK_TILE;
            }

            if(isItem(val2))
            {
                switch(val2)
                {
                case STATE_ATTACK:
                    increaseAttack();
                    break ;
                case STATE_HEALTH_POINT_1:
                    increaseHealthPoint();
                    break ;
                case STATE_HEALTH_POINT_2:
                {
                    increaseHealthPoint();
                    increaseHealthPoint();
                }
                break ;
                }
                val2 = BLANK_TILE;
            }

            if(!isItem(val1) || !isItem(val2))
            {
                y_pos = y2 * TILE_SIZE - HEIGHT_CHARACTER - 1;
                y_val = 0;
                onGround = true;
                is_fall = false;
            }
            else
            {
                int new_y2 = (y_pos + y_val + HEIGHT_CHARACTER + 1) / TILE_SIZE;
                if(!isItem(map_data.tile[new_y2][x1]) || !isItem(map_data.tile[new_y2][x2]))
                {
                    onGround = true;
                    is_fall = false;
                }
                else
                {
                    onGround = false;
                    is_fall = true;
                }

            }
        }
        else if(y_val < 0)
        {

            int &val1 = map_data.tile[y1][x1];
            int &val2 = map_data.tile[y1][x2];

            if(isItem(val1))
            {
                switch(val1)
                {
                case STATE_ATTACK:
                    increaseAttack();
                    break ;
                case STATE_HEALTH_POINT_1:
                    increaseHealthPoint();
                    break ;
                case STATE_HEALTH_POINT_2:
                {
                    increaseHealthPoint();
                    increaseHealthPoint();
                }
                break ;
                }
                val1 = BLANK_TILE;
            }

            if(isItem(val2))
            {
                switch(val2)
                {
                case STATE_ATTACK:
                    increaseAttack();
                    break ;
                case STATE_HEALTH_POINT_1:
                    increaseHealthPoint();
                    break ;
                case STATE_HEALTH_POINT_2:
                {
                    increaseHealthPoint();
                    increaseHealthPoint();
                }
                break ;
                }
                val2 = BLANK_TILE;
            }


            if(!isItem(val1) || !isItem(val2))
            {
                y_pos = (y1 + 1) * TILE_SIZE;
                y_val = 0;
            }
        }

    }

    x_pos += x_val;
    y_pos += y_val;

    if(x_pos < 0)
        x_pos = 0;
    if(x_pos + WIDTH_CHARACTER > map_data.max_x)
    {
        x_pos = map_data.max_x - WIDTH_CHARACTER - 1;
    }
    if(y_pos < 0)
        y_pos = 0;
    if(y_pos > map_data.max_y)
    {
        is_active = false;
    }
}

void MainObject::centerEntityOnMap(Map& map_data, bool isLocked)
{
    if(isLocked)
        return ;
    map_data.start_x = x_pos - SCREEN_WIDTH / 2;
    if(map_data.start_x < 0)
        map_data.start_x = 0;
    if(map_data.start_x + SCREEN_WIDTH >= map_data.max_x)
        map_data.start_x = map_data.max_x - SCREEN_WIDTH;

    map_data.start_y = y_pos - SCREEN_HEIGHT / 2;
    if(map_data.start_y < 0)
        map_data.start_y = 0;
    if(map_data.start_y + SCREEN_HEIGHT >= map_data.max_y)
        map_data.start_y = map_data.max_y - SCREEN_HEIGHT;
}
