#include "Entrance.h"
#include "Game.h"

const ObjectType Entrance::s_type = "Entrance";

GameObject *Entrance::creator(){
    return GCC_NEW Entrance;
}

bool Entrance::init(const XMLElement *doc){
    GameObject::init(doc);

    EventListenerDelegate playerMoveDelegate = fastdelegate::MakeDelegate(this, &Entrance::onPlayerMove);
    g_alita->getEventManager()->addListerner(playerMoveDelegate, ObjectMoveEventData::s_eventType);
    m_delegates.push_back({ObjectMoveEventData::s_eventType, playerMoveDelegate});

    return true;
}

void Entrance::draw(){
    SDL_SetRenderDrawColor(g_alita->getRenderer(), 0xff, 0xff, 0xff, 0xff);

    auto &levelPos = g_alita->getLevelPos();
    for(auto &p: m_areas){
        SDL_Rect rect = {p.getX() - levelPos.getX(), p.getY() - levelPos.getY(), 5, 5};
        SDL_RenderFillRect(g_alita->getRenderer(), &rect);
    }
}

void Entrance::addGrid(const Vector2D &grid) {
    m_areas.push_back(grid);
}

void Entrance::onPlayerMove(IEventDataPtr pEvent){
    auto p = std::static_pointer_cast<ObjectMoveEventData>(pEvent);

    if(p->GetID() == PLAYER_ID && isPlayerInsideEntrance(p->getPos())){
        auto pCreateLevelEvent = std::make_shared<CreateLevelEventData>(m_toWhere, m_enterPos);
        g_alita->getEventManager()->queueEvent(pCreateLevelEvent);

    }
}

bool Entrance::isPlayerInsideEntrance(const Vector2D &playerPos){
    int tileWidth = g_alita->getTileWidth();
    int tileHeight = g_alita->getTileHeight();
    bool switchMap = false;
    for (auto &grid : m_areas) {
        Vector2D xy = playerPos - grid;
        int x = static_cast<int>(xy.getX());
        int y = static_cast<int>(xy.getY());
        if (0 <= x && x < tileWidth && 0 <= y && y < tileHeight) {
            switchMap = true;
            break;
        }
    }
    return switchMap;
}