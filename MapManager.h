#ifndef ALITA_MAP_MANAGER_H
#define ALITA_MAP_MANAGER_H

#include "AlitaStd.h"
#include "GameMap.h"
#include <map>

class MapManager{
public:
	~MapManager(){destroy();}
	bool init(std::string path);
	GameMap *create(GameMapID gid);

	void destroy();

private:
	std::map<GameMapID, std::string> m_mapPaths;
};

#endif
