#include "Legend.h"
#include "Game.h"

const ObjectType Legend::s_type = "Legend";

GameObject *Legend::creator(){
    return GCC_NEW Legend;
}

void Legend::destroy(){
}

bool Legend::init(const XMLElement *doc){
    if(!GameObject::init(doc))
        return false;

    return true;
}

void Legend::draw(){
    Vector2D offset(g_alita->getWindowWidth() / 2, g_alita->getWindowHeight() / 2);
    Vector2D playerPos = g_alita->getLevelPos() + offset;
    char buf[32];
    sprintf_s(buf, "(%.0f, %.0f)", playerPos.getX(), playerPos.getY());

    g_alita->getTextureManager()->drawText(buf, playerPos.getX(), playerPos.getY(), -1, -1,
        {255, 255, 255, 0}, g_alita->getRenderer(), g_alita->getFont());
}