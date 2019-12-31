#include "TextureManager.h"
#include "Game.h"
#include "Utility.h"


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
    auto doc = m_pTextureInfos->FirstChildElement("Global");
    std::string root = doc->Attribute("folder");
    int successCount = 0, failCount = 0;
    bool success = true;

    for(auto it = doc->FirstChildElement(); it; it = it->NextSiblingElement()){
        std::string id = it->Attribute("id");
        std::string type = it->Value();
        if(type == "PackedImages"){
            PackedImages *p = new PackedImages;
            std::string binaryPath = NP(root + FILE_DELIMITER + it->Attribute("binaryPath"));
            std::string offsetsPath = NP(root + FILE_DELIMITER + it->Attribute("offsetsPath"));
            if(p->init(binaryPath,  offsetsPath)){
                successCount++;
            }else{
                failCount++;
                success = false;
            }
            m_packedImagesMaps[id] = p;
        }else if(type == "Texture"){
            std::string path = NP(root + FILE_DELIMITER + it->Attribute("path"));
            SDL_Texture *pTexture = loadTexture(path);

            if(pTexture){
                successCount++;
            }else{
                failCount++;
                success = false;
            }

            m_textureMaps[id] = pTexture;
            m_texturePathMaps[id] = path;
        }
    }
    printf("Load textures, %d successes and %d failures!\n", successCount, failCount);
    return success;
}

bool TextureManager::init(XMLElement *doc){
    m_pTextureInfos = doc;
    return loadGlobalTextures();
}

SDL_Texture *TextureManager::getTexture(const TextureID &tid){
    auto it = m_textureMaps.find(tid);
    if(it != m_textureMaps.cend()){
        return it->second;
    }
    return nullptr;
}

SDL_Texture *TextureManager::getTexture(const TextureID &tid, int offset){
    auto it = m_packedImagesMaps.find(tid);
    // if there is such PackedImages and it's not nullptr
    if(it != m_packedImagesMaps.cend() && it->second){
        return (*it->second)[offset];
    }
    return nullptr;
}

Vector2D TextureManager::getTextureSize(const TextureID &tid, int offset){
    SDL_Texture *p = getTexture(tid, offset);
    int w, h;
    SDL_QueryTexture(p, nullptr, nullptr, &w, &h);
    return {(float)w, (float)h};
}

std::string TextureManager::getTexturePath(const TextureID &tid){
    auto it = m_texturePathMaps.find(tid);
    if(it != m_texturePathMaps.cend()){
        return it->second;
    }
    return "";
}

void TextureManager::drawFrame(const TextureID &tid, int x, int y, int w, int h,
                               int currentRow, int currentFrame, SDL_Renderer *pRenderer,
                               int angle, SDL_RendererFlip flip){
    SDL_Rect src = {w * currentFrame, h * currentRow, w, h};
    SDL_Rect dst = {x, y, w, h};
    SDL_RenderCopyEx(pRenderer, getTexture(tid), &src, &dst, angle, 0, flip);
}

void TextureManager::drawTile(const TextureID &tid, int offset, int x, int y, int w, int h, SDL_Renderer *pRenderer){
    SDL_Texture *p = getTexture(tid, offset);
    if(w == -1 || h == -1){
        SDL_QueryTexture(p, nullptr, nullptr, &w, &h);
    }
    
    SDL_Rect src = {0, 0, w, h};
    SDL_Rect dst = {x, y, w, h};
    SDL_RenderCopy(pRenderer, p, &src, &dst);
}

void TextureManager::destroy(){
    for(auto &it: m_textureMaps){
        SDL_DestroyTexture(it.second);
    }
    m_textureMaps.clear();
    m_texturePathMaps.clear();

    for(auto it: m_packedImagesMaps){
        delete it.second;
    }
    m_packedImagesMaps.clear();

    printf("TextureManager destroyed!\n");
}