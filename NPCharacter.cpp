#include "NPCharacter.h"
#include "Game.h"

const ObjectType NPCharacter::s_type = "NPCharacter";

GameObject *NPCharacter::creator(){
    return new NPCharacter;
}

bool NPCharacter::init(const XMLElement *doc){
    GameObject::init(doc);
    return true;
}

void NPCharacter::draw(){
    Vector2D &levelPos = g_alita->getLevelPos();
    g_alita->getTextureManager()->drawTile(m_textureID, 0, m_pos.getX() - levelPos.getX(), m_pos.getY() - levelPos.getY(),
        -1, -1, g_alita->getRenderer());
}