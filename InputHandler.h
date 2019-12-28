#ifndef ALITA_INPUT_HANDLER_H
#define ALITA_INPUT_HANDLER_H

#include "SDL.h"
#include "Vector2D.h"
#include <map>

class InputHandler{
public:
    static InputHandler *getInstance();

    ~InputHandler() {destroy();}

    bool init();

    void update();
    void destroy();

    const Vector2D &getMousePos() const {return m_mousePos;}

    bool getBtnState(int btn) const;

    bool isKeyDown(SDL_Scancode key) const;

    bool isKeyReleased(SDL_Scancode key) const;

private:
    static InputHandler *s_pInputHandler;

    InputHandler() = default;

    const Uint8 *m_pKeyStates = nullptr;
    Uint8 *m_pKeyReleaseStates = nullptr;
    Vector2D m_mousePos;
    std::map<int, bool> m_btnStateMaps;

    void onKeyDown(SDL_Event &event);
    void onKeyUp(SDL_Event &event);

    void onMouseMove(SDL_Event &event);
    void onMouseButtonDown(SDL_Event &event);
    void onMouseButtonUp(SDL_Event &event);

};


#endif