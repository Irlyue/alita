#ifndef ALITA_PLAYER_H
#define ALITA_PLAYER_H

#include "GameObject.h"
#include "SpriteAnimation.h"


class Player: public GameObject{
public:
    const static ObjectType s_type;

    static GameObject *creator();

    virtual void update();

    virtual void draw();

    virtual bool init(const XMLElement *doc);

    int getPlayerLives() const {
        return m_playerLives;
    }

    const ObjectType &getObjectType() const {
        return s_type;
    }

    bool isFireTimeIrlegal() const;

protected:
    int m_playerLives = 0;
    float m_friction = 1;
    float m_acc = 0;
    float m_angle = 0;

    Uint32 m_lastFireTime = 0;
    Uint32 m_waitFireTime = 0;

    int m_lastOrientation = 0;
    int m_lastMotion = MOTION_STILL;
    int m_nbSprites = NB_MOTION_SPRITES[MOTION_STILL];
    int m_spriteOffset = 0;
    
    Vector2D m_spriteOrigin;

    SpriteAnimation *m_pPlayerAnimation = nullptr;

    int calcOrientation(const Vector2D &v);
    int calcSector(const Vector2D &v);
};

#endif