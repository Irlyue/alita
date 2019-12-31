#include "ObjectLayer.h"

void ObjectLayer::draw(){
	for(auto it: m_gameObjects){
		it->draw();
	}
}


void ObjectLayer::update(){
	for(auto it: m_gameObjects){
		it->update();
	}
}

