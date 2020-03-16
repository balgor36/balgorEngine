#include "cameraManager.h"

CameraManager::CameraManager()
{
    m_index = -1;
}

CameraManager::~CameraManager()
{
    for(int i = 0; i < cameras.size(); i++)
        delete cameras[i];
}

void CameraManager::SetActiveGameCamera(int index)
{
    m_activeIndex = index;
}

void CameraManager::AddCamera(Camera* camera)
{
    camera->SetIndex(m_index);
    cameras.push_back(camera);
    m_index++;
}

int CameraManager::GetActiveGameCamera()
{
    return m_activeIndex;
}

int& CameraManager::GetIndex()
{
    return m_index;
}

std::vector<Camera*>& CameraManager::GetCameras()
{
    return cameras;
}
