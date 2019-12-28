#ifndef ALITA_GAME_OBJECT_H
#define ALITA_GAME_OBJECT_H

#include "tinyxml2.h"
#include "Vector2D.h"
#include "AlitaStd.h"

using CallBackFunc = void(*)();

class GameObject{
public:
    virtual bool init(const XMLElement *doc);

    virtual void destroy() {};

    virtual void draw();

    virtual void update() {m_frame++;}

    virtual const ObjectType &getObjectType() const = 0;

    Vector2D &getVelocity() {return m_velocity;}
    Vector2D &getPos() {return m_pos;}

    const GameObjectID &getGameObjectID() const {
        return m_gameObjectID;
    }

protected:
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