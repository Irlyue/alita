#ifndef ALITA_ANIMATION_PLAYER_FACTORY_H
#define ALITA_ANIMATION_PLAYER_FACTORY_H

#include "SpriteAnimation.h"
#include <map>

class SpriteAnimationFactory{
public:
    static SpriteAnimationFactory *getInstance();

    static SpriteAnimationFactory *s_apFactory;

    SpriteAnimationPtr create(AnimationID aid);

    bool init(std::string path);

private:
    SpriteAnimationFactory() = default;

    SpriteOffsetsPtr loadOffsets(std::string path);

    XMLDocument m_doc;
    std::map<AnimationID, XMLElement*> m_animationMaps;
    std::map<std::string, SpriteOffsetsPtr> m_offsetCache;
};

#endif
