#include "GameMap.h"

void GameMap::LoadMap(const std::string& file_name)
{
    std::ifstream file(file_name);
    if(!file)
    {
        std::cout << "ErrorLoadMap: Cannot open file!";
        SDL_Quit();
        exit(1);
    }

    game_map.max_x = game_map.max_y = 0;
    game_map.start_x = game_map.start_y = 0;
    game_map.file_name = file_name;

    for(int i = 0; i < MAX_MAP_Y; i++)
        for(int j = 0; j < MAX_MAP_X; j++)
        {
            file >> game_map.tile[i][j];
            if(game_map.tile[i][j] > 0)
            {
                if(j > game_map.max_x)
                {
                    game_map.max_x = j;
                }
                if(i > game_map.max_y)
                {
                    game_map.max_y = i;
                }
            }
        }
    file.close();

    game_map.max_x = (game_map.max_x + 1) * TILE_SIZE;
    game_map.max_y = (game_map.max_y + 1) * TILE_SIZE;
}

void GameMap::LoadTiles(SDL_Renderer* renderer)
{
    for(int i = 0; i < MAX_TILES; i++)
    {
        std::ifstream file("map//" + std::to_string(i) + ".png");
        if(!file)
        {
            continue ;
        }
        file.close();
        tile_mat[i].loadImage("map//" + std::to_string(i) + ".png", renderer);
    }
}

void GameMap::DrawMap(SDL_Renderer* renderer)
{
    int x1, x2;
    int y1, y2;
    int pos_map_x, pos_map_y;

    x1 = -(game_map.start_x % TILE_SIZE);
    x2 = x1 + SCREEN_WIDTH + (x1 == 0 ? 0 : TILE_SIZE);
    pos_map_x = game_map.start_x / TILE_SIZE;

    y1 = -(game_map.start_y % TILE_SIZE);
    y2 = y1 + SCREEN_HEIGHT + (y1 == 0 ? 0 : TILE_SIZE);
    pos_map_y = game_map.start_y / TILE_SIZE;

    for(int i = y1; i < y2; i += TILE_SIZE)
    {
        pos_map_x = game_map.start_x / TILE_SIZE;
        for(int j = x1; j < x2; j += TILE_SIZE)
        {
            int val = game_map.tile[pos_map_y][pos_map_x++];
            if(val)
            {
                tile_mat[val].setCoordinates(j, i);
                tile_mat[val].render(renderer);
            }
        }
        ++pos_map_y;
    }

}
