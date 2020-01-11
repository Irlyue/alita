#ifndef ALITA_MONSTER_H
#define ALITA_MONSTER_H

#include "Brain.h"
#include "GameObject.h"
#include "SpriteAnimation.h"
#include "GameObjectStateMachine.h"

class Monster: public GameObject{
public:
    virtual ~Monster() {};

	const static ObjectType s_type;

    void onTheFlyInit(SpriteAnimationPtr pAnimation, Vector2D initPos);

	static GameObject *creator();

	virtual void update();

	virtual void draw();

	virtual const ObjectType &getObjectType() const { return s_type; }

	void setSpriteAnimation(SpriteAnimationPtr pAn) { m_pAnimation = pAn; }

private:

	std::string m_name;

	SpriteAnimationPtr m_pAnimation;

    MonsterStateMachine m_msMachine;

    MonsterBrain m_brain;
};

#endif
