#include "CallBack.h"

void cbChangeToPlayState(){
    GameStatePtr pState = Game::getInstance()->getStateMachine()->create("PlayState");
    Game::getInstance()->getStateMachine()->changeState(pState);
}

void cbChangeToStartMenuState(){
    GameStatePtr pState = Game::getInstance()->getStateMachine()->create("StartMenuState");
    Game::getInstance()->getStateMachine()->changeState(pState);
}

void cbResume(){
    Game::getInstance()->getStateMachine()->popState();
}

void cbExit(){
    Game::getInstance()->quit();
}

std::map<std::string, CallBackFunc> g_cbFuncMaps = {
    {"cbChangeToPlayState", cbChangeToPlayState},
    {"cbChangeToStartMenuState", cbChangeToStartMenuState},
    {"cbResume", cbResume},
    {"cbExit", cbExit},
};
