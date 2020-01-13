#ifndef ALITA_ANIMATION_PLAYER_H
#define ALITA_ANIMATION_PLAYER_H

#include "Vector2D.h"
#include "AlitaStd.h"
#include <vector>
#include <memory>

class IAnimationPlayer{
public:
    virtual void VSwitchTo(int motion, const Vector2D &initPos) = 0;
    virtual void VDraw() = 0;
    virtual Vector2D VUpdate() = 0;
    virtual bool VIsFinished() const = 0;
};

using IAnimationPlayerPtr = std::shared_ptr<IAnimationPlayer>;

class AnimationPlayer: public IAnimationPlayer{
public:
    AnimationPlayer(AnimationID aid): m_aid(aid) {}
    virtual void VSwitchTo(int motion, const Vector2D &initPos);
    virtual void VDraw();
    virtual Vector2D VUpdate();
    virtual bool VIsFinished() const;

    void setSpriteOffsets(const std::vector<int> &offsets) {m_spriteOffsets = offsets;}
    void setMotionOffsets(const std::vector<int> &offsets) {m_motionOffsets = offsets;}

protected:
    std::vector<int> m_spriteOffsets;
    std::vector<int> m_motionOffsets;
    int m_frame = 0;
    int m_spriteIndex = 0;
    int m_spriteOffset = 0;
    int m_nbSprites = 0;
    AnimationID m_aid;
    bool m_finished = true;
    Vector2D m_pos;
    Vector2D m_velocity;
};

#endif
