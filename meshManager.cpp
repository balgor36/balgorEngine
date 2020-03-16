#include "meshManager.h"

MeshManager::MeshManager()
{
    index = 0;
    numChilds = 0;
}

MeshManager::~MeshManager()
{
    for(GLuint i = 0; i < meshes.size(); i++)
        delete meshes[i];
}

bool MeshManager::ImportMesh(std::string path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_GenSmoothNormals | aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_FlipUVs | aiProcessPreset_TargetRealtime_Quality);

    if(scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode || !scene)
    {
        std::cout << importer.GetErrorString() << std::endl;
        return false;
    }

    first = true;
    RecursiveProcess(scene->mRootNode, scene, path);
    AddIndex(meshes[meshes.size()-1]);
    return true;
}

void MeshManager::RecursiveProcess(aiNode* node, const aiScene* scene, std::string path)
{
    // process
    for(GLuint i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        numChilds = node->mNumChildren;
        ProcessMesh(mesh, scene, path, node->mName.C_Str());
        first = false;
    }

    for(GLuint i = 0; i < node->mNumChildren; i++)
    {
        RecursiveProcess(node->mChildren[i], scene, path);
    }
}

void MeshManager::ProcessMesh(aiMesh* mesh, const aiScene* scene, std::string path, std::string name)
{
    std::vector<glm::vec3> vertexData;
    std::vector<glm::vec3> normalData;
    std::vector<glm::vec2> uvData;

    std::vector<glm::vec3> tangentData;
    std::vector<glm::vec3> bitangentData;
    std::vector<glm::vec3> colorData;
    std::vector<GLuint> indicesData;

    for(GLuint i = 0; i < mesh->mNumVertices; i++)
    {
        glm::vec3 tempVec;

        tempVec.x = mesh->mVertices[i].x;
        tempVec.y = mesh->mVertices[i].y;
        tempVec.z = mesh->mVertices[i].z;

        vertexData.push_back(tempVec);

        tempVec.x = mesh->mNormals[i].x;
        tempVec.y = mesh->mNormals[i].y;
        tempVec.z = mesh->mNormals[i].z;
        normalData.push_back(tempVec);

        glm::vec2 tempUV;
        if(mesh->mTextureCoords[0]){
            tempUV.x = mesh->mTextureCoords[0][i].x;
            tempUV.y = mesh->mTextureCoords[0][i].y;
        }
        else{
            tempUV.x = 0;
            tempUV.y = 0;
        }
        uvData.push_back(tempUV);

        if(mesh->mTangents){
            tempVec.x = mesh->mTangents[i].x;
            tempVec.y = mesh->mTangents[i].y;
            tempVec.z = mesh->mTangents[i].z;
        }
        else{
            tempVec.x = 0;
            tempVec.y = 0;
            tempVec.z = 0;
        }
        tangentData.push_back(tempVec);

        if(mesh->mBitangents){
            tempVec.x = mesh->mBitangents[i].x;
            tempVec.y = mesh->mBitangents[i].y;
            tempVec.z = mesh->mBitangents[i].z;
        }
        else{
            tempVec.x = 0;
            tempVec.y = 0;
            tempVec.z = 0;
        }

        bitangentData.push_back(tempVec);

        if(mesh->mColors[0]){
            tempVec.x = mesh->mColors[0][i].r;
            tempVec.y = mesh->mColors[0][i].g;
            tempVec.z = mesh->mColors[0][i].b;
        }
        else{
            tempVec.x = 0.3f;
            tempVec.y = 0.3f;
            tempVec.z = 0.3f;
        }
        colorData.push_back(tempVec);
    }

    for(GLuint i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        indicesData.push_back(face.mIndices[0]);
        indicesData.push_back(face.mIndices[1]);
        indicesData.push_back(face.mIndices[2]);
    }

    // Загрузка материалов
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    aiColor4D ambientColor;
    aiColor4D diffuseColor;
    aiColor4D specularColor;
    //float shininess;

    //aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &ambientColor);
    aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuseColor);
    //aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &specularColor);
    //aiGetMaterialFloat(material, AI_MATKEY_SHININESS, &shininess);

    PBRMaterial m_material;
    /*m_material.ambient.x = ambientColor.r;
    m_material.ambient.y = ambientColor.g;
    m_material.ambient.z = ambientColor.b;

    m_material.diffuse.x = diffuseColor.r;
    m_material.diffuse.y = diffuseColor.g;
    m_material.diffuse.z = diffuseColor.b;

    m_material.specular.x = specularColor.r;
    m_material.specular.y = specularColor.g;
    m_material.specular.z = specularColor.b;

    m_material.shininess = shininess;*/
    m_material.albedo.r = diffuseColor.r;
    m_material.albedo.g = diffuseColor.g;
    m_material.albedo.b = diffuseColor.b;

    m_material.metallic = 0.0f;
    m_material.roughness = 0.0f;
    m_material.occlusion = 0.5f;

    if(first){
        meshes.push_back(new Mesh(&vertexData, &normalData, &uvData, &tangentData, &bitangentData, &colorData, &indicesData, &path, &name, &m_material));
        size_t meshSize = meshes.size();
        lastMesh = meshes[meshSize-1];
    }
    else{
        std::vector<Mesh*>& childMeshes = lastMesh->GetChildMeshes();

        childMeshes.push_back(new Mesh(&vertexData, &normalData, &uvData, &tangentData, &bitangentData, &colorData, &indicesData, &path, &name, &m_material));
        if(numChilds > 0){
            lastMesh = childMeshes[lastMesh->GetChildMeshes().size() - 1];
        }
    }

    /*if(isChild && !first)
    {
        if(lastMesh == NULL)
        {
            meshes[meshes.size() - 1]->GetChildMeshes().push_back(new Mesh(&vertexData, &normalData, &uvData, &tangentData, &bitangentData, &indicesData, &path, &name));
            lastMesh = meshes[meshes.size() - 1]->GetChildMeshes()[meshes[meshes.size() - 1]->GetChildMeshes().size() - 1];
        }
        else
        {
            lastMesh->GetChildMeshes().push_back(new Mesh(&vertexData, &normalData, &uvData, &tangentData, &bitangentData, &indicesData, &path, &name));
            lastMesh = lastMesh->GetChildMeshes()[lastMesh->GetChildMeshes().size() - 1];
        }
    }

    else if(!isChild || first)
    {
        meshes.push_back(new Mesh(&vertexData, &normalData, &uvData, &tangentData, &bitangentData, &indicesData, &path, &name));
        //lastMesh = meshes[meshes.size() - 1];
        lastMesh = NULL;
    }*/
}

void MeshManager::AddIndex(Mesh* mesh)
{
    mesh->SetIndex(index);
    index++;
    for(int i = 0; i < mesh->GetChildMeshes().size(); i++)
    {
        AddIndex(mesh->GetChildMeshes()[i]);
    }
}

std::vector<Mesh*>& MeshManager::GetMeshes()
{
    return meshes;
}

Mesh* MeshManager::GetMeshRecursive(int index, Mesh* mesh)
{
    for(int i = 0; i < mesh->GetChildMeshes().size(); i++)
    {
        if(mesh->GetChildMeshes()[i]->GetIndex() == index)
        {
            return mesh->GetChildMeshes()[i];
        }

    }
    for(int i = 0; i < mesh->GetChildMeshes().size(); i++)
    {
        return GetMeshRecursive(index, mesh->GetChildMeshes()[i]);
    }
    return NULL;
}

Mesh* MeshManager::GetMeshByIndex(int index)
{
    Mesh* tmpMesh;
    for(int i = 0; i < meshes.size(); i++)
    {
        if(meshes[i]->GetIndex() == index)
        {
            return meshes[i];
        }
    }
    for(int i = 0; i < meshes.size(); i++)
    {
        tmpMesh = GetMeshRecursive(index, meshes[i]);
        if(tmpMesh != NULL)
            return tmpMesh;
        else continue;
    }
    return tmpMesh;
}
