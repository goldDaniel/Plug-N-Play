#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <memory>
#include <unordered_map>

class Texture
{
private:
    static std::unordered_map<std::string, std::shared_ptr<Texture>> loaded_textures;

    const unsigned int ID;

    const int width;
    const int height;

public:
    static std::shared_ptr<Texture> CreateTexture(const std::string& filepath);

    Texture(unsigned int ID, int w, int h);

    unsigned int GetID() const;
    int GetWidth() const;
    int GetHeight() const;
};


#endif