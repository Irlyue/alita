#ifndef ALITA_LEGEND_H
#define ALITA_LEGEND_H
#include "GameObject.h"
#include "SDL_ttf.h"

class Legend: public GameObject{
public:
    const static ObjectType s_type;

    static GameObject *creator();

    virtual const ObjectType &getObjectType() const {return s_type;}

    ~Legend() {destroy();}

    virtual bool init(const XMLElement *doc);

    virtual void draw();

    virtual void destroy();

};
#endif // !ALITA_LEGEND_H
