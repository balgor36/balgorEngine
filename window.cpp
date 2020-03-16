#include "window.h"

Window::Window()
{

}

Window::Window(int width, int height, const char* title)
{
    this->m_width = width;
    this->m_height = height;
    this->title = title;
    if(!Init())
    {
        m_result = false;
    }
}

Window::~Window()
{
}

bool Window::Init()
{
    if(!glfwInit())
    {
        std::cout << "Window\tfailed to init GLFW." << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(m_width, m_height, title, NULL, NULL);
    if(!m_window)
    {
        std::cout << "Window\t\tfailed to create window." << std::endl;
        glfwTerminate();
        return false;
    }

    glfwSetWindowUserPointer(m_window, this);
    glfwSetWindowSizeCallback(m_window, WindowResizeCallback);

    glfwMakeContextCurrent(m_window);

    glewExperimental = true;
    if(glewInit() != GLEW_OK)
    {
        std::cout << "Window\t\tfailed to init GLEW." << std::endl;
        glfwTerminate();
        return false;
    }

    glfwSwapInterval(1); // vsync

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    return true;
}

bool Window::GetResult() const
{
    return m_result;
}

void Window::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.250f, 0.250f, 0.250f, 1.0f);
    //glClearColor(0.2784f, 0.2784f, 0.2784f, 1.0f);
    //glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
}

void Window::Update()
{
    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

int Window::isClosed() const
{
    return glfwWindowShouldClose(m_window);
}

void Window::Release()
{
    glfwTerminate();
}

void Window::Close()
{
    glfwWindowShouldClose(m_window);
}

GLFWwindow* Window::GetWindow() const
{
    return m_window;
}

int Window::GetWidth() const
{
    return m_width;
}

int Window::GetHeight() const
{
    return m_height;
}

void Window::WindowResizeCallback(GLFWwindow* win, int width, int height)
{
    Window* window = static_cast<Window*>(glfwGetWindowUserPointer(win));

    window->m_width = width;
    window->m_height = height;

    //window->GetRenderTexture()->CreateFBO(width, height);
    //window->GetRenderGameTexture()->CreateFBO(width, height);
};
