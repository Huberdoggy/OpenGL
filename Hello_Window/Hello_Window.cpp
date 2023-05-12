#include "Hello_Window.h"

/************************Global settings***************************************/
// window specs
constexpr unsigned SRC_WIDTH{ 800 };
constexpr unsigned SRC_HEIGHT{ 600 };

// vertex shader
const char* vertexShaderSrc = "#version 410 core\n"
"layout (location = 0) in vec3 aPos;\n" // vec3 input var corresponding to 3D coord

"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n" // Used as output of shader
"}\0"; // we need to cast to vec4 by setting w param to 1.0f

// Yellow
const char* fragmentShaderSrc_Y = "#version 410 core\n"
"out vec4 FragColor;\n" // Assign its out var to the value input from vertex

"uniform vec4 kyleColor;\n" // global

"void main()\n"
"{\n"
"   FragColor = kyleColor;\n" // RGBA vals between 0.0 and 1.0. Dynamic vals returned from alterColor & passed as params to glUniform4f in render
"}\0";

// Firey Orange
const char* fragmentShaderSrc_O = "#version 410 core\n"
"out vec4 FragColor;\n" // Only req is 1 output var
"void main()\n"
"{\n"
"   FragColor = vec4(0.93f, 0.35f, 0.0f, 1.0f);\n"
"}\0";



/********************************BEGIN MAIN************************************/
int main() {

    glfwInit(); // Init library, on success, call terminate before app end to free resources
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Eliminate uneeded legacy features

    GLFWwindow* window = glfwCreateWindow(SRC_WIDTH, SRC_HEIGHT, "Kyle's Window", NULL, NULL);
    // pass width, height, and title, ignore other two params for now
    // returns ptr to window object
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window!" << '\n';
        glfwTerminate(); // Free resources, requires re-calling init
        return 1;
    }

    glfwMakeContextCurrent(window); // Make window object main context on curr thread 
    // Pass our object & callback func as params to register with each window resize
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Init GLAD to manage function ptrs
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    { // Now that a context is set on calling thread, returns address of specified
      // OpenGL function per OS (definition in glad.c)
        std::cout << "Failed to init GLAD!" << '\n';
        return 1;
    }

    // To store status for any compile errors
    int success;
    char infoLog[512];

    // create vertex shader & attach our source code (C String in globals)
    // and compile
    unsigned* vertexPtr = nullptr, vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    vertexPtr = &vertexShader;
    compileShader(vertexPtr, vertexShaderSrc, success, infoLog);

    // Fragment shaders
    unsigned fragShaderY = glCreateShader(GL_FRAGMENT_SHADER);
    unsigned fragShaderO = glCreateShader(GL_FRAGMENT_SHADER);
    compileShader(fragShaderO, fragmentShaderSrc_O, success, infoLog);
    compileShader(fragShaderY, fragmentShaderSrc_Y, success, infoLog);

    // Create shader program, link our shaders, and activate it for render calls
    unsigned shaderProgram1 = glCreateProgram(),
        shaderProgram2 = glCreateProgram(); // returns ID referencing new prog obj
    activateShaderProg(shaderProgram1, vertexPtr, fragShaderO, success, infoLog);
    activateShaderProg(shaderProgram2, vertexPtr, fragShaderY, success, infoLog);
    // Del no longer needed shader objs post-linking
    vertexPtr = nullptr;
    glDeleteShader(vertexShader);
    glDeleteShader(fragShaderO);
    glDeleteShader(fragShaderY);

    // init array of vertices (normalized range so they're visible) 'z' = 0.0
    // since we're rendering 2D here. For single triangle
    float vertices[] = {
        // Triangle One
        -0.9f, -0.9f, 0.0f, // left
        -0.5f, -0.9f, 0.0f, // right
        -0.7f, 0.25f, 0.0f, // top

        // Triangle Two
        0.5f, -0.9f, 0.0f, // left
        0.9f, -0.9f, 0.0f, // right
        0.7f, 0.25f, 0.0f, // top

        // Triangle Three
        -0.4f, 0.75f, 0.0f, // left
        0.4f, 0.75f, 0.0f, // right
        0.0f, -0.75f, 0.0f, // center tip (inverted)

    };

    // Init vertex array obj to store vertex attribute configuration
    // Init vertex buffer obj to batch send vertices to the GPU
    // Generate a IDs by passing addresses as second params
    unsigned vBuffObj, vArrayObj;
    glGenVertexArrays(1, &vArrayObj);
    glGenBuffers(1, &vBuffObj);

    glBindVertexArray(vArrayObj);
    // Now, bind it - future calls to GL_ARRAY_BUFFER will configure our currently bound obj
    glBindBuffer(GL_ARRAY_BUFFER, vBuffObj);
    // Now, copy vertex data into buff memory
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Static draw appropriate here - position change infrequent, used many times

    // Define attributes - 1st corresponds to our layout loc, second is vec3
    // third is type (float in this case) false for normalization - our input already is
    // stride = space between consecutive vertex attribs - (x,y,z) * sizeof(float)
    // Lastly, offset cast - here, position data is at array beginning aka zero
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    // This is okay, the call to glVertexAttribPointer registered 
    // vBuffObj as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other vArrayObj calls won't accidentally
    // modify this VAO, but this rarely happens. Modifying other VAOs requires 
    // a call to glBindVertexArray anyways so we generally don't unbind VAO/VBO's when it's not directly necessary.
    glBindVertexArray(0);

    std::string kyleColor = "kyleColor"; // Will be converted to GLchar for use with uniform color lookup in alterColor
    // Keep alive
    while (!glfwWindowShouldClose(window)) // Has instruction to close been received?
    {

        processInput(window);

        // Render - set state with fill color each frame, and specify color buff to
        // glClear
        glClearColor(0.11f, 0.14f, 0.12f, 1.0f); // Params float RGBA (int HTML val / 255)
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw triangle
        glUseProgram(shaderProgram1);
        glBindVertexArray(vArrayObj);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // set wireframe mode
        glDrawArrays(GL_TRIANGLES, 0, 6); // primitive type, start index of vertex arr, and how many vertices to draw
        // (aka num triangles)
        glBindVertexArray(0);
        
        // Update global uniform color and bind to third triangle
        glUseProgram(shaderProgram2);
        std::pair<int, float> p = alterColor(shaderProgram2, kyleColor);
        glUniform4f(p.first, 0.0f, p.second, 0.0f, 1.0f);
        glBindVertexArray(vArrayObj);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawArrays(GL_TRIANGLES, 6, 3);

        glfwSwapBuffers(window); // Prevents artifcats via displaying all drawn 
        // commands on back buff to the front
        glfwPollEvents();
    }

    glfwTerminate(); // Cleanup
    return 0;
}
/*************************END MAIN*********************************************/


// Define the resize callback function for the window
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // Set viewport of window coordinates = first 2 are lower left corner, 
    // last 2 are width/height in pixels.
    glViewport(0, 0, width, height);
}


void processInput(GLFWwindow* window) { // Check each iteration in main
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) // Press Expands to true
    {
        glfwSetWindowShouldClose(window, true);
    }
}
