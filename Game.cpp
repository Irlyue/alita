#include "Game.h"
#include "MenuState.h"
#include "InputHandler.h"
#include "MenuButton.h"
#include "GameObjectFactory.h"


Game *Game::s_game = new Game;
Game *g_alita = Game::getInstance();


Game *Game::getInstance(){
    return s_game;
}


bool Game::init(const std::string &title, int x, int y, int width, int height, bool fullScreen){

    m_windowWidth = width;
    m_windowHeight = height;

    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("SDL_Init error: %s\n", SDL_GetError());
        return false;
    }

    int imgFlags = IMG_INIT_PNG;
    if(!(IMG_Init(imgFlags) & imgFlags)){
        printf("IMG_Init error: %s\n", IMG_GetError());
        return false;
    }

    Uint32 flag = fullScreen ? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_SHOWN;
    m_pWindow = SDL_CreateWindow(title.c_str(), x, y, width, height, flag);
    if(!m_pWindow){
        printf("SDL_CreateWindow error: %s\n", SDL_GetError());
        return false;
    }

    m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_ACCELERATED);
    if(!m_pRenderer){
        printf("SDL_CreateRenderer error: %s\n", SDL_GetError());
        return false;
    }

    m_pInputHandler = InputHandler::getInstance();
    if(!m_pInputHandler->init()){
        printf("Failed to initilize input handler!\n");
        return false;
    }

    m_pDoc = new XMLDocument;
    if(m_pDoc->LoadFile("assets/settings.xml")){
        printf("Failed to load settings file!\n");
        return false;
    }

    m_pTextureManager = new TextureManager();
    if(!m_pTextureManager->init(m_pDoc->FirstChildElement("Textures"))){
        printf("TextureManager failed to initialize!\n");
        // it's ok that some textures are missing since
        // it wouldn't crash the app so we just print out a message
        // without returnning false here.
    }
    SDL_SetWindowIcon(m_pWindow, IMG_Load(m_pTextureManager->getTexturePath("ICON").c_str()));

    m_pGameObjectFactory = GameObjectFactory::getInstance();
    m_pGameObjectFactory->init(m_pDoc);
    m_pGameObjectFactory->add("StartMenuButton", MenuButton::creator);
    m_pGameObjectFactory->add("MenuExitButton", MenuButton::creator);
    m_pGameObjectFactory->add("MenuPlayButton", MenuButton::creator);
    m_pGameObjectFactory->add(Player::s_type, Player::creator);
    m_pGameObjectFactory->add(Level::s_type, Level::creator);

    m_pGameStateMachine = new GameStateMachine;
    m_pGameStateMachine->init(m_pDoc->FirstChildElement("GameStates"));
    m_pGameStateMachine->add(StartMenuState::s_gameStateType, StartMenuState::creator);
    m_pGameStateMachine->add(PlayState::s_gameStateType, PlayState::creator);

    m_pGameStateMachine->changeState(m_pGameStateMachine->create("StartMenuState"));

    m_running = true;
    return true;
}

bool Game::isRunning() const{
    return m_running;
}

void Game::render(){
    // clear the screen
    SDL_SetRenderDrawColor(m_pRenderer, 0xff, 0xff, 0xff, 0xff);
    SDL_RenderClear(m_pRenderer);

    m_pGameStateMachine->render();

    SDL_RenderPresent(m_pRenderer);
}

void Game::update(){
    m_pGameStateMachine->update();
}

void Game::handleEvents(){
    m_pInputHandler->update();
}

void Game::destroy(){
    m_pGameStateMachine->destroy();
    m_pTextureManager->destroy();
    m_pInputHandler->destroy();

    SDL_DestroyRenderer(m_pRenderer);
    SDL_DestroyWindow(m_pWindow);
    IMG_Quit();
    SDL_Quit();
}

void Game::quit(){
    m_running = false;
}