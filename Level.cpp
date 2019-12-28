#include "Level.h"
#include "TileLayer.h"

const ObjectType Level::s_type = "Level";

GameObject *Level::creator(){
    return new Level;
}

bool Level::init(const XMLElement *doc){
    TileLayer *p = new TileLayer;
    p->init("/home/wenfeng/vim-files/LPTutorial/LP/Map/0101.map");
    m_layers.push_back(p);
    return true;
}

void Level::update(){
    for(auto &it: m_layers)
        it->update();

}

void Level::draw(){
    for(auto &it: m_layers)
        it->draw();
}