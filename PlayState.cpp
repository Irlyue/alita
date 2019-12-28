#include "PlayState.h"
#include "InputHandler.h"
#include "Game.h"

const GameStateType PlayState::s_gameStateType = "PlayState";

GameState *PlayState::creator(){
    return new PlayState;
}

const GameStateType &PlayState::getGameStateType(){
    return s_gameStateType;
}

bool PlayState::init(XMLElement *doc) {
    GameState::init(doc);

    // m_pLevel = (Level*)GameObjectFactory::getInstance()->create("Level");

    // addGameObject(m_pLevel);
    return true;
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
    GameState::render();
}