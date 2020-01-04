#include "Legend.h"
#include "Game.h"

const ObjectType Legend::s_type = "Legend";

GameObject *Legend::creator(){
	return GCC_NEW Legend;
}

void Legend::destroy(){
}

bool Legend::init(const XMLElement *doc){
	if(!GameObject::init(doc))
		return false;

	return true;
}

void Legend::draw(){
	return;
	TTF_Font *pfont = g_alita->getFont();
	SDL_Renderer *pRenderer = g_alita->getRenderer();
	SDL_Color textColor = {255, 255, 255, 0};
	Vector2D &levelPos = g_alita->getLevelPos();
	char buf[32];
	sprintf_s(buf, "(%.0f, %.0f)", levelPos.getX(), levelPos.getY());
	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(pfont, buf, textColor); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first

	SDL_Texture* Message = SDL_CreateTextureFromSurface(pRenderer, surfaceMessage);
	SDL_Rect dst = {0, 0, 100, 30};
	SDL_RenderCopy(pRenderer, Message, nullptr, &dst);

	SDL_FreeSurface(surfaceMessage);
	SDL_DestroyTexture(Message);
}