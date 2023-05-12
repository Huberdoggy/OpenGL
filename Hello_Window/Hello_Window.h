#pragma once

#include <glad/glad.h> // Include GLAD first - .c file contains required OpenGL headers
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow* window);

void compileShader(unsigned* vertexShader, const char* shaderSrc, int& success,
    char* infoLog);

void compileShader(unsigned& fragShader, const char* shaderSrc, int& success,
    char* infoLog);

void activateShaderProg(unsigned& shaderProg, const unsigned* vertexShader,
   const unsigned& fragShader, int& success, char* infoLog);
