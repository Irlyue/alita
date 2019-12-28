#include "TileLayer.h"
#include "Game.h"


void TileLayer::update(){
    m_pos.setX(240);
    m_pos.setY(240);
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
    m_pos += m_velocity;
}


void TileLayer::draw(){
    // i and j denote tile indices
    // x and y denote real pixels

    // draw one more tile in the right and bottom to eliminate some nasty effects
    int visibleCols = Game::getInstance()->getWindowWidth() / m_tileWidth + 2;
    int visibleRows = Game::getInstance()->getWindowHeight() / m_tileHeight + 2;

    int offset_i = m_pos.getY() / m_tileHeight;
    int offset_j = m_pos.getX() / m_tileWidth;
    int offset_x = static_cast<int>(m_pos.getX()) % m_tileWidth;
    int offset_y = static_cast<int>(m_pos.getY()) % m_tileHeight;

    TextureManager *pTextureManager = g_alita->getTextureManager();
    int oi, oj, floorID, tileID, x, y;
    for(int i = 0; i < visibleRows; i++){
        for(int j = 0; j < visibleCols; j++){
            oi = i + offset_i;
            oj = j + offset_j;  // modular to wrap around the tilemap

            // no need to draw the area outside tilemap
            if(!isInsideTilemap(oi, oj))
                continue;

            floorID = m_gm(oi, oj).floorID;
            tileID = m_gm(oi, oj).tileID;
            x = j * m_tileWidth - offset_x;
            y = i * m_tileHeight - offset_y;

            if(floorID != -1){
                pTextureManager->drawTile("FLOOR", floorID, x, y,
                                          m_tileWidth, m_tileHeight, g_alita->getRenderer());
            }
            if(tileID != -1){
                pTextureManager->drawTile("FJ_" + std::to_string(tileID/10000), tileID%10000, x, y,
                                          m_tileWidth, m_tileHeight, g_alita->getRenderer());
            }
        }
    }
}

bool TileLayer::init(std::string path){
    return m_gm.init(path);
}

bool TileLayer::isInsideTilemap(int i, int j) const {
    return m_gm.isInsideMap(i, j);
}