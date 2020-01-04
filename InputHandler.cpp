#include "InputHandler.h"
#include "Game.h"

InputHandler *InputHandler::s_pInputHandler = nullptr;

InputHandler *InputHandler::getInstance(){
    if(!s_pInputHandler){
        s_pInputHandler = GCC_NEW InputHandler;
    }
    return s_pInputHandler;
}

bool InputHandler::init(){
    int btns[] = {SDL_BUTTON_LEFT, SDL_BUTTON_MIDDLE, SDL_BUTTON_RIGHT};
    for(auto &btn: btns){
        m_btnStateMaps[btn] = false;
    }

    m_pKeyStates = SDL_GetKeyboardState(nullptr);
    m_pKeyReleaseStates = GCC_NEW Uint8[256];
    return true;
}

bool InputHandler::getBtnState(int btn) const {
    auto it = m_btnStateMaps.find(btn);
    return (it == m_btnStateMaps.cend()) ? false : it->second;
}

void InputHandler::update(){
    SDL_Event event;

    memset(m_pKeyReleaseStates, 0, sizeof(Uint8) * 256);

    while(SDL_PollEvent(&event)){
        switch(event.type){
            case SDL_QUIT:
                Game::getInstance()->quit();
                break;
            case SDL_KEYDOWN:
                onKeyDown(event);
                break;
            case SDL_KEYUP:
                onKeyUp(event);
                break;
            case SDL_MOUSEMOTION:
                onMouseMove(event);
                break;
            case SDL_MOUSEBUTTONDOWN:
                onMouseButtonDown(event);
                break;
            case SDL_MOUSEBUTTONUP:
                onMouseButtonUp(event);
                break;
            default:
                break;
        }
    }
}

bool InputHandler::isKeyDown(SDL_Scancode key) const{
    if(m_pKeyStates){
        return m_pKeyStates[key] == 1;
    }
    return false;
}

bool InputHandler::isKeyReleased(SDL_Scancode key) const {
    return m_pKeyReleaseStates[key];
}

void InputHandler::onMouseMove(SDL_Event &event){
    m_mousePos.setX(event.motion.x);
    m_mousePos.setY(event.motion.y);
}

void InputHandler::onMouseButtonDown(SDL_Event &event){
    m_btnStateMaps[event.button.button] = true;
}

void InputHandler::onMouseButtonUp(SDL_Event &event){
    m_btnStateMaps[event.button.button] = false;
}


void InputHandler::onKeyDown(SDL_Event &event){
    m_pKeyStates = SDL_GetKeyboardState(nullptr);
}

void InputHandler::onKeyUp(SDL_Event &event){
    m_pKeyStates = SDL_GetKeyboardState(nullptr);
    m_pKeyReleaseStates[event.key.keysym.scancode] = true;
}

void InputHandler::destroy(){
    m_pKeyStates = nullptr;
    delete[] m_pKeyReleaseStates;

    printf("InputHandler destroyed!\n");
}