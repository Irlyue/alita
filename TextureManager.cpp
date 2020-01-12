#include "TextureManager.h"
#include "Game.h"
#include "Utility.h"
#include <set>

// Helper function to load texture from an image file
// Arguments
// ---------
//     path: path to the image file
// Return
//     a SDL_Texture* giving the loaded texture if success, otherwise a null pointer.
static SDL_Texture *loadTexture(const std::string &path){
    SDL_Surface *pSurface = IMG_Load(path.c_str());
    if(!pSurface){
        printf("IMG_Load error: %s\n", IMG_GetError());
        return nullptr;
    }
    SDL_Texture *pTexture = SDL_CreateTextureFromSurface(Game::getInstance()->getRenderer(), pSurface);
    SDL_FreeSurface(pSurface);
    return pTexture;
}


bool TextureManager::loadGlobalTextures(){
    auto doc = m_pTextureInfos;
    std::string root = doc->Attribute("folder");
    int successCount = 0, failCount = 0;
    bool success = true;

    for(auto it = doc->FirstChildElement(); it; it = it->NextSiblingElement()){
        std::string id = it->Attribute("id");
        std::string type = it->Value();
        if(type == "PackedImages"){
			m_packedImagesInfos[id] = it;
        }else if(type == "Texture"){
            SDL_Texture *pTexture = loadTexture(NP(root + FILE_DELIMITER + it->Attribute("path")));
			m_textureMaps[id] = pTexture;
            if(pTexture){
                successCount++;
            }else{
                failCount++;
                success = false;
            }
		}
    }
    printf("Load textures, %d successes and %d failures!\n", successCount, failCount);

	// global textrues
	loadPackedImages("GAME", false);
	loadPackedImages("FLOOR", false);
	loadPackedImages("RPG_1", false);
	loadPackedImages("WEA_002", false);
	loadPackedImages("WEA_040", false);
	
    return success;
}

bool TextureManager::init(XMLElement *doc){
    m_pTextureInfos = doc;

	EventListenerDelegate mapCreatedDelegate = fastdelegate::MakeDelegate(this, &TextureManager::onPreLoadTextures);
	g_alita->getEventManager()->addListerner(mapCreatedDelegate, MapCreatedEventData::s_eventType);

	return loadGlobalTextures();
}


// Find the texture specified by the texture ID
// Return
//     If not found, a null pointer is returned.
SDL_Texture *TextureManager::getTexture(const TextureID &tid){
    auto it = m_textureMaps.find(tid);
    if(it != m_textureMaps.cend()){
        return it->second;
    }
    return nullptr;
}


void TextureManager::drawFrame(const TextureID &tid, int x, int y, int w, int h,
                               int currentRow, int currentFrame, SDL_Renderer *pRenderer,
                               int angle, SDL_RendererFlip flip){
    SDL_Rect src = {w * currentFrame, h * currentRow, w, h};
    SDL_Rect dst = {x, y, w, h};
    SDL_RenderCopyEx(pRenderer, getTexture(tid), &src, &dst, angle, 0, flip);
}

void TextureManager::draw(const TextureID &tid, int x, int y, int w, int h, SDL_Renderer *pRenderer) {
	SDL_Texture *p = getTexture(tid);
	if (w == -1 || h == -1) {
		SDL_QueryTexture(p, nullptr, nullptr, &w, &h);
	}

	SDL_Rect src = { 0, 0, w, h };
	SDL_Rect dst = { x, y, w, h };
	SDL_RenderCopy(pRenderer, p, &src, &dst);
}


// Draw utf-8 text
// Arguments
// ---------
//     text: utf-8 encoded characters
//     x, y: pixel position on the screen to draw the text
//     w, h: width and height of the rendered text, set it to -1 to copy the whole rendered text.
//     textColor: color of the text, {R, G, B, A}
void TextureManager::drawText(const std::string &text, int x, int y, int w, int h, SDL_Color textColor,
    SDL_Renderer *pRenderer, TTF_Font *pFont){

    SDL_Surface *pSurface = TTF_RenderUTF8_Solid(pFont, text.c_str(), textColor);
    SDL_Texture *pMessage = SDL_CreateTextureFromSurface(pRenderer, pSurface);
    if(w == -1 || h == -1){
        w = pSurface->w;
        h = pSurface->h;
    }
    SDL_Rect dst = {0, 0, w, h};
    SDL_RenderCopy(pRenderer, pMessage, nullptr, &dst);

    SDL_FreeSurface(pSurface);
    SDL_DestroyTexture(pMessage);
}

void TextureManager::destroy(){
    for(auto &it: m_textureMaps){
        SDL_DestroyTexture(it.second);
    }
    m_textureMaps.clear();

    m_packedImagesInfos.clear();

    printf("TextureManager destroyed!\n");
}

std::shared_ptr<PackedImages> TextureManager::createPackedImages(std::string id){
	XMLElement *it = m_packedImagesInfos[id];
	std::string root = m_pTextureInfos->Attribute("folder");
	std::string binaryPath = NP(root + FILE_DELIMITER + it->Attribute("binaryPath"));
	std::string offsetsPath = NP(root + FILE_DELIMITER + it->Attribute("offsetsPath"));

	auto p = std::make_shared<PackedImages>();
	p->init(binaryPath, offsetsPath);

	return p;
}

// release the textures from previous game map
void TextureManager::releaseLastGameMapTextures(){
	
	for (auto it : m_toDeleteTextures) {
		auto findIt = m_textureMaps.find(it);
		if (findIt != m_textureMaps.end()) {
			SDL_DestroyTexture(findIt->second);
			m_textureMaps.erase(findIt);
		}
	}
	m_toDeleteTextures.clear();
}

// Preload the textures that are needed in a game map, including the following stuff:
//     - the FJ_1, FJ_2, FJ_3 and FJ_4 textures, namely the building images
//     - NPCs
//     - Monsters
// This function first releases the textures for the last game map and than loaded the
// new ones for the current map.
//
// Arguments
// ---------
//     pEvent: a shared pointer to `MapCreatedEventData`
void TextureManager::onPreLoadTextures(IEventDataPtr pEvent){
	releaseLastGameMapTextures();

	auto p = std::static_pointer_cast<MapCreatedEventData>(pEvent);
	GameMapPtr pgm = p->getGameMap();

	std::vector<std::shared_ptr<PackedImages>> fjPackedImages = {
		std::make_shared<PackedImages>(),
		createPackedImages("FJ_1"),
		createPackedImages("FJ_2"),
		createPackedImages("FJ_3"),
		createPackedImages("FJ_4"),
	};
	for(int k = 0; k < pgm->getRows() * pgm->getCols(); k++){
		int i = k / pgm->getCols();
		int j = k % pgm->getCols();

		GameMapGrid &grid = (*pgm)(i, j);
		auto roleID = grid.roleID;
		auto tileID = grid.tileID;
		auto other = grid.other;
		auto floorID = grid.floorID;

		// load tile
		std::string tileTextureID = "FJ_" + std::to_string(tileID);
		if(tileID != -1 && m_textureMaps.find(tileTextureID) == m_textureMaps.end()){
			auto texture = (*fjPackedImages[tileID/10000])[tileID%10000];
			m_toDeleteTextures.push_back(tileTextureID);
			m_textureMaps[tileTextureID] = texture;
		}

		
		if(roleID != ""){
			if(roleID[0] == 'N'){
				// load NPC
				std::string npcID = "NPC_" + roleID.substr(roleID.size() - 3, 3);
				if(m_textureMaps.find(npcID + "_0") == m_textureMaps.end())
					loadPackedImages(npcID);
			}

			if(roleID[0] == 'M'){
				// load Monster
				std::string monName = roleID.substr(1, roleID.size() - 1);
				auto &info = g_alita->getMonsterDB()[monName];
				std::string monID = std::to_string(info.Pic);
				std::string monAnimationID = "MON_" + std::string(3 - monID.size(), '0') + monID;

				if(m_textureMaps.find(monAnimationID + "_0") == m_textureMaps.end())
					loadPackedImages(monAnimationID);
			}
		}
	}
}

// Arguments
// ---------
//     tid: say tid = "FLOOR", then each tile will be stored with the key "FLOOR_0", 
//         "FLOOR_1", etc.
//     toDelete: when set to false, do not add textures to the `toDelete` list, so
//         the textures will stay around. Set it to false when loading global
//         textures that are not specific to specific game maps, like floor
//         textures.
void TextureManager::loadPackedImages(std::string tid, bool toDelete){
	auto pi = createPackedImages(tid);
	for (int idx = 0; idx < pi->getNbImages(); idx++) {
		std::string individualTid = tid + "_" + std::to_string(idx);
		
		m_textureMaps[individualTid] = (*pi)[idx];
		if(toDelete)
			m_toDeleteTextures.push_back(individualTid);
	}
}