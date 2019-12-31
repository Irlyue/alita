#ifndef ALITA_OBJECT_LAYER_H
#define ALITA_OBJECT_LAYER_H

#include "Level.h"

class ObjectLayer: public Layer{
public:
	virtual void update();
	virtual void draw();

	std::vector<GameObject*> &getGameObjects() {return m_gameObjects;}

private:
	std::vector<GameObject*> m_gameObjects;
};

#endif
