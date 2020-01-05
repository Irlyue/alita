#ifndef ALITA_MONSTER_H
#define ALITA_MONSTER_H

#include "GameObject.h"
#include "SpriteAnimation.h"

class Monster: public GameObject{
public:
	const static ObjectType s_type;

	static GameObject *creator();

	virtual void update();

	virtual void draw();

	virtual const ObjectType &getObjectType() const { return s_type; }

	void setSpriteAnimation(SpriteAnimationPtr pAn) { m_pAnimation = pAn; }

private:

	std::string m_name;

	SpriteAnimationPtr m_pAnimation;
};

#endif
