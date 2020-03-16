#include "camera.h"

Camera::Camera(Window* window, Input* input, Time* time, bool isGameCamera)
{
    speed = 3.0f;
    mouseSpeed = 1.2f;

    cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    cameraFront = glm::vec3(-0.54f, -0.39f, -0.74f);
    if(!isGameCamera)
        cameraPosition = glm::vec3(14.0f, 8.7f, 13.3f);
    else
        cameraPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    target = glm::vec3(0.0f, 0.0f, 0.0f);

    direction = glm::vec3(-0.49f, -0.39f, -0.77f);
    glm::normalize(direction);

    right = glm::vec3(0.75f, 0.0f, -0.6f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);

    yaw = -126.0f;
    pitch = -23.0f;

    view = glm::lookAt(cameraPosition, cameraPosition + direction, up);

    this->window = window;
    this->input = input;
    this->time = time;
    projection = glm::perspective(glm::radians(60.0f), (float)window->GetWidth()/(float)window->GetHeight(), 0.001f, 1000.0f);

    isGamed = isGameCamera;
}

Camera::~Camera()
{

}

void Camera::Move(glm::vec3 dir)
{
    cameraPosition+=dir;
    view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
}

void Camera::SetPosition(glm::vec3 position)
{
    cameraPosition = position;
}

glm::vec3& Camera::GetPosition()
{
    return cameraPosition;
}

// Индекс нужен для идентификации камеры. Это полезно, например, при скриптинге.
void Camera::SetIndex(int index)
{
    this->index = index;
}

int Camera::GetIndex()
{
    return index;
}

void Camera::Front(float m_speed)
{
    cameraPosition += cameraFront * m_speed * (float)time->GetDeltaTime();
}

void Camera::Back(float m_speed)
{
    cameraPosition -= cameraFront * m_speed * (float)time->GetDeltaTime();
}

void Camera::Right(float m_speed)
{
    cameraPosition += glm::normalize(glm::cross(cameraFront, up)) * m_speed * (float)time->GetDeltaTime();
}

void Camera::Left(float m_speed)
{
    cameraPosition -= glm::normalize(glm::cross(cameraFront, up)) * m_speed * (float)time->GetDeltaTime();
}

void Camera::Yaw(float offset)
{
    yaw += offset * (float)time->GetDeltaTime();
    if(!isGamed)
    {
        if(yaw > 360)
            yaw -= 360;
        if(yaw < -360)
            yaw += 360;
    }
    glm::vec3 front;
    front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    front.y = sin(glm::radians(pitch));
    front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    cameraFront = glm::normalize(front);
}

void Camera::Pitch(float offset)
{
    pitch += offset * (float)time->GetDeltaTime();

    if(!isGamed)
    {
        if(pitch > 89.0f)
            pitch = 89.0f;
        if(pitch < -89.0f)
            pitch = -89.0f;
    }
    glm::vec3 front;
    front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    front.y = sin(glm::radians(pitch));
    front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    cameraFront = glm::normalize(front);
}

void Camera::InputMove()
{
    if(!isGamed)
    {
        if(input->GetMouseButtonDown(2))
        {
            float deltaX = input->GetDeltaX();
            float deltaY = input->GetDeltaY();

            double mouseX, mouseY;

            input->GetCursorPos(&mouseX, &mouseY);
            if(mouseX > window->GetWidth()-10){
                deltaX = 5;
                input->SetCursorPos(15, mouseY);
            }
            else if(mouseX < 10){
                deltaX = 5;
                input->SetCursorPos(window->GetWidth()-15, mouseY);
            }

            if(mouseY > window->GetHeight()-10){
                input->SetCursorPos(mouseX, 15);
                deltaY = 5;
            }
            else if(mouseY < 10){
                input->SetCursorPos(mouseX, window->GetHeight()-15);
                deltaY = 5;
            }

            Yaw(deltaX * mouseSpeed);
            Pitch(-deltaY * 2.0f * mouseSpeed);
        }
    }

    //direction = glm::normalize(cameraPosition - target);

    direction.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    direction.y = sin(glm::radians(pitch));
    direction.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), direction));
    up = glm::cross(direction, right);

    if(!isGamed)
    {
        if(input->GetKeyDown(GLFW_KEY_LEFT_SHIFT))
        {
            speed = 9.0f;
        }
        else
            speed = 3.0f;
        if(input->GetKeyDown(GLFW_KEY_W))
        {
            Front(speed);
        }

        if(input->GetKeyDown(GLFW_KEY_S))
        {
            Back(speed);
        }

        if(input->GetKeyDown(GLFW_KEY_A))
        {
            Left(speed);
        }

        if(input->GetKeyDown(GLFW_KEY_D))
        {
            Right(speed);
        }

        if(input->GetKeyDown(GLFW_KEY_R))
        {
            cameraPosition += glm::vec3(0.0f, 1.0f, 0.0f) * speed * (float)time->GetDeltaTime();
        }

        if(input->GetKeyDown(GLFW_KEY_F))
        {
            cameraPosition += glm::vec3(0.0f, -1.0f, 0.0f) * speed * (float)time->GetDeltaTime();
        }
    }

    view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, up);
}

glm::mat4 Camera::GetView() const
{
    return view;
}

glm::mat4 Camera::GetProjection() const
{
    return projection;
}
