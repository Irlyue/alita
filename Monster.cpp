#include "Monster.h"
#include "Game.h"

const ObjectType Monster::s_type = "Monster";

GameObject *Monster::creator(){
	return new Monster;
}

void Monster::update(){
}

void Monster::draw(){
	m_pAnimation->VDraw();

	auto &levelPos = g_alita->getLevelPos();
	SDL_Rect rect = { m_pos.getX() - levelPos.getX(), m_pos.getY() - levelPos.getY(), 5, 5 };
	SDL_SetRenderDrawColor(g_alita->getRenderer(), 0x00, 0xff, 0x00, 0xff);
	SDL_RenderFillRect(g_alita->getRenderer(), &rect);
}
