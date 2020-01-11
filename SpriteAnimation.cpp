#include "SpriteAnimation.h"
#include "Game.h"
#include <cmath>
#include <fstream>

void SpriteAnimation::VDraw(){
	auto *pt = g_alita->getTextureManager();
	auto &offset = (*m_pOffsets)[m_spriteIndex];
	Vector2D &levelPos = g_alita->getLevelPos();

	int x = static_cast<int>(m_pos.getX() + offset.getX() - levelPos.getX());
	int y = static_cast<int>(m_pos.getY() + offset.getY() - levelPos.getY());

	pt->draw(m_aid + "_" + std::to_string(m_spriteIndex), x, y, -1, -1, g_alita->getRenderer());
}


const std::vector<Vector2D> PlayerSpriteAnimation::VELOCITIES = {
		{0, 0}, {48 * 1, 32 * 1}, {48 * 2, 32 * 3}, {0, 0}, {0, 0}, {0, 0}, {0, 0}
};

const std::vector<int> PlayerSpriteAnimation::SPRITE_FRAMES = {
	8, 1, 4, 4, 4, 4
};

Vector2D PlayerSpriteAnimation::VUpdate(){
	m_frame++;
	if(m_frame >= m_nbTotalFrames - 1){
		m_finished = true;
	}

	m_spriteIndex = m_spriteOffset + m_frame / SPRITE_FRAMES[m_motion];
	m_pos += m_velocity;
	return m_pos;
}

void PlayerSpriteAnimation::VSwitchMotion(int motion, const Vector2D &initPos, const Vector2D &acc){
	m_finished = false;
	m_frame = 0;
	m_motion = motion;
	m_pos = initPos;

	// compute sprite offset
	m_spriteOffset = 0;
	for (int i = 0; i < m_motion; i++) {
		m_spriteOffset += NB_MOTION_SPRITES[i] * 8;
	}
	m_spriteOffset += NB_MOTION_SPRITES[m_motion] * m_orientation;

	m_nbSprites = NB_MOTION_SPRITES[m_motion];
	m_nbTotalFrames = SPRITE_FRAMES[m_motion] * NB_MOTION_SPRITES[m_motion];

	m_velocity = VELOCITIES[m_motion] / m_nbTotalFrames;
	m_velocity.setX(m_velocity.getX() * acc.getX());
	m_velocity.setY(m_velocity.getY() * acc.getY());
}

void PlayerSpriteAnimation::VSwitchOrientation(int ori){
	m_orientation = ori;
}


void PlayerSpriteAnimation::VDraw(){
	SpriteAnimation::VDraw();
}


// NPC

const int NPCSpriteAnimation::NB_SPRITE_FRAMES = 12;

void NPCSpriteAnimation::VDraw(){
	SpriteAnimation::VDraw();
}

void NPCSpriteAnimation::VSwitchOrientation(int ori){
	m_orientation = ori;
}

void NPCSpriteAnimation::VSwitchMotion(int motion, const Vector2D &initPos, const Vector2D &acc){
	m_finished = false;
	m_frame = 0;
	m_motion = motion;
	m_pos = initPos;

	m_spriteOffset = m_orientation;
}

Vector2D NPCSpriteAnimation::VUpdate(){
	m_frame++;

	m_spriteIndex = m_spriteOffset + m_frame / NB_SPRITE_FRAMES % 4;
	return m_pos;
}

// Monster

const int MonsterSpriteAnimation::NB_MOTION_SPRITES[6] = {4, 6, 6, 10};
const int MonsterSpriteAnimation::SPRITE_FRAMES[6] = { 12, 6, 6, 10 };
const std::vector<Vector2D> MonsterSpriteAnimation::VELOCITIES = {
    {0, 0}, {48 * 1, 32 * 1}, {48 * 2, 32 * 2}, {0, 0}, {0, 0}, {0, 0}, {0, 0}
};

void MonsterSpriteAnimation::VDraw(){
	SpriteAnimation::VDraw();
}

void MonsterSpriteAnimation::VSwitchOrientation(int ori){
    m_orientation = ori;
}

void MonsterSpriteAnimation::VSwitchMotion(int motion, const Vector2D &initPos, const Vector2D &acc){
    m_finished = false;
    m_frame = 0;
    m_motion = motion;
    m_pos = initPos;

    // compute sprite offset
    m_spriteOffset = 0;
    for (int i = 0; i < m_motion; i++) {
        m_spriteOffset += NB_MOTION_SPRITES[i] * 8;
    }
    m_spriteOffset += NB_MOTION_SPRITES[m_motion] * m_orientation;

    m_nbSprites = NB_MOTION_SPRITES[m_motion];
    m_nbTotalFrames = SPRITE_FRAMES[m_motion] * NB_MOTION_SPRITES[m_motion];

    m_velocity = VELOCITIES[m_motion] / m_nbTotalFrames;
    m_velocity.setX(m_velocity.getX() * acc.getX());
    m_velocity.setY(m_velocity.getY() * acc.getY());
}

Vector2D MonsterSpriteAnimation::VUpdate(){
    m_frame++;
    if (m_frame == m_nbTotalFrames - 1) {
        m_finished = true;
    }

    m_spriteIndex = m_spriteOffset + m_frame / SPRITE_FRAMES[m_motion];
    m_pos += m_velocity;
    return m_pos;
}