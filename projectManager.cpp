#include "projectManager.h"

ProjectManager::ProjectManager()
{
    /*mkdir("/project");
    mkdir("/project/meshes");
    mkdir("/project/images");*/

    boost::filesystem::create_directory("project");
    boost::filesystem::create_directory("project/meshes");
    boost::filesystem::create_directory("project/images");
}

ProjectManager::~ProjectManager()
{
}

bool ProjectManager::Refresh(ImageLoader* imageLoader)
{

    for(GLuint i = 0; i < imageLoader->GetDiffuseTextures().size(); i++)
    {
        std::ifstream in;
        std::ofstream out;

        std::ifstream testFile;

        std::string fullPath, outPath;

        fullPath = imageLoader->GetDiffuseTextures()[i].path;
        boost::filesystem::path p(fullPath);

        outPath = "project/images/" + p.leaf().string();

        testFile.open(outPath);
        if(testFile.is_open()) // Если мы обнаружали ранее созданый файл - пропускаем его обновление.
            continue;

        in.open(fullPath, std::ios::binary);
        out.open(outPath, std::ios::binary);

        if(!in.is_open())
        {
            std::cout << "Failed to open the image: " << fullPath << std::endl;
            return false;
        }

        if(!out.is_open())
        {
            std::cout << "Failed to write the image: " << outPath << std::endl;
            return false;
        }

        std::copy(std::istreambuf_iterator <char> (in), std::istreambuf_iterator <char> (), std::ostreambuf_iterator <char> (out));

        in.close();
        out.close();

        testFile.close();
    }

    return true;
}

bool ProjectManager::Refresh(MeshManager* meshManager)
{
    /*for(GLuint i = 0; i < meshManager->GetMeshes().size(); i++)
    {
        std::ifstream in;
        std::ofstream out;

        std::ifstream testFile;

        std::string fullPath, outPath;
        fullPath = meshManager->GetMeshes()[i]->GetPath();
        boost::filesystem::path p(fullPath);

        outPath = "project/meshes/" + p.leaf().string();

        testFile.open(outPath);
        if(!testFile.fail()) // Если мы обнаружали ранее созданый файл - пропускаем его обновление.
            continue;

        in.open(fullPath);
        out.open(outPath);

        if(!in.is_open())
        {
            std::cout << "Failed to open the mesh: " << fullPath << std::endl;
            return false;
        }

        if(!out.is_open())
        {
            std::cout << "Failed to write the mesh: " << outPath << std::endl;
            return false;
        }


        std::copy(std::istreambuf_iterator <char> (in), std::istreambuf_iterator <char> (), std::ostreambuf_iterator <char> (out));
        in.close();
        out.close();

        testFile.close();
    }
*/
    return true;
}
