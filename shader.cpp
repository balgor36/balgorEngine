#include "shader.h"

Shader::Shader()
{

}

Shader::~Shader()
{
    Clean();
}

void Shader::LoadFile(const char* filename, std::string& str)
{
    std::ifstream in(filename);
    if(!in.is_open())
    {
        std::cout << "The file " << filename << " cannot be opened" << std::endl;
        return;
    }
    char tmp[300];
    while(!in.eof())
    {
        in.getline(tmp, 300);
        str+=tmp;
        str+='\n';
    }
    in.close();
}

GLuint Shader::LoadShader(std::string& source, GLuint mode)
{
    GLuint id;
    id = glCreateShader(mode);

    const char* csource = source.c_str();

    glShaderSource(id, 1, &csource, NULL);
    glCompileShader(id);

    char error[1000];
    glGetShaderInfoLog(id, 1000, NULL, error);
    std::cout << "Compile status: " << std::endl << error << std::endl;
    //assert(error == "\0");
    return id;
}

void Shader::InitShader(const char* vname, const char* fname, bool gameShader=false)
{
    std::string source;
    LoadFile(vname, source);
    vs = LoadShader(source, GL_VERTEX_SHADER);
    source = "";

    LoadFile(fname, source);
    fs = LoadShader(source, GL_FRAGMENT_SHADER);

    program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glDeleteShader(vs);
    glDeleteShader(fs);

    glLinkProgram(program);
}

void Shader::RegisterID(std::string m_name){
    GLuint id = glGetUniformLocation(program, m_name.c_str());
    m_ids.push_back(std::make_pair(id, m_name));
}

GLuint Shader::GetID(std::string m_name){
    for(int i = 0; i < m_ids.size(); i++){
        if(m_ids[i].second == m_name){
            return m_ids[i].first;
        }
    }
    return 0;
}

GLuint Shader::GetProgramID(){
    return program;
}

void Shader::Clean()
{
    glDetachShader(program, vs);
    glDetachShader(program, fs);
    glDeleteProgram(program);
}
