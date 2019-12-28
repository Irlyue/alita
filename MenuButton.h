#ifndef ALITA_MENU_BUTTON_H
#define ALITA_MENU_BUTTON_H

#include "GameObject.h"

enum ButtonStates{
    ON_MOUSE_OUT = 0,
    ON_MOUSE_HOVER = 1,
    ON_MOUSE_CLICKED = 2,
};

class MenuButton: public GameObject{
public:
    virtual bool init(const XMLElement *doc);

    virtual void update();
    
    virtual const ObjectType &getObjectType() const;

    static GameObject *creator();

    static const ObjectType s_type;

private:

    CallBackFunc m_cbFunc = nullptr;
    bool m_released = true;

    bool isMouseInside(const Vector2D &pos);

};


#endif