#ifndef ALITA_PACKED_IMAGES_H
#define ALITA_PACKED_IMAGES_H

#include <string>
#include <vector>
#include "SDL.h"
#include "Vector2D.h"

class PackedImages{
public:
    ~PackedImages() {destroy();};
    bool init(std::string binary_path, std::string offsets_path);
    void destroy();

    SDL_Texture *operator[](int i);

private:
    bool m_colorKey = true;
    std::vector<SDL_Texture*> m_textures;
    std::vector<Vector2D> m_sizes;
};

#endif