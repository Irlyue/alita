#ifndef ALITA_TILE_LAYER_H
#define ALITA_TILE_LAYER_H

#include "Level.h"
#include "Vector2D.h"
#include "GameMap.h"

class TileLayer: public Layer{
public:
    friend class LevelParser;

    virtual void update();

    virtual void draw();

    int getRows() const {return m_pParentLevel->getGameMap().getRows();}
    int getCols() const {return m_pParentLevel->getGameMap().getCols();}
    bool isInsideTilemap(int i, int j) const;

private:

    Vector2D m_acceleration;
    Vector2D m_velocity;



};


#endif