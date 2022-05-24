#include "BossObject.h"

BossObject::BossObject(const int& delayTime_)
{
    x_pos = y_pos = 0;
    x_val = y_val = 0;

    flipType = SDL_FLIP_NONE;
    status_ = WALK_LEFT;
    frame_ = 0;

    width_frame = 0;
    height_frame = 0;

    width_threat = 0;
    height_threat = 0;

    onGround = false;
    is_fall = true;

    map_x = 0;
    map_y = 0;

    is_dead = false;
    is_active = true;
    inputType = {0, 0, 0, 0, 0, 0};

    delayCounter = 1;
    delayTime = delayTime_;

}

BossObject::~BossObject()
{
    ;
}


int BossObject::checkCollision(SDL_Rect rect_)
{
    SDL_Rect nRect = rect;
    nRect.w = width_threat;
    nRect.h = height_threat;

    nRect.x += (width_frame - width_threat) / 2;

    if(nRect.y + nRect.h <= rect_.y)
        return 0;

    if(rect_.y + rect_.h <= nRect.y)
        return 0;

    if(nRect.x + nRect.w <= rect_.x)
        return 0;

    if(rect_.x + rect_.w <= nRect.x)
        return 0;

    if(rect_.x <= nRect.x && nRect.x + nRect.w <= rect_.x + rect_.w) // rect_ va nRect co cung diem bat dau --> se theo huong cua 2 object
        return 2;

    if(nRect.x <= rect_.x && rect_.x + rect_.w <= nRect.x + nRect.w) // rect_ va nRect co cung diem bat dau --> se theo huong cua 2 object
        return 2;

    if(nRect.x <= rect_.x + rect_.w && rect_.x + rect_.w <= nRect.x + nRect.w) // rect_ o ben trai so voi rect
        return -1;

    if(nRect.x <= rect_.x && rect_.x <= nRect.x + nRect.w) // rect_ o ben phai so voi rect
        return 1;

    return 0;
}


void BossObject::setClip(const bool& is_individual, const int& HealthPoint_, const int& Attack_, const double& Speed_, const int& Difficulty_)
{

    if(is_individual)
    {
        animation.Run = threatObject::Action(12, getPath(), RUN, is_individual);
        animation.Run.setEx("Walk//demon_walk_");

        //set Attack
        animation.Attack = threatObject::Action(15, getPath(), ATTACK, is_individual);
        animation.Attack.setEx("Cleave//demon_cleave_");

        //set Dead
        animation.Dead = threatObject::Action(22, getPath(), DEAD, is_individual);
        animation.Dead.setEx("Death//demon_death_");

        animation.Idle = threatObject::Action(6, getPath(), IDLE, is_individual);
        animation.Idle.setEx("Idle//demon_idle_");
    }
    animation.currentAnimation = animation.Idle;
    stateBoss = State(HealthPoint_, Attack_, Speed_, Difficulty_);
}
void BossObject::loadImage(const std::string& path, SDL_Renderer* renderer)
{
    BObject::loadImage(path, renderer);
    width_frame = rect.w;
    if(animation.currentAnimation.is_individual == 0)
        width_frame /= animation.currentAnimation.numFrames;
    height_frame = rect.h;
    rect.w = width_frame;
}
void BossObject::Show(SDL_Renderer* renderer)
{
    if(!is_active)
        return ;
    if(!is_dead)
    {
        if(status_ == WALK_RIGHT)
            flipType = SDL_FLIP_HORIZONTAL;
        else
            flipType = SDL_FLIP_NONE;
        if(onGround)
        {
            if(inputType.attack_)
            {
                if(animation.currentAnimation.typeAction != ATTACK)
                {
                    animation.currentAnimation = animation.Attack;
                    frame_ = 0;
                    delayCounter = 0;
                }
                delayCounter--;
                if(delayCounter <= 0)
                {
                    ++frame_;
                    inputType.attack_--;
                    delayCounter = delayTime;
                }
            }
            else if(inputType.left_ == 1 || inputType.right_ == 1)
            {
                if(animation.currentAnimation.typeAction != RUN)
                {
                    animation.currentAnimation = animation.Run;
                    frame_ = 0;
                    delayCounter = 0;
                }
                frame_++;
            }
            else
            {
                if(animation.currentAnimation.typeAction != IDLE)
                {
                    animation.currentAnimation = animation.Idle;
                    frame_ = 0;
                    delayCounter = 0;
                }
                frame_++;
            }
        }

    }
    else
    {
        if(animation.Dead.typeAction == -1)
        {
            is_active = false;
            return ;
        }
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
                delayCounter = 3;
                animation.currentAnimation.checkNumFrame(frame_);
                if(frame_ == 1)
                {
                    is_active = false;
                    return ;
                }
            }
        }
    }
    if(frame_ == 0)
        frame_++;
    if(animation.currentAnimation.is_individual)
    {
        loadImage(animation.currentAnimation.path + animation.currentAnimation.getIndividualFrame(frame_), renderer);
    }
    else
        loadImage(animation.currentAnimation.path, renderer);

    rect.x = x_pos - map_x - (width_frame - width_threat) / 2;
    rect.y = y_pos - map_y;

//    if(rect.x < 0)
//        rect.x = 0;
//    if(rect.y < 0)
//        rect.y = 0;

    SDL_Rect renderQuad = {rect.x, rect.y, width_frame, height_frame};
    SDL_Rect* src = NULL;

    if(!animation.currentAnimation.is_individual)
    {
        SDL_Rect temp = animation.currentAnimation.getFrameInSprite(frame_, width_frame, height_frame);
        src = &temp;
    }

    SDL_RenderCopyEx(renderer, Object, src, &renderQuad, NULL, NULL, flipType);

}
void BossObject::HandleMove(Map& map_data)
{
    if(!is_dead && is_active)
    {
        x_val = 0;
        y_val += GRAVITY_SPEED;
        if(y_val > MAX_FALL_SPEED)
            y_val = MAX_FALL_SPEED;
        if(inputType.left_ == 1)
        {
            x_val -= stateBoss.Speed;
        }
        else if(inputType.right_ == 1)
        {
            x_val += stateBoss.Speed;
        }
        checkToMap(map_data);
    }
}
void BossObject::checkToMap(Map& map_data)
{
    int x1 = 0;
    int x2 = 0;

    int y1 = 0;
    int y2 = 0;

    // check chieu ngang
    int height_min = height_threat;

    x1 = (x_pos + x_val) / TILE_SIZE;
    x2 = (x_pos + x_val + width_threat - 1) / TILE_SIZE; // neu ko du thi cham vao bien --> tru di 1

    y1 = y_pos / TILE_SIZE;
    y2 = (y_pos + height_min - 1) / TILE_SIZE;

    if(x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
    {
        if(x_val >= 0)
        {
            int val1 = map_data.tile[y1][x2];
            int val2 = map_data.tile[y2][x2];
            if((val1 != BLANK_TILE && val1 != STATE_MONEY) || (val2 != BLANK_TILE && val2 != STATE_MONEY))
            {
                x_pos = x2 * TILE_SIZE - width_threat - 2;
                x_val = 0;
            }
        }
        else if(x_val < 0)
        {
            int val1 = map_data.tile[y1][x1];
            int val2 = map_data.tile[y2][x1];
            if((val1 != BLANK_TILE && val1 != STATE_MONEY) || (val2 != BLANK_TILE && val2 != STATE_MONEY))
            {
                x_pos = (x1 + 1) * TILE_SIZE;
                x_val = 0;
            }
        }

    }


    // check chieu cao

    int width_min = width_threat;

    x1 = (x_pos) / TILE_SIZE;
    x2 = (x_pos + width_min - 1) / TILE_SIZE; // neu ko du thi cham vao bien --> tru di 1

    y1 = (y_pos + y_val) / TILE_SIZE;
    y2 = (y_pos + y_val + height_threat - 1) / TILE_SIZE;

    if(x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
    {
        if(y_val >= 0)
        {
            int val1 = map_data.tile[y2][x1];
            int val2 = map_data.tile[y2][x2];
            if((val1 != BLANK_TILE && val1 != STATE_MONEY) || (val2 != BLANK_TILE && val2 != STATE_MONEY))
            {
                y_pos = y2 * TILE_SIZE - height_threat - 1;
                y_val = 0;
                onGround = true;
                is_fall = false;
            }
            else
            {
                int new_y2 = (y_pos + y_val + height_threat + 1) / TILE_SIZE;
                int val1 = map_data.tile[new_y2][x1];
                int val2 = map_data.tile[new_y2][x2];
                if((val1 != BLANK_TILE && val1 != STATE_MONEY) || (val2 != BLANK_TILE && val2 != STATE_MONEY))
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
            int val1 = map_data.tile[y1][x1];
            int val2 = map_data.tile[y1][x2];
            if((val1 != BLANK_TILE && val1 != STATE_MONEY) || (val2 != BLANK_TILE && val2 != STATE_MONEY))
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
    if(x_pos + width_threat > map_data.max_x)
    {
        x_pos = map_data.max_x - width_threat - 1;
    }
    if(y_pos < 0)
        y_pos = 0;
//    if(y_pos > map_data.max_y)
//    {
//        is_active = false;
//    }
}
void BossObject::ImproveMoveType(MainObject& Player, Map& map_data, const int& difficult)
{
    if(!is_active)
        return ;
    SDL_Rect rect_ = Player.getRect();
    int result = checkCollision({rect_.x + (Player.getWidthFrame() - WIDTH_CHARACTER) / 2, rect_.y, WIDTH_CHARACTER, HEIGHT_CHARACTER});
    State PlayerState = Player.getState();
//    std::cout << result << '\n';
    if(result)
    {
        inputType.attack_ += animation.Attack.numFrames;
        while(inputType.attack_ >= (2 + difficult) * animation.Attack.numFrames)
        {
            inputType.attack_ -= animation.Attack.numFrames;
        }
        if(inputType.attack_ <= 0)
            inputType.attack_ = 0;
        else
        {
            inputType.right_ = 0;
            inputType.left_ = 0;
        }
        int curFrame = Player.getFrame();
        mainObject::Action curAnimation = Player.getAnimation().currentAnimation;
        if(curAnimation.typeAction == ATTACK)
        {
            switch(result)
            {
            case -1:
            {
                if(Player.getStatus() == WALK_RIGHT)
                {
                    if(curFrame == 5 || curFrame == 9)
                    {
                        // Trung don --> tru HP
                        stateBoss.HealthPoint -= PlayerState.Attack;
                    }
                }
            }
            break ;
            case 1:
            {
                if(Player.getStatus() == WALK_LEFT)
                {
                    if(curFrame == 5 || curFrame == 9)
                    {
                        // Trung don --> tru HP
                        stateBoss.HealthPoint -= PlayerState.Attack;
                    }
                }
            }
            break ;
            case 2:
            {
                if(curFrame == 5 || curFrame == 9)
                {
                    // Trung don --> tru HP
                    stateBoss.HealthPoint -= PlayerState.Attack;
                }
            }
            break ;
            default:
                break ;
            }
        }

        if(inputType.attack_)
        {
            InputAction playerInput = Player.getInputType();

            int frameAttackToCompare = 10;
            switch(result)
            {
            case -1:
            {
                if(status_ == WALK_LEFT)
                {
                    if(frame_ == frameAttackToCompare && delayCounter == 1)
                    {
                        // Trung don --> tru HP
                        PlayerState.HealthPoint -= stateBoss.Attack;
                        Player.setIsHurtByBoss(result);
                    }
                }
            }
            break ;
            case 1:
            {
                if(status_ == WALK_RIGHT)
                {
                    if(frame_ == frameAttackToCompare && delayCounter == 1)
                    {
                        // Trung don --> tru HP
                        PlayerState.HealthPoint -= stateBoss.Attack;
                        Player.setIsHurtByBoss(result);
                    }
                }
            }
            break ;
            case 2:
            {
                if(frame_ == frameAttackToCompare && delayCounter == 1)
                {
                    // Trung don --> tru HP
                    PlayerState.HealthPoint -= stateBoss.Attack;
                    Player.setIsHurtByBoss(result);
                }
            }
            break ;
            default:
                break ;
            }
        }
    }
    else
    {
        inputType.attack_ = 0;
//        PlayerState.HealthPoint -= stateBoss.Attack / 3;
    }
    Player.setState(PlayerState);
    if(stateBoss.HealthPoint <= 0)
    {
        is_dead = true;
        inputType.attack_ = 0;
        inputType.left_ = 0;
        inputType.right_ = 0;
        return ;
    }
    if(x_pos < Player.get_pos_x() && !inputType.attack_)
    {
        if(difficult > EASY && onGround == true)
        {
            inputType.right_ = 1;
            inputType.left_ = 0;
        }
        status_ = WALK_RIGHT;
        inputType.attack_ = 0;
    }
    if(x_pos > Player.get_pos_x() && !inputType.attack_)
    {
        if(difficult > EASY && onGround == true)
        {
            inputType.right_ = 0;
            inputType.left_ = 1;
        }
        inputType.attack_ = 0;
        status_ = WALK_LEFT;
    }

    if(x_pos < map_x)
    {
        x_val = 0;
        x_pos = map_x;
    }

    if(x_pos > map_x + SCREEN_WIDTH)
    {
        x_val = 0;
        x_pos = map_x + SCREEN_WIDTH - width_frame;
    }

}

