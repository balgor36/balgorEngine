#ifndef WINDOW_H
#define WINDOW_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "renderToTexture.h"

#include <iostream>

class RenderToTexture;

class Window
{
public:
    Window();
    Window(int width, int height, const char* title);
    ~Window();

    GLFWwindow* GetWindow() const;

    void Update();
    void Release();
    void Clear();
    void Close();

    bool GetResult() const;
    int isClosed() const;

    int GetWidth() const;
    int GetHeight() const;

    int m_width;
    int m_height;
private:
    bool Init();

    bool m_result = true;

    const char* title;
    GLFWwindow* m_window;

    static void WindowResizeCallback(GLFWwindow* win, int width, int height);
};

#endif
