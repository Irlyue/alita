#ifndef ALITA_LEVEL_H
#define ALITA_LEVEL_H

#include <vector>
#include <string>
#include "GameObject.h"
#include "GameMap.h"
#include "EventManager.h"

class Level;

class Layer{
public:
    virtual void draw() = 0;
    virtual void update() = 0;
	void setParentLevel(Level *pLevel) {m_pParentLevel = pLevel;}
protected:
	Level *m_pParentLevel = nullptr;
};


class Level: public GameObject{
public:
    const static ObjectType s_type;
    
    static GameObject *creator();

	virtual void destroy();

    virtual bool init(const XMLElement *doc) override;

    std::vector<Layer*> *getLayers() {return &m_layers;}
	GameMap &getGameMap() {return m_gm;}

    virtual void draw() override;
    virtual void update() override;

    virtual const ObjectType &getObjectType() const{
        return s_type;
    }

	// delegates
	void onPlayerMove(IEventDataPtr pEvent);

private:
    std::vector<Layer*> m_layers;
	GameMap m_gm;
};


#endif