#ifndef ALITA_STD_H
#define ALITA_STD_H

#include <string>
#include "SDL.h"
#include "tinyxml2.h"

using TextureID = std::string;
using ObjectType = std::string;
using GameObjectID = std::string;
using tinyxml2::XMLDocument;
using tinyxml2::XMLElement;

#define FILE_DELIMITER "/"
#define ABS(x) (x > 0 ? x : -x)
#define SIGN(x) (x==0 ? 0 : x / ABS(x))
constexpr int FPS = 33;
constexpr double FRAME_TIME = 1000.0 / FPS;

#endif