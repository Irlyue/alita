#ifndef ALITA_GAME_MAP_H
#define ALITA_GAME_MAP_H

#include "AlitaStd.h"
#include <iostream>
#include <string>
#include <vector>
#include <memory>


struct GameMapGrid{
    int floorID = -1;
    int tileID = -1;
    std::string roleID = "";
    std::string other = "";
};

class GameMap{
public:
	~GameMap(){destroy();}
	void destroy();
    bool init(std::string path);
    int getRows() const {return m_rows;}
    int getCols() const {return m_cols;}
    std::string getName() const {return m_name;}
	GameMapID getGameMapID() {return m_gid;}

	void setGameMapID(GameMapID &gid) {m_gid = gid;}

    GameMapGrid &operator()(int i, int j);
    
    bool isInsideMap(int i, int j) const;

	friend std::ostream &operator<<(std::ostream &os, GameMap &gm);

private:
	GameMapID m_gid;
    std::string m_name;
    int m_rows = 0;
    int m_cols = 0;

    std::vector<GameMapGrid> m_maps;
};


using GameMapPtr = std::shared_ptr<GameMap>;

#endif