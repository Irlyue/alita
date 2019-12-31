#ifndef ALITA_NON_PLAYER_CHARACTER_H
#define ALITA_NON_PLAYER_CHARACTER_H

#include "GameObject.h"

class NPCharacter: public GameObject{
public:
    const static ObjectType s_type;

    static GameObject *creator();

    virtual bool init(const XMLElement *doc);

    virtual void draw();

    virtual const ObjectType &getObjectType() const {return s_type;}

private:

    std::string m_name;
};

#endif