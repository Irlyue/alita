#include "MapManager.h"
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
	
}

GameMap *MapManager::create(GameMapID gid){
	GameMap *pMap = new GameMap;
	pMap->init(m_mapPaths[gid]);
	pMap->setGameMapID(gid);
	std::fstream fs("assets\\map.csv", std::fstream::out);
	fs << (*pMap);
	fs.close();
	return pMap;
}