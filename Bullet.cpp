#include "Bullet.h"

void Bullet::update(){
    m_pos += m_velocity;
}

bool Bullet::init(const XMLElement *doc){
    GameObject::init(doc);
    m_velocity.setX(doc->FloatAttribute("speed"));
    m_velocity.setY(0.0);
    return true;
}

void Bullet::draw(){
    if(m_frame % 2 == 0)
        GameObject::draw();
}

const ObjectType PlayerBullet::s_type = "PlayerBullet";

GameObject *PlayerBullet::creator(){
    return new PlayerBullet;
}