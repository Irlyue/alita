#ifndef ALITA_SPRITE_ANIMATION_H
#define ALITA_SPRITE_ANIMATION_H

#include <vector>
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

struct AnimationConfig{
    int orientation;
    int spriteStart;
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

class SpriteAnimation{
public:
    virtual bool init(const std::string &path);
    virtual void onMotionChanged(Vector2D pos, int motion, int orientation);
    virtual void VDraw();
    bool isMotionPreemptive(int motion) const;
    void nextSprite();
private:
    std::vector<Vector2D> m_offsets;
    std::vector<int> m_motionStack;
    TextureID m_textureID;
    Vector2D m_pos;
    int m_currentFrame = 0;
    int m_spriteOffset = 0;
    int m_frame = 0;
    int m_lastOrientation = 0;

    void changeOffset();
};

class PlayerSpriteAnimation: public SpriteAnimation{
};

class WeaponSpriteAnimation{
public:
};

class MagicAttackSpriteAnimation{

};

class MonsterSpriteAnimation{
public:
};

#endif