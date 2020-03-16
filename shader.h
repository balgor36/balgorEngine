#ifndef Shader_H
#define Shader_H

#include "window.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cassert>
#include <map>
#include <fstream>

class Shader
{
public:
    Shader();
    ~Shader();

    void RegisterID(std::string m_name);
    void LoadFile(const char* filename, std::string& str);
    GLuint LoadShader(std::string& source, GLuint  mode);
    void InitShader(const char* vname, const char* fname, bool gameShader);
    void Clean();

    GLuint GetID(std::string m_name);
    GLuint GetProgramID();
private:
    GLuint vs, fs, program;

    std::vector<std::pair<GLuint, std::string>> m_ids;
};

#endif
