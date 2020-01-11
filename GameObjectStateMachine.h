#ifndef ALITA_GAME_OBJECT_STATE_MACHINE_H
#define ALITA_GAME_OBJECT_STATE_MACHINE_H

#include "SpriteAnimation.h"

enum MonsterState{
    NORMAL,
    ATTACKED,
    RUN,
    ATTACK,
    DEAD,
};

class Monster;

class MonsterStateMachine{
public:
    int getCurState() const {return m_curState;}
    bool isEnded() const;
    void enter(int state);
    Vector2D update();
    void draw();
    void setMaster(Monster *pMaster) {m_pMaster = pMaster;}
    void setSpriteAnimation(SpriteAnimationPtr pAnimation) {m_pAnimation = pAnimation;}

private:
    SpriteAnimationPtr m_pAnimation;
    Monster *m_pMaster = nullptr;
    int m_curState = 0;
};

#endif
