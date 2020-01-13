#ifndef ALITA_STD_H
#define ALITA_STD_H

#include <string>
#include "SDL.h"
#include "tinyxml2.h"

using GameObjectID = int;
using AnimationID = std::string;
using GameMapID = std::string;
using TextureID = std::string;
using ObjectType = std::string;
using EventType = unsigned long long;
using tinyxml2::XMLDocument;
using tinyxml2::XMLElement;

#if defined DEBUG
#if defined WIN32
#define GCC_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#else
#define GCC_NEW new
#endif
#else
#define GCC_NEW new
#endif


#define SAFE_STRING(x) (x ? x : "")

#ifdef WIN32
#define FILE_DELIMITER "\\"
#else
#define FILE_DELIMITER "/"
#define sprintf_s sprintf
#endif

#define ABS(x) (x > 0 ? x : -x)
#define SIGN(x) (x==0 ? 0 : x / ABS(x))
constexpr int FPS = 33;
constexpr double FRAME_TIME = 1000.0 / FPS;
constexpr int PLAYER_ID = -1;
constexpr int LEVEL_ID = -1000;

#endif