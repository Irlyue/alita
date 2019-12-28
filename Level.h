#ifndef ALITA_LEVEL_H
#define ALITA_LEVEL_H

#include <vector>
#include <string>
#include "GameObject.h"


class Layer{
public:
    virtual void draw() = 0;
    virtual void update() = 0;
};


class Level: public GameObject{
public:
    const static ObjectType s_type;
    
    static GameObject *creator();

    virtual bool init(const XMLElement *doc) override;

    std::vector<Layer*> *getLayers() {return &m_layers;}

    virtual void draw() override;
    virtual void update() override;

    virtual const ObjectType &getObjectType() const{
        return s_type;
    }

private:
    std::vector<Layer*> m_layers;
};


#endif