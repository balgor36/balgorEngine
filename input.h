#ifndef INPUT_H
#define INPUT_H

#include "window.h"
#include <imgui/imgui.h>

class Window;

class Input
{
public:
    Input();
    Input(Window* m_window);
    ~Input();

    bool GetKeyDown(int key);
    bool GetKeyUp(int key);
    bool GetMouseButtonDown(int key);
    void GetCursorPos(double* xpos, double* ypos);
    void SetCursorPos(double xpos, double ypos);

    void HideCursor();
    void ShowCursor();

    float GetDeltaX();
    float GetDeltaY();

    void SetIO();
private:
    Window* m_window;
    ImGuiIO* io;
};

#endif
