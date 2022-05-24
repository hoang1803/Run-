#include "BaseFunc.h"

void SDLBaseFunc::logSDLError(std::ostream& os,const std::string &msg, bool fatal)
{
    os << msg << " Error: " << SDL_GetError() << std::endl;
    if (fatal)
    {
        SDL_Quit();
        exit(1);
    }
}

void SDLBaseFunc::logIMGError(std::ostream& os,const std::string &msg, bool fatal)
{
    os << msg << " Error: " << IMG_GetError() << std::endl;
    if (fatal)
    {
        SDL_Quit();
        exit(1);
    }
}

void SDLBaseFunc::logMIXError(std::ostream& os,const std::string &msg, bool fatal)
{
    os << msg << " Error: " << Mix_GetError() << std::endl;
    if (fatal)
    {
        SDL_Quit();
        exit(1);
    }
}

void SDLBaseFunc::logTTFError(std::ostream& os,const std::string &msg, bool fatal)
{
    os << msg << " Error: " << TTF_GetError() << std::endl;
    if (fatal)
    {
        SDL_Quit();
        exit(1);
    }
}

void SDLBaseFunc::initSDL(SDL_Window* &window, SDL_Renderer* &renderer)
//              Cua so ve              But ve
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        SDLBaseFunc::logSDLError(std::cout, "SDL_Init", true);
    window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);  // Mở cửa sổ vẽ theo kích thước đã chọn

    //window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED,
    //    SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP);
    if (window == nullptr)
        SDLBaseFunc::logSDLError(std::cout, "CreateWindow", true);
    //Lấy bút vẽ
    //Khi chạy trong môi trường bình thường (không chạy trong máy ảo)
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
                                  SDL_RENDERER_PRESENTVSYNC);
    //Khi chạy ở máy ảo (ví dụ tại máy tính trong phòng thực hành ở trường)
    //renderer = SDL_CreateSoftwareRenderer(SDL_GetWindowSurface(window));
    if (renderer == nullptr)
        SDLBaseFunc::logSDLError(std::cout, "CreateRenderer", true);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);


    if(TTF_Init() == -1)
    {
        logTTFError(std::cout, "TTF Init", 1);
    }

    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1)
    {
        logMIXError(std::cout, "InitMixer", 1);
    }
}

void SDLBaseFunc::quitSDL(SDL_Window* window, SDL_Renderer* renderer)
{
    //Giải phóng bộ nhớ quản lý cửa sổ và bút vẽ
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void SDLBaseFunc::waitUntilKeyPressed()
{
    // Tiện ích: Đợi nhấn phím
    SDL_Event e;
    while (true)
    {
        if ( SDL_WaitEvent(&e) != 0 &&
                (e.type == SDL_KEYDOWN || e.type == SDL_QUIT) ) // Xác định sự kiện bàn phím
            return;
        SDL_Delay(100);
    }
}

int SDLBaseFunc::randInt(int l, int r)
{
    return rand() % (r - l + 1) + l;
}

bool SDLBaseFunc::checkFocusWithRect(const int& x, const int& y, SDL_Rect rect)
{
    if(rect.x <= x && x < rect.x + rect.w
            && rect.y <= y && y < rect.y + rect.h)
        return true;
    return false;
}
