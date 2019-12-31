#ifndef ALITA_PLAY_STATE_H
#define ALITA_PLAY_STATE_H

#include "GameState.h"
#include "GameMap.h"

class PlayState: public GameState{
public:

    virtual bool init(XMLElement *doc) override;

    virtual void render() override;

    const static GameStateType s_gameStateType;

    static GameState *creator();

    virtual const GameStateType &getGameStateType();

    virtual void update() override;

private:

	GameMap *m_pgm = nullptr;

	void renderTileLayer();

	void onPlayerMove(IEventDataPtr pEvent);
	void onCreateLevel(IEventDataPtr pEvent);
	void onDestroyGameObject(IEventDataPtr pEvent);
	bool initFromGameMap();

	Vector2D searchAroundEntrance(const Vector2D &center);
	
	void destroy();
};

#endif