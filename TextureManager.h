#ifndef ALITA_TEXTURE_MANAGER_H
#define ALITA_TEXTURE_MANAGER_H

#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"
#include <map>
#include "AlitaStd.h"
#include "PackedImages.h"
#include "EventData.h"

class TextureManager{
public:
    ~TextureManager(){destroy();}
    bool init(XMLElement *doc);

    void draw(const TextureID &tid, int x, int y, int w, int h, SDL_Renderer *pRenderer);
    void drawFrame(const TextureID &tid, int x, int y, int w, int h, int currentRow, int currentFrame,
                   SDL_Renderer *pRenderer, int angle=0, SDL_RendererFlip flip=SDL_FLIP_NONE);

    void drawText(const std::string &text, int x, int y, int w, int h, SDL_Color textColor,
        SDL_Renderer *pRenderer, TTF_Font *pFont);

    void destroy();

    SDL_Texture *getTexture(const TextureID &tid);

private:
    std::map<TextureID, SDL_Texture*> m_textureMaps;
    std::vector<TextureID> m_toDeleteTextures;
    std::map<TextureID, XMLElement*> m_packedImagesInfos;

    XMLElement *m_pTextureInfos = nullptr;

    bool loadGlobalTextures();
    void loadPackedImages(std::string tid, bool toDelete=true);
    void releaseLastGameMapTextures();
    std::shared_ptr<PackedImages> createPackedImages(std::string id);

    void onPreLoadTextures(IEventDataPtr pEvent);
};

#endif