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

protected:
    int m_playerLives = 0;
    float m_friction = 1;
    float m_acc = 0;
    float m_angle = 0;

    int m_orientation = 0;
    int m_motion = 0;

    SpriteAnimationPtr m_pAnimation;
	SpriteAnimationPtr m_pWeaponAnimation;

    int calcOrientation(const Vector2D &v);
    int calcSector(const Vector2D &v);

    void renderLegend();

	void onMapCreated(IEventDataPtr pEvent);
};

#endif