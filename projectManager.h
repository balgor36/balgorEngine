#ifndef PROJECT_MANAGER_H
#define PROJECT_MANAGER_H

#include "meshManager.h"
#include "imageLoader.h"

#include <boost/filesystem.hpp>

#include <fstream>

class MeshManager;
class ImageLoader;

class ProjectManager
{
public:
    ProjectManager();
    ~ProjectManager();

    bool Refresh(MeshManager* meshManager);
    bool Refresh(ImageLoader* imageLoader);
};

#endif
