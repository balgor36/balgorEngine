#include "imageLoader.h"

ImageLoader::ImageLoader()
{
    if(GLEW_EXT_texture_filter_anisotropic){
        anisotropic = true;
    }
    else{
        std::cout << "WARNING: anisotropic filtration is not support." << std::endl;
        anisotropic = false;
    }
}

ImageLoader::~ImageLoader()
{

}

GLuint ImageLoader::LoadCubemap(std::vector<std::string> faces){
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

    int width, height;
    unsigned char* data;
    for(GLuint i = 0; i < faces.size(); i++){
        try{
            data = SOIL_load_image(faces[i].c_str(), &width, &height, 0, SOIL_LOAD_RGB);
        }
        catch(const std::exception& ex){
            std::cout << ex.what() << std::endl;
            SOIL_free_image_data(data);
        }
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        SOIL_free_image_data(data);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return texture;
}

GLuint ImageLoader::CreateTexture(int width, int height, bool isDepth = false, unsigned char* data = 0, bool mipmap = true)
{
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, (!isDepth ? GL_RGB : GL_DEPTH_COMPONENT), width, height, 0, (!isDepth ? GL_RGB : GL_DEPTH_COMPONENT), GL_UNSIGNED_BYTE, data);
    if(anisotropic){
        GLfloat fLargest;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest);
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if(isDepth){
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
    }
    if(mipmap)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    return texture;
}

void ImageLoader::LoadImage(const char* path, GLint type)
{
    std::cout << type << std::endl;

    int width = 0;
    int height = 0;
    unsigned char* data;
    try{
        data  = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGB);
    }
    catch(const std::exception& ex){
        std::cout << ex.what() << std::endl;
    }

    GLuint texture = CreateTexture(width, height, false, data, true);

    SOIL_free_image_data(data);

    Texture tmpTex;
    tmpTex.texture = texture;
    tmpTex.path = path;

    switch(type)
    {
    case DIFFUSE:
        m_textures.push_back(tmpTex);
        break;
    case NORMAL:
        m_normalMaps.push_back(tmpTex);
        break;
    }
}

std::vector<Texture>& ImageLoader::GetDiffuseTextures()
{
    return m_textures;
}

std::vector<Texture>& ImageLoader::GetNormalMaps()
{
    return m_normalMaps;
}
