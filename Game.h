#ifndef ALITA_GAME_H
#define ALITA_GAME_H

#include <string>
#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"
#include "GameState.h"
#include "GameStateMachine.h"
#include "TextureManager.h"
#include "GameObjectFactory.h"
#include "InputHandler.h"
#include "EventManager.h"
#include "MapManager.h"
#include "SpriteAnimationFactory.h"
#include "MonsterInfo.h"

class Game{
public:
    static Game *s_game;
    static Game *getInstance();

    bool init(const std::string &title, int x, int y, int width, int height, bool fullScreen=false);

    void render();
    void update();
    void handleEvents();
    void destroy();

    bool isRunning() const;
    void quit();

    SDL_Renderer *getRenderer() {return m_pRenderer;}

    TextureManager *getTextureManager() {return m_pTextureManager;}
    InputHandler *getInputHandler() {return m_pInputHandler;}
    GameStateMachine *getStateMachine() const {return m_pGameStateMachine;}
	GameObjectFactory *getGameObjectFactory() const {return m_pGameObjectFactory;}
	EventManager *getEventManager() {return m_pEventManager;}
	Vector2D &getLevelPos() {return m_levelPos;}
	MapManager *getMapManager() {return m_pMapManager;}
	TTF_Font *getFont() {return m_pFont;}
	SpriteAnimationFactory *getAnimationPlayerFactory() {return m_pAPFactory;}
	MonsterDB &getMonsterDB() {return m_monsterDB;}

    int getWindowWidth() const {return m_windowWidth;}
    int getWindowHeight() const {return m_windowHeight;}
	int getTileWidth() const {return m_tileWidth;}
	int getTileHeight() const {return m_tileHeight;}

private:
    SDL_Renderer *m_pRenderer = nullptr;
    SDL_Window *m_pWindow = nullptr;
	Vector2D m_levelPos;
	TTF_Font *m_pFont = nullptr;

    int m_windowWidth = -1;
    int m_windowHeight = -1;
	int m_tileWidth = 48;
	int m_tileHeight = 32;

    XMLDocument m_doc;

    bool m_running = false;

    GameStateMachine *m_pGameStateMachine = nullptr;
    TextureManager *m_pTextureManager = nullptr;
    GameObjectFactory *m_pGameObjectFactory = nullptr;
    InputHandler *m_pInputHandler = nullptr;
	EventManager *m_pEventManager = nullptr;
	MapManager *m_pMapManager = nullptr;
	SpriteAnimationFactory *m_pAPFactory = nullptr;

	MonsterDB m_monsterDB;

	void onLevelMove(IEventDataPtr pEvent);
};

extern Game *g_alita;


#endif