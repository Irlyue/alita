cmake_minimum_required(VERSION 3.10)
project(alita)

set(CMAKE_CXX_STANDARD 11)

ADD_SUBDIRECTORY(dev/tinyxml2)

include_directories(alita /usr/include/SDL2)
include_directories(alita dev/tinyxml2/include)

add_executable(alita main.cpp Game.cpp InputHandler.cpp Vector2D.cpp
    GameObject.cpp GameObjectFactory.cpp GameState.cpp GameStateMachine.cpp
    MenuButton.cpp MenuState.cpp Player.cpp PlayState.cpp TextureManager.cpp
    CallBack.cpp PauseState.cpp Bullet.cpp PackedImages.cpp GameMap.cpp
    SpriteAnimation.cpp EventManager.cpp EventData.cpp Entrance.cpp
    MapManager.cpp Utility.cpp NPCharacter.cpp Monster.cpp MonsterInfo.cpp
    SpriteAnimationFactory.cpp FastDelegate.h FastDelegateBind.h
    EventData.h EventManager.h Legend.cpp GameObjectStateMachine.cpp
    Brain.cpp)

target_link_libraries(alita SDL2 SDL2_image SDL2_ttf tinyxml2)
target_compile_definitions(alita PRIVATE DEBUG)