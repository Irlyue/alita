#ifndef ALITA_PACKED_IMAGES_H
#define ALITA_PACKED_IMAGES_H

#include <string>
#include <vector>
#include <set>
#include "SDL.h"
#include "Vector2D.h"
#include <fstream>

class PackedImages{
public:
    ~PackedImages();
    bool init(std::string binary_path, std::string offsets_path);

    SDL_Texture *operator[](int i);

    int getNbImages() const {return m_offsets.size() - 1;}

private:
    std::fstream m_binaryFs;
    std::vector<int> m_offsets;
    bool m_colorKey = true;
    char *m_pBuffer = nullptr;
};

#endif