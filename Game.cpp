#include "Game.h"
#include "MenuState.h"
#include "InputHandler.h"
#include "MenuButton.h"
#include "GameObjectFactory.h"


Game *Game::s_game = GCC_NEW Game;
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

	if(TTF_Init() == -1){
		printf("TTF_Init error: %s\n", TTF_GetError());
	}
	m_pFont = TTF_OpenFont("assets/simsun.ttc", 14);

    Uint32 flag = fullScreen ? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_SHOWN;
    m_pWindow = SDL_CreateWindow(title.c_str(), x, y, width, height, flag);
    if(!m_pWindow){
        printf("SDL_CreateWindow error: %s\n", SDL_GetError());
        return false;
    }

    m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_ACCELERATED);
	//m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_SOFTWARE);
    if(!m_pRenderer){
        printf("SDL_CreateRenderer error: %s\n", SDL_GetError());
        return false;
    }

    m_pInputHandler = InputHandler::getInstance();
    if(!m_pInputHandler->init()){
        printf("Failed to initilize input handler!\n");
        return false;
    }

    if(m_doc.LoadFile("assets/settings.xml")){
        printf("Failed to load settings file!\n");
        return false;
    }

	m_pEventManager = EventManager::getInstance();

    m_pTextureManager = GCC_NEW TextureManager;
    if(!m_pTextureManager->init(m_doc.FirstChildElement("Textures"))){
        printf("TextureManager failed to initialize!\n");
        // it's ok that some textures are missing since
        // it wouldn't crash the app so we just print out a message
        // without returnning false here.
    }
    //SDL_SetWindowIcon(m_pWindow, IMG_Load(m_pTextureManager->getTexturePath("ICON").c_str()));

	m_pMapManager = GCC_NEW MapManager;
	if(!m_pMapManager->init("assets/maps.xml")){
		printf("MapManager failed to initialize\n");
		return false;
	}

	m_pAPFactory = SpriteAnimationFactory::getInstance();
	if(!m_pAPFactory->init("assets/animation.xml")){
		printf("AnimationPlayerFactory failed to initialize\n");
		return false;
	}
	if(!m_monsterDB.init("assets/Mon1.txt")){
		printf("MonsterDB failed to initialize\n");
		return false;
	}

	EventListenerDelegate levelMoveDelegate = fastdelegate::MakeDelegate(this, &Game::onLevelMove);
	m_pEventManager->addListerner(levelMoveDelegate, ObjectMoveEventData::s_eventType);

    m_pGameObjectFactory = GameObjectFactory::getInstance();
    m_pGameObjectFactory->init(m_doc.FirstChildElement("GameObjects"));
    m_pGameObjectFactory->add("StartMenuButton", MenuButton::creator);
    m_pGameObjectFactory->add("MenuExitButton", MenuButton::creator);
    m_pGameObjectFactory->add("MenuPlayButton", MenuButton::creator);
    m_pGameObjectFactory->add("MenuResumeButton", MenuButton::creator);
    m_pGameObjectFactory->add("MenuMainButton", MenuButton::creator);
    m_pGameObjectFactory->add(Player::s_type, Player::creator);
	m_pGameObjectFactory->add(Entrance::s_type, Entrance::creator);
	m_pGameObjectFactory->add(Monster::s_type, Monster::creator);

	for(auto it: m_pGameObjectFactory->getGameObjectInfos()){
		if(it.first[0] == 'N')
		    m_pGameObjectFactory->add(it.first, NPCharacter::creator);
	}
	
	m_pGameObjectFactory->add(Legend::s_type, Legend::creator);

    m_pGameStateMachine = GCC_NEW GameStateMachine;
    m_pGameStateMachine->init(m_doc.FirstChildElement("GameStates"));
    m_pGameStateMachine->add(StartMenuState::s_gameStateType, StartMenuState::creator);
    m_pGameStateMachine->add(PlayState::s_gameStateType, PlayState::creator);
    m_pGameStateMachine->add(PauseState::s_gameStateType, PauseState::creator);

    m_pGameStateMachine->changeState(m_pGameStateMachine->create("StartMenuState"));

    m_running = true;
    return true;
}

void Game::onLevelMove(IEventDataPtr pEvent) {
	auto p = std::static_pointer_cast<ObjectMoveEventData>(pEvent);
	if (p->GetID() == LEVEL_ID) {
		m_levelPos = p->getPos();
	}
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
	m_pEventManager->update();
}

void Game::handleEvents(){
    m_pInputHandler->update();
}

void Game::destroy(){
    delete m_pGameStateMachine;
	delete m_pGameObjectFactory;
	delete m_pEventManager;
	delete m_pAPFactory;
	delete m_pMapManager;
    delete m_pTextureManager;
    delete m_pInputHandler;

    SDL_DestroyRenderer(m_pRenderer);
    SDL_DestroyWindow(m_pWindow);
	TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void Game::quit(){
    m_running = false;
}