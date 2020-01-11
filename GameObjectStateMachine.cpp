#include "GameObjectStateMachine.h"
#include "Monster.h"

bool MonsterStateMachine::isEnded() const {
    return m_pAnimation->isFinished();
}

void MonsterStateMachine::enter(int state) {
    m_curState = state;

    int motion = state / 8;
    int orientation = state % 8;
    
    m_pAnimation->VSwitchOrientation(orientation);
    m_pAnimation->VSwitchMotion(motion, m_pMaster->getPos(), {0., 0.});
}

Vector2D MonsterStateMachine::update(){
    return m_pAnimation->VUpdate();
}

void MonsterStateMachine::draw(){
    m_pAnimation->VDraw();
}