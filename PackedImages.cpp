#include "PackedImages.h"
#include "Game.h"
#include <fstream>

static int getMaxBufferSize(const std::vector<int> &offsets){
    int size = -1, t = -1;
    for(int i = 0; i < offsets.size() - 1; i++){
        t = offsets[i+1] - offsets[i];
        if(size < t)
            size = t;
    }
    return size;
}

bool PackedImages::init(std::string binary_path, std::string offsets_path){
    std::fstream fs;
    fs.open(offsets_path, std::fstream::in);
    if(!fs){
        printf("Failed to open `%s`!\n", offsets_path.c_str());
        return false;
    }

    std::vector<int> offsets;
    int t;
    char c;
    while(fs>>t>>c){
        offsets.push_back(t);
    }
    fs.close();

    fs.open(binary_path, std::fstream::binary | std::fstream::in);
    if(!fs){
        printf("Failed to open `%s`!\n", binary_path.c_str());
        return false;
    }

    int maxBufferSize = getMaxBufferSize(offsets);
    char *pBuffer = new char[maxBufferSize];
    int successCount = 0;
    for(int i = 0; i < offsets.size() - 1; i++){
        int nbBytes = offsets[i+1] - offsets[i];
        fs.read(pBuffer, nbBytes);
        SDL_RWops *ops = SDL_RWFromMem(pBuffer, nbBytes);

        SDL_Surface *pSurface = IMG_Load_RW(ops, 1);
        m_sizes.emplace_back(pSurface->w, pSurface->h);
        if(m_colorKey)
            SDL_SetColorKey(pSurface, SDL_TRUE, SDL_MapRGB(pSurface->format, 0x00, 0x00, 0x00));
        SDL_Texture *pTexture = SDL_CreateTextureFromSurface(g_alita->getRenderer(), pSurface);
        if(pTexture){
            successCount++;
        }
        SDL_FreeSurface(pSurface);
        m_textures.push_back(pTexture);
    }
    fs.close();

    printf("%d packed images loaded and %d successes!\n", offsets.size() - 1, successCount);

    delete[] pBuffer;
    return true;
}


SDL_Texture *PackedImages::operator[](int i){
    if(i < 0 || i >= m_textures.size()){
        printf("Indices %d is out of range [0, %d)!\n", i, m_textures.size());
        return nullptr;
    }

    return m_textures[i];
}


void PackedImages::destroy(){
    for(auto it: m_textures){
        SDL_DestroyTexture(it);
    }

    m_textures.clear();
    m_sizes.clear();
}