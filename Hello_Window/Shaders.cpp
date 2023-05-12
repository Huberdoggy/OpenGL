#include "Hello_Window.h"


void compileShader(unsigned* vertexShader, const char* shaderSrc, int& success,
    char* infoLog) {

    glShaderSource(*vertexShader, 1, &shaderSrc, NULL);
    glCompileShader(*vertexShader);
    // And check compile errors...
    glGetShaderiv(*vertexShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(*vertexShader, sizeof(*infoLog) / sizeof(char), NULL, infoLog);
        std::cout << "ERROR! Shader VERTEX compilation failed\n"
            << infoLog << std::endl;
        return;
    }
}

// Overload
void compileShader(unsigned& fragShader, const char* shaderSrc, int& success,
    char* infoLog) {

    glShaderSource(fragShader, 1, &shaderSrc, NULL);
    glCompileShader(fragShader);

    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(fragShader, sizeof(*infoLog) / sizeof(char), NULL, infoLog);
        std::cout << "ERROR! Shader FRAGMENT compilation failed\n"
            << infoLog << std::endl;
        return;
    }
}



void activateShaderProg(unsigned& shaderProg, const unsigned* vertexShader,
   const unsigned& fragShader, int& success, char* infoLog) {

    glAttachShader(shaderProg, *vertexShader);
    glAttachShader(shaderProg, fragShader);
    glLinkProgram(shaderProg);

    glGetProgramiv(shaderProg, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProg, sizeof(*infoLog) / sizeof(char), NULL, infoLog);
        std::cout << "ERROR! Shader PROGRAM LINKING failed\n"
            << infoLog << std::endl;
        return;
    }
}


std::pair<int, float> alterColor(const unsigned& shaderProg, std::string& myColor) {

    std::pair<int, float> colorPair;
    double timeValue = glfwGetTime();
    float greenValue = (sin(static_cast<float>(timeValue)) / 2.0f) + 0.5f;
    int vertexColorLocation = glGetUniformLocation(shaderProg, myColor.c_str());

    colorPair.first = vertexColorLocation;
    colorPair.second = greenValue;
    return colorPair;
}


