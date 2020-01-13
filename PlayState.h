#ifndef ALITA_PLAY_STATE_H
#define ALITA_PLAY_STATE_H

#include "GameState.h"
#include "GameMap.h"
#include "Monster.h"
#include "NPCharacter.h"
#include "Entrance.h"

class PlayState: public GameState{
public:

    virtual bool init(XMLElement *doc) override;

    virtual void render() override;

    const static GameStateType s_gameStateType;

    static GameState *creator();

    virtual const GameStateType &getGameStateType();

    virtual void update() override;

private:

	int m_frame = 0;

	GameMapPtr m_pgm;

	void renderTileLayer();
    void renderLegend();

	void onPlayerMove(IEventDataPtr pEvent);
	void onCreateLevel(IEventDataPtr pEvent);
	void onDestroyGameObject(IEventDataPtr pEvent);
	bool initFromGameMap();

	NPCharacter *createNPC(std::string roleID, Vector2D &initPos);
	Entrance *createEntrance(std::string other, Vector2D &initPos);
	Monster *createMonster(std::string roleID, Vector2D &initPos);

	Vector2D searchAroundEntrance(const Vector2D &center);
	
	void destroy();
};

#endif