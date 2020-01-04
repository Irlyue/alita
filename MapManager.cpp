#include "MapManager.h"
#include "Utility.h"
#include <fstream>


bool MapManager::init(std::string path){
	XMLDocument doc;
	if(doc.LoadFile(path.c_str()) != tinyxml2::XML_SUCCESS){
		printf("Tinyxml failed to load `%s`\n", path.c_str());
		return false;
	}

	std::string rootFolder = SAFE_STRING(doc.FirstChildElement("Maps")->Attribute("folder"));

	for(auto it = doc.FirstChildElement("Maps")->FirstChildElement(); it; it = it->NextSiblingElement()){
		m_mapPaths[it->Attribute("mapID")] = rootFolder + FILE_DELIMITER + it->Attribute("path");
	}
	printf("%d maps loaded\n", m_mapPaths.size());

	return true;
}

void MapManager::destroy(){
	printf("MapManager destroyed!\n");
}

GameMapPtr MapManager::create(GameMapID gid){
	GameMapPtr pMap = GameMapPtr(GCC_NEW GameMap);
	pMap->init(m_mapPaths[gid]);
	pMap->setGameMapID(gid);
	//std::fstream fs(NP("assets/map.csv"), std::fstream::out);
	//fs << (*pMap);
	//fs.close();
	return pMap;
}