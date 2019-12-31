#include "Level.h"
#include "TileLayer.h"
#include "Game.h"
#include "ObjectLayer.h"
#include "Entrance.h"
#include <map>

const ObjectType Level::s_type = "Level";

GameObject *Level::creator(){
    return new Level;
}

void Level::destroy(){
	for(auto p: m_layers){
		delete p;
	}
	m_layers.clear();

	printf("Level destroyed!\n");
}

void Level::onPlayerMove(IEventDataPtr pEvent){
	auto p = std::static_pointer_cast<ObjectMoveEventData>(pEvent);
	if(p->GetID() == PLAYER_ID){
		Vector2D &pos = p->getPos();
		Vector2D windowOffset(g_alita->getWindowWidth() / 2, g_alita->getWindowHeight() / 2);
		m_pos = pos - windowOffset;
		if(m_pos.getX() < 0) m_pos.setX(0);
		if(m_pos.getY() < 0) m_pos.setY(0);

		auto pLevelMoveEvent = std::make_shared<ObjectMoveEventData>(getGameObjectID(), m_pos);
		g_alita->getEventManager()->queueEvent(pLevelMoveEvent);
	}

}

bool Level::init(const XMLElement *doc){
	m_gm.init(doc->Attribute("mapPath"));
	m_gameObjectID = LEVEL_ID;

	EventListenerDelegate playerMoveDelegate = fastdelegate::MakeDelegate(this, &Level::onPlayerMove);
	g_alita->getEventManager()->addListerner(playerMoveDelegate, ObjectMoveEventData::s_eventType);

    TileLayer *pTileLayer = new TileLayer;
	pTileLayer->setParentLevel(this);
    m_layers.push_back(pTileLayer);

	ObjectLayer *pObjectLayer = new ObjectLayer;
	m_layers.push_back(pObjectLayer);

	bool playerPlaced = false;
	std::map<std::string, Entrance*> entrances;
	for(int k = 0; k < m_gm.getRows() * m_gm.getCols(); k++){
		int i = k / m_gm.getCols();
		int j = k % m_gm.getCols();

		if(!playerPlaced && m_gm(i, j).other != ""){
			playerPlaced = true;
			printf("(%d %d)\n", i, j);
			int offset_y = i * g_alita->getTileHeight();
			int offset_x = j * g_alita->getTileWidth();

			GameObject *pPlayer = g_alita->getGameObjectFactory()->create("Player");
			pObjectLayer->getGameObjects().push_back(pPlayer);
			Vector2D playerPos(offset_x, offset_y);
			pPlayer->setPos(playerPos);

			auto pPlayerMoveEvent = std::make_shared<ObjectMoveEventData>(pPlayer->getGameObjectID(), pPlayer->getPos());
			g_alita->getEventManager()->queueEvent(pPlayerMoveEvent);
		}

		std::string &other = m_gm(i, j).other;
		if (other != "") {
			auto it = entrances.find(other);
			if (it == entrances.cbegin()) {
				Entrance *pEn = (Entrance*)(g_alita->getGameObjectFactory()->create(Entrance::s_type));
				pEn->setToWhere(other.substr(0, 4));
				pEn->setEnterPos({ std::stod(other.substr(4, 4)), std::stod(other.substr(8, 4)) });
				pEn->addGrid({(double)j, (double)i});
				pObjectLayer->getGameObjects().push_back(pEn);
			}
			else {
				it->second->addGrid({(double)j, (double)i});
			}

		}
	}
    return true;
}

void Level::update(){
    for(auto &it: m_layers)
        it->update();

}

void Level::draw(){
    for(auto &it: m_layers)
        it->draw();
}