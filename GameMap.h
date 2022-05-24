#ifndef GAME_MAP_H_
#define GAME_MAP_H_


#include "BaseFunc.h"
#include "BaseObject.h"
#define MAX_TILES 100

class TileMat : public BObject
{
public:
    TileMat() {;}
    ~TileMat() {;}
};


class GameMap
{
public:
    GameMap() {;}
    ~GameMap() {;}

    void LoadMap(const std::string& file_name);
    void LoadTiles(SDL_Renderer* renderer);
    void DrawMap(SDL_Renderer* renderer);

    Map getGameMap() const  {return game_map;}
    void setMap(const Map& map_data) {game_map = map_data;}

private:
    Map game_map;
    TileMat tile_mat[MAX_TILES];
};

#endif // GAME_MAP_H_
