#ifndef ALITA_GAME_MAP_H
#define ALITA_GAME_MAP_H

#include <string>
#include <vector>

struct GameMapGrid{
    int floorID = -1;
    int tileID = -1;
    std::string roleID;
    std::string other;
};

class GameMap{
public:
    bool init(std::string path);
    int getRows() const {return m_rows;}
    int getCols() const {return m_cols;}
    std::string getName() const {return m_name;}

    GameMapGrid &operator()(int i, int j);
    
    bool isInsideMap(int i, int j) const;

private:
    std::string m_name;
    int m_rows = 0;
    int m_cols = 0;

    std::vector<GameMapGrid> m_maps;
};


#endif