#include "PlayState.h"
#include "InputHandler.h"
#include "Game.h"
#include "Utility.h"
#include "NPCharacter.h"

const GameStateType PlayState::s_gameStateType = "PlayState";

GameState *PlayState::creator(){
    return new PlayState;
}

const GameStateType &PlayState::getGameStateType(){
    return s_gameStateType;
}

bool PlayState::init(XMLElement *doc) {
    GameState::init(doc);

	EventListenerDelegate playerMoveDelegate = fastdelegate::MakeDelegate(this, &PlayState::onPlayerMove);
	g_alita->getEventManager()->addListerner(playerMoveDelegate, ObjectMoveEventData::s_eventType);

	EventListenerDelegate createLevelDelegate = fastdelegate::MakeDelegate(this, &PlayState::onCreateLevel);
	g_alita->getEventManager()->addListerner(createLevelDelegate, CreateLevelEventData::s_eventType);

	EventListenerDelegate destroyObjectDelegate = fastdelegate::MakeDelegate(this, &PlayState::onDestroyGameObject);
	g_alita->getEventManager()->addListerner(destroyObjectDelegate, DestroyObjectEventData::s_eventType);

	std::string mapID = SAFE_STRING(doc->Attribute("mapID"));
	m_pgm = g_alita->getMapManager()->create(mapID);

	GameObject *pPlayer = g_alita->getGameObjectFactory()->create("Player");
	addGameObject(pPlayer);

	return initFromGameMap();
}

bool PlayState::initFromGameMap(){

	double tileWidth = g_alita->getTileWidth();
	double tileHeight = g_alita->getTileHeight();

	std::map<std::string, Entrance*> entrances;
	for (int k = 0; k < m_pgm->getRows() * m_pgm->getCols(); k++) {
		int i = k / m_pgm->getCols();
		int j = k % m_pgm->getCols();
		Vector2D tilePos(j * tileWidth, i * tileHeight);

		std::string roleID = (*m_pgm)(i, j).roleID;
		if(roleID[0] == 'N'){
			NPCharacter *pnpc = (NPCharacter*)(g_alita->getGameObjectFactory()->create(NPCharacter::s_type));
			addGameObject(pnpc);

			pnpc->setPos(tilePos);
			TextureID tid = "NPC_" + roleID.substr(roleID.size() - 2, 2);
			pnpc->setTextureID(tid);
		}

		std::string &other = (*m_pgm)(i, j).other;
		if (other != "") {
			auto it = entrances.find(other);
			if (it == entrances.cbegin()) {
				Entrance *pEn = (Entrance*)(g_alita->getGameObjectFactory()->create(Entrance::s_type));
				int enterPosOffset = -1;

				// if "me", switch between the same map, no need to load a new map
				if(toLower(other.substr(0, 2)) == "me"){
					pEn->setToWhere(m_pgm->getGameMapID());
					enterPosOffset = 2;
				}else{
					pEn->setToWhere(other.substr(0, 4));
					enterPosOffset = 4;
				}

				Vector2D enterPos(std::stod(other.substr(enterPosOffset, 4)) * tileWidth,
					std::stod(other.substr(enterPosOffset + 4, 4)) * tileHeight);
				pEn->setEnterPos(enterPos);
				pEn->addGrid(tilePos);
				addGameObject(pEn);
			}
			else {
				it->second->addGrid(tilePos);
			}
		}
	}
	return true;
}

void PlayState::onPlayerMove(IEventDataPtr pEvent) {
	auto p = std::static_pointer_cast<ObjectMoveEventData>(pEvent);
	if (p->GetID() == PLAYER_ID) {
		Vector2D &playerPos = p->getPos();
		Vector2D &levelPos = g_alita->getLevelPos();
		Vector2D windowOffset(g_alita->getWindowWidth() / 2, g_alita->getWindowHeight() / 2);
		levelPos = playerPos - windowOffset;
		levelPos.setX(clip(levelPos.getX(), 0, (m_pgm->getCols() - 1) * g_alita->getTileWidth()));
		levelPos.setY(clip(levelPos.getY(), 0, (m_pgm->getRows() - 1)* g_alita->getTileHeight()));
	}
}

void PlayState::destroy(){
	for(auto it: m_gameObjects){
		delete it.second;
	}
	m_gameObjects.clear();
}

void PlayState::onCreateLevel(IEventDataPtr pEvent) {
	auto p = std::static_pointer_cast<CreateLevelEventData>(pEvent);

	// if not the same game map, create a new one
	if(p->getGameMapID() != m_pgm->getGameMapID()){
		std::vector<GameObjectID> toDelete;
		for (auto it : m_gameObjects) {
			if (it.second->getGameObjectID() == PLAYER_ID) {
				continue;
			}

			it.second->removeAllDelegates();
			toDelete.push_back(it.first);
		}

		delete m_pgm;
		m_pgm = g_alita->getMapManager()->create(p->getGameMapID());
		initFromGameMap();

		for (auto &key : toDelete) {
			auto destroyObject = std::make_shared<DestroyObjectEventData>(key);
			g_alita->getEventManager()->queueEvent(destroyObject);
		}
	}

	// switch to new position
	Vector2D landingPos = searchAroundEntrance(p->getInitPos());
	m_gameObjects[PLAYER_ID]->setPos(landingPos);
	auto pPlayerMoveEvent = std::make_shared<ObjectMoveEventData>(PLAYER_ID, landingPos);
	g_alita->getEventManager()->queueEvent(pPlayerMoveEvent);
}

Vector2D PlayState::searchAroundEntrance(const Vector2D &center){
	int ci = center.getY() / g_alita->getTileHeight();
	int cj = center.getX() / g_alita->getTileWidth();
	Vector2D space = center;
	for(int i = -2; i < 3; i++){
		for(int j = -2; j < 3; j++){
			int di = ci + i;
			int dj = cj + j;
			if((*m_pgm)(di, dj).roleID == "0" && (*m_pgm)(di, dj).other == ""){
				space.setX(dj * g_alita->getTileWidth());
				space.setY(di * g_alita->getTileHeight());
				return space;
			}
		}
	}
	return space;
}

void PlayState::onDestroyGameObject(IEventDataPtr pEvent){
	auto p = std::static_pointer_cast<DestroyObjectEventData>(pEvent);

	auto findIt = m_gameObjects.find(p->GetID());
	if(findIt != m_gameObjects.end()){
		delete findIt->second;

		m_gameObjects.erase(findIt);
	}
}

void PlayState::update(){
    if(InputHandler::getInstance()->isKeyDown(SDL_SCANCODE_ESCAPE)){
        GameState *p = Game::getInstance()->getStateMachine()->create(PauseState::s_gameStateType);
        Game::getInstance()->getStateMachine()->pushState(p);
    }else{
        GameState::update();
    }
}

void PlayState::render(){
	renderTileLayer();
    GameState::render();
}

void PlayState::renderTileLayer(){
	// i and j denote tile indices
// x and y denote real pixels

	int tileWidth = g_alita->getTileWidth();
	int tileHeight = g_alita->getTileHeight();

	// draw one more tile in the right and bottom to eliminate some nasty effects
	int visibleCols = Game::getInstance()->getWindowWidth() / tileWidth + 2;
	int visibleRows = Game::getInstance()->getWindowHeight() / tileHeight + 2;

	Vector2D &pos = g_alita->getLevelPos();
	int offset_i = pos.getY() / tileHeight;
	int offset_j = pos.getX() / tileWidth;
	int offset_x = static_cast<int>(pos.getX()) % tileWidth;
	int offset_y = static_cast<int>(pos.getY()) % tileHeight;

	TextureManager *pTextureManager = g_alita->getTextureManager();
	int oi, oj, floorID, tileID, x, y;
	for (int i = 0; i < visibleRows; i++) {
		for (int j = 0; j < visibleCols; j++) {
			oi = i + offset_i;
			oj = j + offset_j;  // modular to wrap around the tilemap

			// no need to draw the area outside tilemap
			if (!m_pgm->isInsideMap(oi, oj))
				continue;

			floorID = (*m_pgm)(oi, oj).floorID;
			tileID = (*m_pgm)(oi, oj).tileID;
			x = j * tileWidth - offset_x;
			y = i * tileHeight - offset_y;

			if (floorID != -1) {
				pTextureManager->drawTile("FLOOR", floorID, x, y,
					tileWidth, tileHeight, g_alita->getRenderer());
			}
			if (tileID != -1) {
				pTextureManager->drawTile("FJ_" + std::to_string(tileID / 10000), tileID % 10000, x, y,
					tileWidth, tileHeight, g_alita->getRenderer());
			}
		}
	}
}