#include "SpriteAnimation.h"
#include "Game.h"
#include <cmath>
#include <fstream>


bool SpriteAnimation::init(const std::string &id){
    m_textureID = "RPG_0";
    m_motionStack.push_back(MOTION_STILL);

    std::fstream fs;
    fs.open("/home/wenfeng/Desktop/LPFCQDJB31/Date/Rpg/26.Zb", std::fstream::in);

    if(!fs){
        printf("Error\n");
        return false;
    }

    int x, y;
    char c;
    while(fs >> x >> c >> y >> c){
        m_offsets.emplace_back(x, y);
    }
    return true;
}

void SpriteAnimation::onMotionChanged(Vector2D pos, int motion, int orientation){
    m_pos = pos;
    int lastMotion = m_motionStack.back();
    // orientation changed
    bool changed = false;
    if(orientation != -1 && orientation != m_lastOrientation){
        changed = true;
        m_lastOrientation = orientation;
    }

    // motion changed
    if(motion != lastMotion){
        if(isMotionPreemptive(motion) || !isMotionPreemptive(lastMotion)){
            changed = true;

            m_motionStack.clear();
            m_motionStack.push_back(MOTION_STILL);
            m_motionStack.push_back(motion);
        }
    }

    if(changed){
        changeOffset();
    }

    nextSprite();
}

void SpriteAnimation::changeOffset(){
    m_frame = 0;
    m_spriteOffset = 0;
    int lastMotion = m_motionStack.back();
    for(int i = 0; i < lastMotion; i++){
        m_spriteOffset += NB_MOTION_SPRITES[i] * 8;
    }
    m_spriteOffset += NB_MOTION_SPRITES[lastMotion] * m_lastOrientation;
}

void SpriteAnimation::nextSprite(){
    m_currentFrame = m_frame / 4;
    int lastMotion = m_motionStack.back();
    if(isMotionPreemptive(lastMotion) && m_frame && m_currentFrame == NB_MOTION_SPRITES[lastMotion]){
        m_motionStack.pop_back();
        changeOffset();
    }else{
        m_currentFrame %= NB_MOTION_SPRITES[lastMotion];
    }

    m_frame++;
}

bool SpriteAnimation::isMotionPreemptive(int motion) const {
    if(motion == MOTION_MAGIC_ATTACK || motion == MOTION_DIRECT_ATTACK){
        return true;
    }
    return false;
}

void SpriteAnimation::VDraw(){
    int idx = m_spriteOffset + m_currentFrame;
    auto *pt = g_alita->getTextureManager();
    auto &offset = m_offsets[idx];

    pt->drawTile(m_textureID, idx, m_pos.getX() + offset.getX(), m_pos.getY() + offset.getY(), -1, -1, g_alita->getRenderer());
}