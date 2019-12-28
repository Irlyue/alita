#include "Player.h"
#include "Game.h"
#include "InputHandler.h"
#include <cmath>

const ObjectType Player::s_type = "Player";


GameObject *Player::creator(){
    return new Player;
}

bool Player::init(const XMLElement *doc){
    GameObject::init(doc);

    m_playerLives = doc->IntAttribute("lives");
    m_acc = doc->FloatAttribute("acceleration");
    m_friction = doc->FloatAttribute("friction");
    m_waitFireTime = doc->Unsigned64Attribute("waitFireTime");

    m_pos = {100, 100};
    m_currentFrame = 0;

    m_pPlayerAnimation = new PlayerSpriteAnimation;
    m_pPlayerAnimation->init("");
    return true;

}

static float clip(float a, float l, float r){
    if (a < l){
        return l;
    }else if(a > r){
        return r;
    }else{
        return a;
    }

}

int Player::calcOrientation(const Vector2D &v){
    int x = SIGN(v.getX()) + 1, y = SIGN(v.getY()) + 1;
    return ORIENTATIONS[x * 4 + y];
}

int Player::calcSector(const Vector2D &v){
    double degree = atan2(v.getY(), v.getX()) * 180 / M_PI;
    degree = degree > 0 ? degree : 360 + degree;

    double sum = -22.5;
    int i;
    for(i = 0; i < 8; i++){
        sum += 45;
        if(sum > degree){
            break;
        }
    }
    return (i + 2) % 8;
}

void Player::update(){
    GameObject::update();
    auto pInputHandler = InputHandler::getInstance();

    m_acceleration.setX(0);
    m_acceleration.setY(0);

    int motion = MOTION_STILL;
    if(pInputHandler->isKeyDown(SDL_SCANCODE_W)){
        m_acceleration.setY(-m_acc);
    }else if(pInputHandler->isKeyDown(SDL_SCANCODE_S)){
        m_acceleration.setY(m_acc);
    }

    if(pInputHandler->isKeyDown(SDL_SCANCODE_A)){
        m_acceleration.setX(-m_acc);
    }else if(pInputHandler->isKeyDown(SDL_SCANCODE_D)){
        m_acceleration.setX(m_acc);
    }

    int orientation = calcOrientation(m_acceleration);
    if(!m_acceleration.isZero()){
        motion = MOTION_RUN;
    }

    if(pInputHandler->getBtnState(SDL_BUTTON_RIGHT)){
        orientation = calcSector(pInputHandler->getMousePos() - m_pos);
        motion = MOTION_MAGIC_ATTACK;
    }else if(pInputHandler->getBtnState(SDL_BUTTON_LEFT)){
        orientation = calcSector(pInputHandler->getMousePos() - m_pos);
        motion = MOTION_DIRECT_ATTACK;
    }

    m_acceleration.normalize();
    m_acceleration *= m_acc;

    m_velocity *= m_friction;
    m_velocity +=  m_acceleration;
    m_pos += m_acceleration;
    m_pPlayerAnimation->onMotionChanged(m_pos, motion, orientation);

    m_currentFrame = (m_frame / 4) % m_nbSprites;
}

bool Player::isFireTimeIrlegal() const {
    return SDL_GetTicks() - m_lastFireTime > m_waitFireTime;
}

void Player::draw(){
    // Vector2D wh = g_alita->getTextureManager()->getTextureSize("RPG_0", m_spriteOffset + m_currentFrame);
    // g_alita->getTextureManager()->drawTile("RPG_0", m_spriteOffset + m_currentFrame,
    //                                        m_pos.getX(), m_pos.getY(), -1, -1, g_alita->getRenderer());
    m_pPlayerAnimation->VDraw();

    SDL_Rect rect = {m_pos.getX(), m_pos.getY(), 5, 5};
    SDL_SetRenderDrawColor(g_alita->getRenderer(), 0xff, 0x00, 0x00, 0xff);
    SDL_RenderFillRect(g_alita->getRenderer(), &rect);
}