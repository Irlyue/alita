#ifndef ALITA_TEXTURE_MANAGER_H
#define ALITA_TEXTURE_MANAGER_H

#include "SDL.h"
#include "SDL_image.h"
#include <map>
#include "AlitaStd.h"
#include "PackedImages.h"

class TextureManager{
public:
    bool init(XMLElement *doc);

    void draw(const TextureID &tid, SDL_Rect &rect, SDL_Renderer *pRenderer, bool flip=false);
    void drawFrame(const TextureID &tid, int x, int y, int w, int h, int currentRow, int currentFrame,
                   SDL_Renderer *pRenderer, int angle=0, SDL_RendererFlip flip=SDL_FLIP_NONE);

    void drawTile(const TextureID &tid, int offset, int x, int y, int w, int h, SDL_Renderer *pRenderer);

    void destroy();

    SDL_Texture *getTexture(const TextureID &tid);
    SDL_Texture *getTexture(const TextureID &tid, int offset);
    std::string getTexturePath(const TextureID &tid);

    Vector2D getTextureSize(const TextureID &tid, int offset);
private:
    std::map<TextureID, SDL_Texture*> m_textureMaps;
    std::map<TextureID, std::string> m_texturePathMaps;
    std::map<TextureID, PackedImages*> m_packedImagesMaps;

    XMLElement *m_pTextureInfos = nullptr;

    bool loadGlobalTextures();
};

#endif