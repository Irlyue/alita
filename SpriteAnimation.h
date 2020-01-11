#ifndef ALITA_SPRITE_ANIMATION_H
#define ALITA_SPRITE_ANIMATION_H

#include <vector>
#include <memory>
#include "AlitaStd.h"
#include "Vector2D.h"

enum Orientation{
    ORI_ORIENTI_UPPER=0,
    ORI_UPPER_RIGHT=1,
    ORI_RIGHT=2,
    ORI_BOTTOM_RIGHT=3,
    ORI_BOTTOM=4,
    ORI_BOTTOM_LEFT=5,
    ORI_LEFT=6,
    ORI_UPPER_LEFT=7,
    ORI_TOTAL,
};

enum Motion{
    MOTION_STILL=0,
    MOTION_WALK=1,
    MOTION_RUN=2,
    MOTION_DIRECT_ATTACK=3,
    MOTION_MAGIC_ATTACK=4,
    MOTION_ATTACKED=5,
    MOTION_DYING=6,
    MOTION_TOTAL,
};

constexpr int ORIENTATIONS[] = {
  //0, 1, 2,  3, 4,  5, 6,  7, 8, 9, 10, 11, 12, 13, 14, 15
    7, 6, 5, -1, 0, -1, 4, -1, 1, 2, 3, -1, -1, -1, -1, -1
};

constexpr int NB_MOTION_SPRITES[] = {
    4, 6, 6, 6, 6, 3, 4
};
using SpriteOffsets = std::vector<Vector2D>;
using SpriteOffsetsPtr = std::shared_ptr<SpriteOffsets>;

class SpriteAnimation{
public:
	virtual ~SpriteAnimation() {}
	virtual void VDraw();
	virtual void VSwitchOrientation(int ori) = 0;
	virtual void VSwitchMotion(int motion, const Vector2D &initPos, const Vector2D &acc) = 0;
	virtual Vector2D VUpdate() = 0;

	void setAnimationID(AnimationID aid) {m_aid = aid;}
	void setOffsets(SpriteOffsetsPtr offsets) {m_pOffsets = offsets;}
	bool isFinished() const { return m_finished; }
	int getMotion() const {return m_motion;}

protected:
	AnimationID m_aid;
	Vector2D m_velocity;
	Vector2D m_pos;
	SpriteOffsetsPtr m_pOffsets;

	bool m_finished = true;
	int m_motion = 0;
	int m_orientation = 0;
	int m_nbTotalFrames = 0;
	int m_spriteIndex = 0;
	int m_spriteOffset = 0;
	int m_frame = 0;
	int m_nbSprites = 1;
};

using SpriteAnimationPtr = std::shared_ptr<SpriteAnimation>;

class PlayerSpriteAnimation: public SpriteAnimation{
public:

	virtual void VDraw();
	virtual void VSwitchOrientation(int ori);
	virtual void VSwitchMotion(int motion, const Vector2D &initPos, const Vector2D &acc);
	virtual Vector2D VUpdate();
	
protected:
	static const std::vector<Vector2D> VELOCITIES;
	static const std::vector<int> SPRITE_FRAMES;
};

class WeaponSpriteAnimation: public PlayerSpriteAnimation {
};

class NPCSpriteAnimation: public SpriteAnimation{
public:
	virtual void VDraw();
	virtual void VSwitchOrientation(int ori);
	virtual void VSwitchMotion(int motion, const Vector2D &initPos, const Vector2D &acc);
	virtual Vector2D VUpdate();

protected:
	static const int NB_SPRITE_FRAMES;
};

class MonsterSpriteAnimation: public SpriteAnimation{
public:
    const static int NB_MOTION_SPRITES[6];
    const static int SPRITE_FRAMES[6];
    const static std::vector<Vector2D> VELOCITIES;

	virtual void VDraw();
	virtual void VSwitchOrientation(int ori);
	virtual void VSwitchMotion(int motion, const Vector2D &initPos, const Vector2D &acc);
	virtual Vector2D VUpdate();
};


#endif