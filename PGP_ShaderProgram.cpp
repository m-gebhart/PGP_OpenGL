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
    GLuint ProjectionID = glGetUniformLocation(program, "P");
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)1920 / (float)1280, 0.1f, 100.0f);
    glUniformMatrix4fv(ProjectionID, 1, GL_FALSE, &projection[0][0]);

    GLuint viewID = glGetUniformLocation(program, "V");
    glm::vec3 cameraPos = glm::vec3(0, 1, 4);
    glm::mat4 view = glm::lookAt(
        cameraPos,
        glm::vec3(0, 0, 0),
        glm::vec3(0, 1, 0)
    );
    glUniformMatrix4fv(viewID, 1, GL_FALSE, &view[0][0]);

    GLuint modelID = glGetUniformLocation(program, "M");
    glm::mat4 model = glm::mat4(1.0f);
    glUniformMatrix4fv(modelID, 1, GL_FALSE, &model[0][0]);

    glm::mat4 mvp = projection * view * model;
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