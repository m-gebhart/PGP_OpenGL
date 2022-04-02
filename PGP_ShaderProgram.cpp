#include "PGP_ShaderProgram.h"

GLuint PGP_ShaderProgram::CreateShader(GLuint shaderType, const std::string& sourceText)
{
    GLuint shaderID = glCreateShader(shaderType);
    const char* src = sourceText.c_str();
    glShaderSource(shaderID, 1, &src, nullptr);
    glCompileShader(shaderID);
    CheckForError(shaderID);
    return shaderID;
}

bool PGP_ShaderProgram::CheckForError(int shaderID)
{
    int result;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);
        GLchar message[1000];
        glGetShaderInfoLog(shaderID, length, &length, message);
        std::cout << "\nERROR (" << shaderID << "):\n" << message << std::endl;
        glDeleteShader(shaderID);
        return 0;
    }
    return 1;
}

GLuint PGP_ShaderProgram::CreateProgram(const std::string& vertexShaderSrc, const std::string& fragmentShaderSrc)
{
    std::vector<GLuint> shaderList;
    shaderList.push_back(PGP_ShaderProgram::CreateShader(GL_VERTEX_SHADER, vertexShaderSrc));
    shaderList.push_back(PGP_ShaderProgram::CreateShader(GL_FRAGMENT_SHADER, fragmentShaderSrc));

    GLuint program = glCreateProgram();

    for (GLuint shader : shaderList)
        glAttachShader(program, shader);

    glLinkProgram(program);
    glValidateProgram(program);

    for (GLuint shader : shaderList)
        glDeleteShader(shader);

    return program;
}

std::string PGP_ShaderProgram::vertexShader;
std::string PGP_ShaderProgram::fragmentShader;

GLuint PGP_ShaderProgram::CreateAndUseNewProgram()
{
    PGP_ShaderProgram::ReadFile("Vertex.glsl", vertexShader);
    PGP_ShaderProgram::ReadFile("Fragment.glsl", fragmentShader);
    GLuint newProgram = PGP_ShaderProgram::CreateProgram(vertexShader, fragmentShader);
    PGP_ShaderProgram::UseProgram(newProgram);
    return newProgram;
}

void PGP_ShaderProgram::UseProgram(GLuint program)
{
    glUseProgram(program);
}

void PGP_ShaderProgram::ReadFile(const std::string fileName, std::string& output)
{
    ifstream file(fileName);
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    string buffer(size, ' ');
    file.seekg(0);
    file.read(&buffer[0], size);
    output = buffer;
}