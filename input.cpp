#include "input.h"

Input::Input()
{

}

Input::~Input()
{

}

void Input::SetIO()
{
    this->io = &ImGui::GetIO();
}

Input::Input(Window* m_window)
{
    this->m_window = m_window;
}

float Input::GetDeltaX()
{
    return io->MouseDelta.x;
}

float Input::GetDeltaY()
{
    return io->MouseDelta.y;
}

bool Input::GetKeyDown(int key)
{
    return glfwGetKey(m_window->GetWindow(), key) == GLFW_PRESS;
}

bool Input::GetKeyUp(int key)
{
    return glfwGetKey(m_window->GetWindow(), key) == GLFW_RELEASE;
}

bool Input::GetMouseButtonDown(int key)
{
    return glfwGetMouseButton(m_window->GetWindow(), key) == GLFW_PRESS;
}

void Input::GetCursorPos(double* xpos, double* ypos)
{
    glfwGetCursorPos(m_window->GetWindow(), xpos, ypos);
}

void Input::SetCursorPos(double xpos, double ypos)
{
    glfwSetCursorPos(m_window->GetWindow(), xpos, ypos);
}

void Input::HideCursor()
{
    glfwSetInputMode(m_window->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

void Input::ShowCursor()
{
    glfwSetInputMode(m_window->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}
