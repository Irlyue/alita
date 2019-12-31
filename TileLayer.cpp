#include "TileLayer.h"
#include "Game.h"


void TileLayer::update(){
    return;

    auto pInputHandler = g_alita->getInputHandler();

    int speed = 2;
    m_acceleration.setX(0);
    m_acceleration.setY(0);

    if(pInputHandler->isKeyDown(SDL_SCANCODE_W)){
        m_acceleration.setY(-speed);
    }else if(pInputHandler->isKeyDown(SDL_SCANCODE_S)){
        m_acceleration.setY(speed);
    }

    if(pInputHandler->isKeyDown(SDL_SCANCODE_A)){
        m_acceleration.setX(-speed);
    }
    if(pInputHandler->isKeyDown(SDL_SCANCODE_D)){
        m_acceleration.setX(speed);
    }
    m_velocity *= 0.9;
    m_velocity += m_acceleration;
}


void TileLayer::draw(){
    // i and j denote tile indices
    // x and y denote real pixels

	int tileWidth = g_alita->getTileWidth();
	int tileHeight = g_alita->getTileHeight();

    // draw one more tile in the right and bottom to eliminate some nasty effects
    int visibleCols = Game::getInstance()->getWindowWidth() / tileWidth + 2;
    int visibleRows = Game::getInstance()->getWindowHeight() / tileHeight + 2;

	Vector2D &pos = m_pParentLevel->getPos();
    int offset_i = pos.getY() / tileHeight;
    int offset_j = pos.getX() / tileWidth;
    int offset_x = static_cast<int>(pos.getX()) % tileWidth;
    int offset_y = static_cast<int>(pos.getY()) % tileHeight;

    TextureManager *pTextureManager = g_alita->getTextureManager();
    int oi, oj, floorID, tileID, x, y;
	GameMap &gm = m_pParentLevel->getGameMap();
    for(int i = 0; i < visibleRows; i++){
        for(int j = 0; j < visibleCols; j++){
            oi = i + offset_i;
            oj = j + offset_j;  // modular to wrap around the tilemap

            // no need to draw the area outside tilemap
            if(!isInsideTilemap(oi, oj))
                continue;

            floorID = gm(oi, oj).floorID;
            tileID = gm(oi, oj).tileID;
            x = j * tileWidth - offset_x;
            y = i * tileHeight - offset_y;

            if(floorID != -1){
                pTextureManager->drawTile("FLOOR", floorID, x, y,
                                          tileWidth, tileHeight, g_alita->getRenderer());
            }
            if(tileID != -1){
                pTextureManager->drawTile("FJ_" + std::to_string(tileID/10000), tileID%10000, x, y,
                                          tileWidth, tileHeight, g_alita->getRenderer());
            }
        }
    }
}

bool TileLayer::isInsideTilemap(int i, int j) const {
    return m_pParentLevel->getGameMap().isInsideMap(i, j);
}