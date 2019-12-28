#include "InputHandler.h"
#include "MenuButton.h"
#include "Game.h"

extern std::map<std::string, CallBackFunc> g_cbFuncMaps;

const ObjectType MenuButton::s_type = "MenuButton";

GameObject *MenuButton::creator(){
    return new MenuButton;
}

const ObjectType &MenuButton::getObjectType() const {
    return s_type;
}

bool MenuButton::init(const XMLElement *doc){
    GameObject::init(doc);

    if(doc->Attribute("callback")){
        m_cbFunc = g_cbFuncMaps.find(doc->Attribute("callback"))->second;
    }

    m_currentFrame = 0;
    m_currentRow = 0;
    return true;
}

void MenuButton::update(){
    if(isMouseInside(InputHandler::getInstance()->getMousePos())){
        m_currentFrame = ON_MOUSE_HOVER;
        if(InputHandler::getInstance()->getBtnState(SDL_BUTTON_LEFT)){
            m_released = false;
            m_currentFrame = ON_MOUSE_CLICKED;
        }else if(!m_released && !InputHandler::getInstance()->getBtnState(SDL_BUTTON_LEFT)){
            m_released = true;
            m_currentFrame = ON_MOUSE_HOVER;
            if(m_cbFunc){
                m_cbFunc();
            }
        }
    }else{
        m_currentFrame = ON_MOUSE_OUT;
        m_released = true;
    }
}

bool MenuButton::isMouseInside(const Vector2D &mousePos){
    Vector2D ul = m_pos;
    Vector2D br = m_pos + Vector2D(m_width, m_height);
    return (ul.getX() <= mousePos.getX() && mousePos.getX() <= br.getX() &&
            ul.getY() <= mousePos.getY() && mousePos.getY() <= br.getY());
}
