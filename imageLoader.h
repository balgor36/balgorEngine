#ifndef IMAGE_LOADER_H
#define IMAGE_LOADER_H

#include "window.h"
#include <vector>
#include <string>
#include <SOIL.h>

struct Texture
{
    GLuint texture;
    const char* path;
};

enum type {DIFFUSE, NORMAL};

class ImageLoader
{
public:
    ImageLoader();
    ~ImageLoader();

    GLuint CreateTexture(int width, int height, bool isDepth, unsigned char* data, bool mipmap);
    void LoadImage(const char* path, GLint type);
    GLuint LoadCubemap(std::vector<std::string> faces);
    std::vector<Texture>& GetDiffuseTextures();
    std::vector<Texture>& GetNormalMaps();
private:
    std::vector<Texture> m_textures;
    std::vector<Texture> m_normalMaps;

    bool anisotropic;
};

#endif
