#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <memory>
#include <unordered_map>

class Texture
{
private:
    static std::unordered_map<std::string, std::shared_ptr<Texture>> loaded_textures;

public:

    const unsigned int ID;

    const int width;
    const int height;

    static Texture* CreateTexture(const std::string& filepath);

    Texture(unsigned int ID, int w, int h);
};


#endif