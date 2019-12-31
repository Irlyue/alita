#ifndef ALITA_GAME_OBJECT_H
#define ALITA_GAME_OBJECT_H

#include "tinyxml2.h"
#include "Vector2D.h"
#include "AlitaStd.h"
#include "EventManager.h"
#include <vector>

using CallBackFunc = void(*)();
using DelegatePair = std::pair<EventType, EventListenerDelegate>;

class GameObject{
public:
    virtual bool init(const XMLElement *doc);
	virtual ~GameObject() {destroy();}

    virtual void destroy() {};

    virtual void draw();

    virtual void update() {m_frame++;}

    virtual const ObjectType &getObjectType() const = 0;

    Vector2D &getVelocity() {return m_velocity;}
    Vector2D &getPos() {return m_pos;}

	void setPos(const Vector2D &pos){m_pos = pos;}

    const GameObjectID &getGameObjectID() const {
        return m_gameObjectID;
    }

	void removeAllDelegates();

protected:

	std::vector<DelegatePair> m_delegates;
    static int s_defaultGameObjectID;
    Vector2D m_pos;
    Vector2D m_velocity;
    Vector2D m_acceleration;

    GameObjectID m_gameObjectID;

    int m_width = -1;
    int m_height = -1;

    TextureID m_textureID;
    int m_currentFrame = 0;
    int m_currentRow = 0;

    int m_frame = 0;
};

using GameObjectCreator = GameObject*(*)();

#endif