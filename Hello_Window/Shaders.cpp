#include "Hello_Window.h"


void compileShader(unsigned* vertexShader, const char* shaderSrc, int& success,
    char* infoLog) {

    glShaderSource(*vertexShader, 1, &shaderSrc, NULL);
    glCompileShader(*vertexShader);
    // And check compile errors again...
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
    // And check compile errors again...
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

    // And check for errors
    glGetProgramiv(shaderProg, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProg, sizeof(*infoLog) / sizeof(char), NULL, infoLog);
        std::cout << "ERROR! Shader PROGRAM LINKING failed\n"
            << infoLog << std::endl;
        return;
    }
}

