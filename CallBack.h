#ifndef ALITA_CALLBACK_H
#define ALITA_CALLBACK_H

#include "Game.h"
#include <map>

extern std::map<std::string, CallBackFunc> g_cbFuncMaps;

void cbChangeToPlayState();

#endif