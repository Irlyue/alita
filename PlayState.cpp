#include "PlayState.h"
#include "InputHandler.h"
#include "Game.h"
#include "Utility.h"
#include "NPCharacter.h"
#include "AlitaStd.h"

const GameStateType PlayState::s_gameStateType = "PlayState";

GameState *PlayState::creator(){
    return GCC_NEW PlayState;
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

    auto mapCreated = std::make_shared<MapCreatedEventData>(m_pgm, Vector2D(0, 0));
    g_alita->getEventManager()->queueEvent(mapCreated);

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
            addGameObject(createNPC(roleID, tilePos));
        }

        if(roleID[0] == 'M'){
            addGameObject(createMonster(roleID, tilePos));
        }

        std::string &other = (*m_pgm)(i, j).other;
        if (other != "") {
            auto it = entrances.find(other);
            if (it == entrances.end()) {
                Entrance *pEn = createEntrance(other, tilePos);
                entrances[other] = pEn;
                addGameObject(pEn);
            } else {
                it->second->addGrid(tilePos);
            }
        }
    }
     return true;
}

Monster *PlayState::createMonster(std::string roleID, Vector2D &initPos) {
    std::string monName = roleID.substr(1, roleID.size() - 1);
    
    auto &info = g_alita->getMonsterDB()[monName];
    std::string monID = std::to_string(info.Pic);
    AnimationID aid = "MON_" + std::string(3 - monID.size(), '0') + monID;
    SpriteAnimationPtr m_pAnimation = g_alita->getAnimationPlayerFactory()->create(aid);
    std::static_pointer_cast<MonsterSpriteAnimation>(m_pAnimation)->setMonsterName(monName);
    m_pAnimation->VSwitchMotion(0, initPos, { 0., 0. });
    
    Monster *pm = (Monster*)(g_alita->getGameObjectFactory()->create("Monster"));
    pm->onTheFlyInit(m_pAnimation, initPos);
    return pm;
}

NPCharacter *PlayState::createNPC(std::string roleID, Vector2D &initPos){
    std::string npcType = roleID.substr(0, roleID.size() - 4);
    NPCharacter *pnpc = (NPCharacter*)(g_alita->getGameObjectFactory()->create(npcType));

    AnimationID aid = "NPC_" + roleID.substr(roleID.size() - 3, 3);
    SpriteAnimationPtr m_pAnimation = g_alita->getAnimationPlayerFactory()->create(aid);
    m_pAnimation->VSwitchOrientation(std::stoi(roleID.substr(roleID.size() - 4, 1)) - 1);
    m_pAnimation->VSwitchMotion(0, initPos, { 0., 0. });
    pnpc->setSpriteAnimation(m_pAnimation);

    pnpc->setPos(initPos);
    return pnpc;
}

Entrance *PlayState::createEntrance(std::string other, Vector2D &initPos){
    double tileWidth = g_alita->getTileWidth();
    double tileHeight = g_alita->getTileHeight();

    Entrance *pEn = (Entrance*)(g_alita->getGameObjectFactory()->create(Entrance::s_type));
    
    int enterPosOffset = -1;

    // if "me", switch between the same map, no need to load a new map
    if (toLower(other.substr(0, 2)) == "me") {
        pEn->setToWhere(m_pgm->getGameMapID());
        enterPosOffset = 2;
    }
    else {
        pEn->setToWhere(other.substr(0, 4));
        enterPosOffset = 4;
    }

    Vector2D enterPos(std::stod(other.substr(enterPosOffset, 4)) * tileWidth,
        std::stod(other.substr(enterPosOffset + 4, 4)) * tileHeight);
    pEn->setEnterPos(enterPos);
    pEn->addGrid(initPos);

    return pEn;
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

    // if not the same game map, create a GCC_NEW one
    if(p->getGameMapID() != m_pgm->getGameMapID()){
        for (auto it = m_gameObjects.begin(); it != m_gameObjects.end();) {
            if (it->second->getGameObjectID() == PLAYER_ID) {
                it++;
                continue;
            }

            delete it->second;
            m_gameObjects.erase(it++);
        }

        m_pgm = g_alita->getMapManager()->create(p->getGameMapID());
        initFromGameMap();

    }

    auto destroyObject = std::make_shared<MapCreatedEventData>(m_pgm, searchAroundEntrance(p->getInitPos()));
    g_alita->getEventManager()->triggerEvent(destroyObject);
}

Vector2D PlayState::searchAroundEntrance(const Vector2D &center){
    int ci = static_cast<int>(center.getY() / g_alita->getTileHeight());
    int cj = static_cast<int>(center.getX() / g_alita->getTileWidth());
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

    m_frame++;

    if(InputHandler::getInstance()->isKeyDown(SDL_SCANCODE_ESCAPE)){
        GameStatePtr p = Game::getInstance()->getStateMachine()->create(PauseState::s_gameStateType);
        Game::getInstance()->getStateMachine()->pushState(p);
    }else{
        GameState::update();
    }
}

void PlayState::render(){
    renderTileLayer();
    GameState::render();
    renderLegend();
}

void PlayState::renderLegend(){
    Vector2D playerPos = m_gameObjects[PLAYER_ID]->getPos();
    Vector2D pos(playerPos.getX() / g_alita->getTileWidth(), playerPos.getY() / g_alita->getTileHeight());
    char buf[32];
    sprintf_s(buf, "%s (%3.0f, %3.0f)", m_pgm->getName().c_str(), pos.getX(), pos.getY());
    g_alita->getTextureManager()->drawText(buf, 0, 0, -1, -1,
        { 255, 255, 255, 0 }, g_alita->getRenderer(), g_alita->getFont());
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
    int offset_i = static_cast<int>(pos.getY() / tileHeight);
    int offset_j = static_cast<int>(pos.getX() / tileWidth);
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
                pTextureManager->draw("FLOOR_" + std::to_string(floorID), x, y,
                    tileWidth, tileHeight, g_alita->getRenderer());
            }
            if (tileID != -1) {
                pTextureManager->draw("FJ_" + std::to_string(tileID), x, y,
                    tileWidth, tileHeight, g_alita->getRenderer());
            }
        }
    }
}