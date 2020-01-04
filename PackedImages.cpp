#include "PackedImages.h"
#include "Game.h"
#include <fstream>
#include <set>

PackedImages::~PackedImages(){
	if(m_pBuffer)
		delete[] m_pBuffer;

	m_binaryFs.close();
}

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

    int t;
    char c;
    while(fs>>t>>c){
        m_offsets.push_back(t);
    }
    fs.close();

	int maxBufferSize = getMaxBufferSize(m_offsets);
	m_pBuffer = GCC_NEW char[maxBufferSize];
	   
	std::vector<std::pair<int, SDL_Texture*>> textures;
	m_binaryFs.open(binary_path, std::fstream::binary | std::fstream::in);
	if (!m_binaryFs) {
		printf("Failed to open `%s`!\n", binary_path.c_str());
		return false;
	}

    return true;
}


SDL_Texture *PackedImages::operator[](int i){
	int nbBytes = m_offsets[i + 1] - m_offsets[i];
	m_binaryFs.seekg(m_offsets[i]);
	m_binaryFs.read(m_pBuffer, nbBytes);
	SDL_RWops *ops = SDL_RWFromMem(m_pBuffer, nbBytes);

	SDL_Surface *pSurface = IMG_Load_RW(ops, 1);
	if (m_colorKey)
		SDL_SetColorKey(pSurface, SDL_TRUE, SDL_MapRGB(pSurface->format, 0x00, 0x00, 0x00));
	SDL_Texture *pTexture = SDL_CreateTextureFromSurface(g_alita->getRenderer(), pSurface);
	SDL_FreeSurface(pSurface);

	return pTexture;
}