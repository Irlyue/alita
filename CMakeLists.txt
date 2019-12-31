cmake_minimum_required(VERSION 3.10)
project(alita)

set(CMAKE_CXX_STANDARD 11)

include_directories(alita /usr/include/SDL2)
include_directories(alita dev/tinyxml2/include)
link_directories(dev/tinyxml2/lib)

add_executable(alita main.cpp Game.cpp InputHandler.cpp Vector2D.h
    GameObject.cpp GameObjectFactory.cpp GameState.cpp GameStateMachine.cpp
    MenuButton.cpp MenuState.cpp Player.cpp PlayState.cpp TextureManager.cpp
    CallBack.cpp PauseState.cpp Bullet.cpp PackedImages.cpp GameMap.cpp
    SpriteAnimation.cpp EventManager.cpp EventData.cpp Entrance.cpp
    MapManager.cpp Utility.cpp NPCharacter.cpp)

target_link_libraries(alita SDL2 SDL2_image tinyxml2)