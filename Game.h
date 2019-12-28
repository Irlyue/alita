#ifndef ALITA_GAME_H
#define ALITA_GAME_H

#include <string>
#include "SDL.h"
#include "SDL_image.h"
#include "GameState.h"
#include "GameStateMachine.h"
#include "TextureManager.h"
#include "GameObjectFactory.h"
#include "InputHandler.h"

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

    XMLDocument *getXMLSettings() const {return m_pDoc;}


    int getWindowWidth() const {return m_windowWidth;}
    int getWindowHeight() const {return m_windowHeight;}

private:
    SDL_Renderer *m_pRenderer = nullptr;
    SDL_Window *m_pWindow = nullptr;

    int m_windowWidth = -1;
    int m_windowHeight = -1;

    XMLDocument *m_pDoc = nullptr;

    bool m_running = false;

    GameStateMachine *m_pGameStateMachine = nullptr;
    TextureManager *m_pTextureManager = nullptr;
    GameObjectFactory *m_pGameObjectFactory = nullptr;
    InputHandler *m_pInputHandler = nullptr;

};

extern Game *g_alita;


#endif