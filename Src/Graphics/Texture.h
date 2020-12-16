#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>


class Texture
{
private:
    unsigned int ID;

    int width;
    int height;
public:


    Texture(const std::string& filepath);

    ~Texture();

    unsigned int GetID() const;

    int GetWidth() const;

    int GetHeight() const;
};


#endif