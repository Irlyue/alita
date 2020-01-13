#include "GameObject.h"
#include "Game.h"

int GameObject::s_defaultGameObjectID = 0;

void GameObject::removeAllDelegates(){
    for(auto pair: m_delegates){
        g_alita->getEventManager()->removeListerner(pair.second, pair.first);
    }

    m_delegates.clear();
}

void GameObject::draw(){
    auto &levelPos = g_alita->getLevelPos();
    Game::getInstance()->getTextureManager()->drawFrame(
        m_textureID, m_pos.getX() - levelPos.getX(), m_pos.getY() - levelPos.getY(), m_width, m_height,
        m_currentRow, m_currentFrame, Game::getInstance()->getRenderer()
    );
}

bool GameObject::init(const XMLElement *doc){
    m_pos = Vector2D(doc->FloatAttribute("x", 0), doc->FloatAttribute("y", 0));
    m_width = doc->IntAttribute("width", 0);
    m_height = doc->IntAttribute("height", 0);
    m_textureID = SAFE_STRING(doc->Attribute("textureID"));

    if(doc->Attribute("id")){
        m_gameObjectID = doc->IntAttribute("id");
    }else{
        s_defaultGameObjectID++;
        m_gameObjectID = s_defaultGameObjectID;
    }

    return true;
}

void GameObject::destroy(){
    removeAllDelegates();
}