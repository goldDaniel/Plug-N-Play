#include "Texture.h"

#include <Core.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

std::unordered_map<std::string, std::shared_ptr<Texture>> Texture::loaded_textures;

std::shared_ptr<Texture> Texture::CreateTexture(const std::string& filepath)
{
    if(loaded_textures.find(filepath) != loaded_textures.end())
    {
        return loaded_textures[filepath];
    }

    unsigned int ID;
    int width;
    int height;
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    int nrChannels;

    void* data = stbi_load(filepath.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        assert((nrChannels == 3 || nrChannels == 4) && "Texture channel count is invalid");

        if(nrChannels == 3)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        if(nrChannels == 4)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }   

        loaded_textures[filepath] = std::make_shared<Texture>(ID, width, height);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    return loaded_textures[filepath];
}

Texture::Texture(unsigned int ID, int w, int h) : ID(ID), width(w), height(h)
{}

Texture::~Texture()
{
    glDeleteTextures(1, &ID);
}

unsigned int Texture::GetID() const
{
    return ID;
}

int Texture::GetWidth() const
{
    return width;
}

int Texture::GetHeight() const
{
    return height;
}