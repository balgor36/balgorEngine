#include "mousePicker.h"

MousePicker::MousePicker()
{

}

MousePicker::~MousePicker()
{

}

glm::vec3 MousePicker::GetCurrentRay()
{
    return currentRay;
}

void MousePicker::Update(Input* input, Window* window, Camera* camera)
{
    currentRay = CalculateMouseRay(input, window, camera);
    std::cout << glm::to_string(currentRay) << std::endl;
}

glm::vec3 MousePicker::CalculateMouseRay(Input* input, Window* window, Camera* camera)
{
    double mouseX, mouseY;
    input->GetCursorPos(&mouseX, &mouseY);

    glm::vec2 normalizedCoords = GetNormalizedDeviceCoords(mouseX, mouseY, window);
    glm::vec4 clipCoords = glm::vec4(normalizedCoords.x, normalizedCoords.y, -1.0f, 1.0f);
    glm::vec4 eyeCoords = ToEyeCoords(clipCoords, camera);
    glm::vec3 worldCoords = ToWorldCoords(eyeCoords, camera);
    return worldCoords;
}

glm::vec2 MousePicker::GetNormalizedDeviceCoords(float mouseX, float mouseY, Window* window)
{
    float x = (2.0f * mouseX) / (float)window->GetWidth() - 1.0f;
    float y = (2.0f * mouseY) / (float)window->GetHeight() - 1.0f;
    return glm::vec2(x, y);
}

glm::vec4 MousePicker::ToEyeCoords(glm::vec4 clipCoords, Camera* camera)
{
    glm::mat4 invertedProjection = glm::inverse(camera->GetProjection());
    glm::vec4 eyeCoords = invertedProjection * clipCoords;
    return glm::vec4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);
}

glm::vec3 MousePicker::ToWorldCoords(glm::vec4 eyeCoords, Camera* camera)
{
    glm::mat4 invertedView = glm::inverse(camera->GetView());
    glm::vec4 rayWorld = invertedView * eyeCoords;
    glm::vec3 mouseRay = glm::vec3(rayWorld.x, rayWorld.y, rayWorld.z);
    glm::normalize(mouseRay);
    return mouseRay;
}
