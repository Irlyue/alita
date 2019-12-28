#include "GameObject.h"
#include "Game.h"

int GameObject::s_defaultGameObjectID = 0;

void GameObject::draw(){
    Game::getInstance()->getTextureManager()->drawFrame(
        m_textureID, m_pos.getX(), m_pos.getY(), m_width, m_height,
        m_currentRow, m_currentFrame, Game::getInstance()->getRenderer()
    );
}

bool GameObject::init(const XMLElement *doc){
    m_pos = Vector2D(doc->FloatAttribute("x"), doc->FloatAttribute("y"));
    m_width = doc->IntAttribute("width");
    m_height = doc->IntAttribute("height");
    m_textureID = doc->Attribute("textureID");

    if(doc->Attribute("id")){
        m_gameObjectID = doc->Attribute("id");
    }else{
        s_defaultGameObjectID++;
        m_gameObjectID = std::to_string(s_defaultGameObjectID);
    }

    return true;
}