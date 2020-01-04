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

	std::vector<Vector2D> spriteOffsets = loadOffsets(NP(root + "/" + findIt->second->Attribute("soPath")));

	std::string animationType = aid.substr(0, 3);
	if(animationType == "RPG"){
		PlayerSpriteAnimation *ppsa = GCC_NEW PlayerSpriteAnimation;
		ppsa->setAnimationID(aid);
		ppsa->setOffsets(spriteOffsets);
		return SpriteAnimationPtr(ppsa);
	}else if(animationType == "NPC"){
		NPCSpriteAnimation *pnsa = GCC_NEW NPCSpriteAnimation;
		pnsa->setAnimationID(aid);
		pnsa->setOffsets(spriteOffsets);
		return SpriteAnimationPtr(pnsa);
	}else if(animationType == "Monster"){
		return SpriteAnimationPtr();
	}else{
		return SpriteAnimationPtr();
	}
}


std::vector<Vector2D> SpriteAnimationFactory::loadOffsets(std::string path){
	std::fstream fs;
	fs.open(path, std::fstream::in);

	if (!fs) {
		printf("Error loading %s\n", path.c_str());
	}

	std::vector<Vector2D> offsets;
	int x, y;
	char c;
	while (fs >> x >> c >> y >> c) {
		offsets.emplace_back(x, y);
	}

	return offsets;
}
