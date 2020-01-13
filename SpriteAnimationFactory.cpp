#include "SpriteAnimationFactory.h"
#include "Utility.h"
#include <fstream>

SpriteAnimationFactory *SpriteAnimationFactory::s_apFactory = nullptr;

SpriteAnimationFactory *SpriteAnimationFactory::getInstance(){
    if(!s_apFactory)
        s_apFactory = GCC_NEW SpriteAnimationFactory;
    return s_apFactory;
}

bool SpriteAnimationFactory::init(std::string path){
    if(m_doc.LoadFile(path.c_str())){
        printf("Error loading %s\n", path.c_str());
        return false;
    }

    for(auto it = m_doc.FirstChildElement("Animations")->FirstChildElement(); it; it = it->NextSiblingElement()){
        m_animationMaps[it->Attribute("aid")] = it;
    }

    return true;
}

SpriteAnimationPtr SpriteAnimationFactory::create(AnimationID aid){

    std::string root = m_doc.FirstChildElement("Animations")->Attribute("folder");

    auto findIt = m_animationMaps.find(aid);
    if(findIt == m_animationMaps.end()){
        printf("No such animation as %s\n", aid.c_str());
        return SpriteAnimationPtr();
    }

    auto pSpriteOffsets = loadOffsets(NP(root + "/" + findIt->second->Attribute("soPath")));

    std::string animationType = aid.substr(0, 3);
    if(animationType == "RPG"){
        PlayerSpriteAnimation *ppsa = GCC_NEW PlayerSpriteAnimation;
        ppsa->setAnimationID(aid);
        ppsa->setOffsets(pSpriteOffsets);
        return SpriteAnimationPtr(ppsa);
    }else if(animationType == "WEA"){
        WeaponSpriteAnimation *pwsa = GCC_NEW WeaponSpriteAnimation;
        pwsa->setAnimationID(aid);
        pwsa->setOffsets(pSpriteOffsets);
        return SpriteAnimationPtr(pwsa);
    }else if(animationType == "NPC"){
        NPCSpriteAnimation *pnsa = GCC_NEW NPCSpriteAnimation;
        pnsa->setAnimationID(aid);
        pnsa->setOffsets(pSpriteOffsets);
        return SpriteAnimationPtr(pnsa);
    }else if(animationType == "MON"){
        MonsterSpriteAnimation *pmsa = GCC_NEW MonsterSpriteAnimation;
        pmsa->setAnimationID(aid);
        pmsa->setOffsets(pSpriteOffsets);
        return SpriteAnimationPtr(pmsa);
    }else{
        return SpriteAnimationPtr();
    }
}


SpriteOffsetsPtr SpriteAnimationFactory::loadOffsets(std::string path){
    // if it's already in the cache, just return it.
    auto findIt = m_offsetCache.find(path);
    if(findIt != m_offsetCache.end()){
        return findIt->second;
    }

    // otherwise, read from file
    std::fstream fs;
    fs.open(path, std::fstream::in);

    if (!fs) {
        printf("Error loading %s\n", path.c_str());
    }

    auto offsets = std::make_shared<SpriteOffsets>();
    int x, y;
    char c;
    while (fs >> x >> c >> y >> c) {
        offsets->emplace_back(x, y);
    }
    m_offsetCache[path] = offsets;

    return offsets;
}
