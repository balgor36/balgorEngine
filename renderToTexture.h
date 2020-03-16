#ifndef RENDER_TO_TEXTURE
#define RENDER_TO_TEXTURE

#include "window.h"
#include "imageLoader.h"
#include <cassert>

class ImageLoader;

class RenderToTexture
{
public:
    RenderToTexture(ImageLoader* imageLoader);
    ~RenderToTexture();

    void CreateDepthBuffer(int width, int height);
    void CreateFBO(int width, int height);
    void StartDepth();
    void StartRender();
    void StopRender();
    GLuint GetRenderedTexture();
    GLuint GetShadowMap();
private:
    GLuint FBO;
    GLuint renderedTexture;
    GLuint depthTexture;
    GLuint shadowMap;

    ImageLoader* imageLoader;
};

#endif
