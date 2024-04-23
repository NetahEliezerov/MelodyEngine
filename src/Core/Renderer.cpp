#include "Renderer.h"
#include <vector>

void Renderer::GameLoop()
{
    
}

unsigned int Renderer::CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    GLint status;
    glGetShaderiv( id, GL_COMPILE_STATUS, &status );
    if ( status == GL_FALSE )
    {
        GLint maxLen;
        glGetShaderiv( id, GL_INFO_LOG_LENGTH, &maxLen );
        std::vector< char >log( maxLen );
        GLsizei len;
        glGetShaderInfoLog( id, maxLen, &len, log.data() );
        std::cout << "compile error:" << std::endl << log.data() << std::endl;
    }
    return id;
}

int Renderer::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) 
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER,vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER,fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDetachShader(program,vs);
    glDetachShader(program,fs);
    glDeleteShader(vs); 
    glDeleteShader(fs);

    return program;
}