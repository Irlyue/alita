#ifndef ALITA_TILE_LAYER_H
#define ALITA_TILE_LAYER_H

#include "Level.h"
#include "Vector2D.h"
#include "GameMap.h"

class TileLayer: public Layer{
public:
    friend class LevelParser;

    bool init(std::string path);

    virtual void update();

    virtual void draw();

    int getRows() const {return m_gm.getRows();}
    int getCols() const {return m_gm.getCols();}
    bool isInsideTilemap(int i, int j) const;

private:
    int m_tileWidth = 48;
    int m_tileHeight = 32;  // grid size of one tile

    Vector2D m_pos;
    Vector2D m_acceleration;
    Vector2D m_velocity;

    GameMap m_gm;
};


#endif