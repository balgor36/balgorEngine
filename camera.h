#ifndef CAMERA_H
#define CAMERA_H

#include <glm/gtx/string_cast.hpp>
#include "input.h"
#include "meshData.h"
#include "time_bg.h"

class Window;
class Input;

class Camera
{
public:
    Camera(Window* window, Input* input, Time* time, bool isGameCamera);
    ~Camera();

    void Move(glm::vec3 move);
    void InputMove();
    void SetPosition(glm::vec3 position);

    glm::vec3& GetPosition();

    void SetIndex(int index);
    int GetIndex();

    glm::mat4 GetView() const;
    glm::mat4 GetProjection() const;

    void Front(float m_speed);
    void Back(float m_speed);
    void Right(float m_speed);
    void Left(float m_speed);

    void Yaw(float offset);
    void Pitch(float offset);
private:
    glm::mat4 projection;
    glm::mat4 view;

    glm::vec3 direction;
    glm::vec3 right;
    glm::vec3 up;

    glm::vec3 target;

    glm::vec3 cameraPosition;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;

    float speed;
    float mouseSpeed;

    float yaw;
    float pitch;

    bool isGamed;

    Window* window;
    Input* input;
    Time* time;

    int index;
};

#endif
