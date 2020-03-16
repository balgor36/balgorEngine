#ifndef MOUSE_PICKER_H
#define MOUSE_PICKER_H

#include "camera.h"

class MousePicker
{
public:
    MousePicker();
    ~MousePicker();

    glm::vec3 GetCurrentRay();
    void Update(Input* input, Window* window, Camera* camera);
private:
    glm::vec3 currentRay;

    glm::vec3 CalculateMouseRay(Input* input, Window* window, Camera* camera);
    glm::vec2 GetNormalizedDeviceCoords(float mouseX, float mouseY, Window* window);
    glm::vec4 ToEyeCoords(glm::vec4 clipCoords, Camera* camera);
    glm::vec3 ToWorldCoords(glm::vec4 eyeCoords, Camera* camera);
};
#endif
