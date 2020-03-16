#ifndef MESH_MANAGER_H
#define MESH_MANAGER_H

#include <glm/gtx/string_cast.hpp>
#include "mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Mesh;

class MeshManager
{
public:
    MeshManager();
    ~MeshManager();

    bool ImportMesh(std::string path);
    void RecursiveProcess(aiNode* node, const aiScene* scene, std::string path);
    void ProcessMesh(aiMesh* mesh, const aiScene* scene, std::string path, std::string name);

    std::vector<Mesh*>& GetMeshes();
    Mesh* GetMeshByIndex(int index);

    void AddIndex(Mesh* mesh);
private:
    std::vector<Mesh*> meshes;
    Mesh* lastMesh;

    Mesh* GetMeshRecursive(int index, Mesh* mesh);
    bool first;
    int index = 0;
    int numChilds = 0;
};

#endif
