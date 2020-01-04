#include "Player.h"
#include "Game.h"
#include "InputHandler.h"
#include <cmath>

const ObjectType Player::s_type = "Player";


GameObject *Player::creator(){
    return GCC_NEW Player;
}

bool Player::init(const XMLElement *doc){
    GameObject::init(doc);

    m_playerLives = doc->IntAttribute("lives");
    m_acc = doc->FloatAttribute("acceleration");
    m_friction = doc->FloatAttribute("friction");

    m_pos = {10 * 48, 10 * 32};
    m_currentFrame = 0;

    m_pAnimation = g_alita->getAnimationPlayerFactory()->create("RPG_1");
	m_pAnimation->VSwitchMotion(MOTION_STILL, m_pos, {0., 0.});

	EventListenerDelegate mapCreatedDelegate = fastdelegate::MakeDelegate(this, &Player::onMapCreated);
	g_alita->getEventManager()->addListerner(mapCreatedDelegate, MapCreatedEventData::s_eventType);

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

	Vector2D runVelocity(1, 1);
	Vector2D walkVelocity(1, 1);

	if(m_pAnimation->getMotion() == MOTION_STILL || m_pAnimation->isFinished()){

		int motion = MOTION_STILL;
		m_acceleration.setX(0.);
		m_acceleration.setY(0.);

		// MOTION_RUN
		if (pInputHandler->isKeyDown(SDL_SCANCODE_W)) {
			motion = MOTION_RUN;
			m_acceleration.setY(-runVelocity.getY());
		}else if (pInputHandler->isKeyDown(SDL_SCANCODE_S)) {
			motion = MOTION_RUN;
			m_acceleration.setY(runVelocity.getY());
		}

		if (pInputHandler->isKeyDown(SDL_SCANCODE_A)) {
			motion = MOTION_RUN;
			m_acceleration.setX(-runVelocity.getX());
		}else if (pInputHandler->isKeyDown(SDL_SCANCODE_D)) {
			motion = MOTION_RUN;
			m_acceleration.setX(runVelocity.getX());
		}
		int orientation = calcOrientation(m_acceleration);

		if (pInputHandler->getBtnState(SDL_BUTTON_RIGHT)) {
			orientation = calcSector(pInputHandler->getMousePos() + g_alita->getLevelPos() - m_pos);
			motion = MOTION_MAGIC_ATTACK;
		}else if (pInputHandler->getBtnState(SDL_BUTTON_LEFT)) {
			orientation = calcSector(pInputHandler->getMousePos() + g_alita->getLevelPos() - m_pos);
			motion = MOTION_DIRECT_ATTACK;
		}

		if(orientation != -1){
			m_pAnimation->VSwitchOrientation(orientation);
		}

		if(motion != MOTION_STILL || m_pAnimation->isFinished()){
			m_pAnimation->VSwitchMotion(motion, m_pos, m_acceleration);
		}else{
			m_pos = m_pAnimation->VUpdate();
		}

	}else{
		m_pos = m_pAnimation->VUpdate();
	}

	auto pPlayerMoveEvent = std::make_shared<ObjectMoveEventData>(getGameObjectID(), getPos());
	g_alita->getEventManager()->queueEvent(pPlayerMoveEvent);
}

void Player::draw(){
    // Vector2D wh = g_alita->getTextureManager()->getTextureSize("RPG_0", m_spriteOffset + m_currentFrame);
    // g_alita->getTextureManager()->drawTile("RPG_0", m_spriteOffset + m_currentFrame,
    //                                        m_pos.getX(), m_pos.getY(), -1, -1, g_alita->getRenderer());
    m_pAnimation->VDraw();

	auto &levelPos = g_alita->getLevelPos();
    SDL_Rect rect = {m_pos.getX() - levelPos.getX(), m_pos.getY() - levelPos.getY(), 5, 5};
    SDL_SetRenderDrawColor(g_alita->getRenderer(), 0xff, 0x00, 0x00, 0xff);
    SDL_RenderFillRect(g_alita->getRenderer(), &rect);
}

void Player::onMapCreated(IEventDataPtr pEvent){
	auto p = std::static_pointer_cast<MapCreatedEventData>(pEvent);
	// switch to new position
	m_pos = p->getInitPos();
	m_pAnimation->VSwitchMotion(MOTION_STILL, m_pos, {0., 0.});

	auto pPlayerMoveEvent = std::make_shared<ObjectMoveEventData>(PLAYER_ID, m_pos);
	g_alita->getEventManager()->queueEvent(pPlayerMoveEvent);
}