#include "NPCharacter.h"
#include "Game.h"

const ObjectType NPCharacter::s_type = "NPCharacter";

GameObject *NPCharacter::creator(){
    return GCC_NEW NPCharacter;
}

bool NPCharacter::init(const XMLElement *doc){
    GameObject::init(doc);
    return true;
}

void NPCharacter::draw(){
	m_pAnimation->VDraw();
}

void NPCharacter::update(){
	GameObject::update();

	m_pAnimation->VUpdate();
}