#include "Monster.h"
#include "Game.h"
#include <cstdlib>

const ObjectType Monster::s_type = "Monster";

GameObject *Monster::creator(){
	return new Monster;
}

void Monster::onTheFlyInit(SpriteAnimationPtr pAnimation, Vector2D initPos){
    m_pAnimation = pAnimation;
    m_pos = initPos;

    m_msMachine.setMaster(this);
    m_msMachine.setSpriteAnimation(m_pAnimation);

    int x = rand() % 10;
    for(int i = 0; i < x; i++) m_pAnimation->VUpdate();
}

void Monster::update(){
    if(m_msMachine.isEnded()){
        m_msMachine.enter(m_brain.nextState(m_msMachine.getCurState()));
    }else{
        m_pos = m_msMachine.update();
    }
}

void Monster::draw(){
	m_msMachine.draw();

	auto &levelPos = g_alita->getLevelPos();
	SDL_Rect rect = { m_pos.getX() - levelPos.getX(), m_pos.getY() - levelPos.getY(), 5, 5 };
	SDL_SetRenderDrawColor(g_alita->getRenderer(), 0x00, 0xff, 0x00, 0xff);
	SDL_RenderFillRect(g_alita->getRenderer(), &rect);
}
