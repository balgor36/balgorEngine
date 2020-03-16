#ifndef CAMERA_MANAGER_H
#define CAMERA_MANAGER_H

#include <vector>
#include "camera.h"

class Camera;

class CameraManager
{
public:
    CameraManager();
    ~CameraManager();

    void AddCamera(Camera* camera);
    void SetActiveGameCamera(int index);

    int GetActiveGameCamera();
    int& GetIndex();
    std::vector<Camera*>& GetCameras();
private:
    std::vector<Camera*> cameras;

    int m_index;
    int m_activeIndex;
};

#endif
