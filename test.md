**OpenGL Shader Code in C++**

This example demonstrates how to create and use a simple vertex shader, fragment shader, and program object.

[] Todo 1
[X] Todo 2

### Vertex Shader

```cpp
// vertex_shader.cpp

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Define the vertex structure for the 3D model
struct Vertex {
    float x, y, z;
    float texCoord;
};

// Define the vertex shader source code
const char* vertexShaderSource = R"(
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}
)";

// Define the fragment shader source code
const char* fragmentShaderSource = R"(
#version 330 core

in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D textureSampler;

void main()
{
    FragColor = texture(textureSampler, TexCoord);
}
)";
```

### Program Setup and Usage
```cpp
// program.cpp

#include <GL/glew.h>
#include <GLFW/glfw3.h>

int main() {
    // Initialize GLFW and GLEW
    if (!glfwInit()) return -1;
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glCreateCapabilities(GL_CreateCapabilities);

    // Create a window
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Shaders", NULL, NULL);
    if (!window) return -1;

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewInit();

    // Create a shader program object
    GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    GLuint programObject = glCreateProgram();

    // Compile and attach shaders to the program object
    glShaderSource(vertexShaderID, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShaderID);

    glShaderSource(fragmentShaderID, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShaderID);

    glAttachShader(programObject, vertexShaderID);
    glAttachShader(programObject, fragmentShaderID);

    // Link the program object
    if (glLinkProgram(programObject) != GL_SUCCESS) {
        printf("Error linking program: %s\n", gluGetErrorString(GLU_ERROR_LOG));
        return -1;
    }

    // Set up texture sampler uniform
    GLuint textureSampler = glGetUniformLocation(programObject, "textureSampler");

    // Create and bind a texture object for testing
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Draw a quad using the shader program
    GLfloat vertices[] = {
        -0.5f,  0.5f,  0.0f,     1.0f,  0.0f,
         0.5f,  0.5f,  0.0f,     0.0f,  1.0f
    };

    GLfloat texCoords[] = {
        0.0f,  0.0f,
        1.0f,  1.0f
    };

    GLuint vboID;
    glGenBuffers(1, &vboID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(texCoords), NULL, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)(sizeof(vertices)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    // Unbind textures and shaders
    glDeleteTextures(1, &textureID);
    glDetachShader(programObject, vertexShaderID);
    glDetachShader(programObject, fragmentShaderID);
    glDeleteProgram(programObject);

    glfwTerminate();

    return 0;
}
```

This example creates a simple vertex shader that takes in position and texture coordinates, as well as a fragment shader that samples from a texture using the provided `textureSampler` uniform. The program object is created by linking the shaders together.

Note: You'll need to include necessary headers (`GL/glew.h`, `GLFW/glfw3.h`) and link against GLEW and GLFW libraries in your project build process.

### Build Instructions
To compile this example, you can use the following commands:

- Compile with GLEW and GLFW:
```bash
g++ -c vertex_shader.cpp -o vertex_shader.o
g++ -c program.cpp -o program.o
g++ vertex_shader.o program.o -lGLEW -lglfw3 -o shader_program
```

- Run the program using the resulting executable:
```bash
./shader_program
```
Note: Make sure to install GLFW and GLEW libraries in your system.



**Introduction to OpenGl and Vulkan**

OpenGl and Vulkan are two popular rendering APIs used for creating 2D and 3D graphics on a wide range of platforms, including Windows, macOS, Linux, Android, and many more.

**OpenGl**

OpenGl is a widely-used, cross-platform API developed by Silicon Graphics (SGI) in the late 1980s. It provides a way to create high-performance 2D and 3D graphics on multiple hardware configurations. OpenGl supports various features, including:

* 2D and 3D rendering
* Shading models (e.g., fixed-function shading)
* Transformations (e.g., rotations, translations)
* Lighting (e.g., point lighting, directional lighting)
* Textures and buffers

OpenGl has been around for over three decades and is still widely used today. However, it has some limitations:

* **Insecure**: OpenGl uses a fixed-function pipeline, which makes it vulnerable to security issues.
* **Less efficient**: The fixed-function pipeline can lead to performance bottlenecks.
* **Less flexible**: OpenGl's API is relatively simple, but it also means that users have limited control over the rendering pipeline.

**Vulkan**

Vulkan is a modern, cross-platform API developed by the Khronos Group (a consortium of companies) in 2015. It was designed to provide improved performance, security, and flexibility compared to OpenGl. Vulkan features:

* **Immediate mode**: Vulkan uses an immediate mode API, which allows for more direct control over the rendering pipeline.
* **Asynchronous processing**: Vulkan supports asynchronous processing, allowing for better multitasking and improved performance.
* **High-performance graphics**: Vulkan provides high-performance graphics capabilities, including support for modern GPU architectures.
* **Multi-threading**: Vulkan supports multi-threading, enabling developers to take full advantage of multi-core processors.

Vulkan also has some benefits over OpenGl:

* **More secure**: Vulkan uses a more secure architecture, with improved protection against common exploits like buffer overflows and heap corruption.
* **Faster development**: Vulkan's API is designed for faster development, with features like immediate mode and asynchronous processing.
* **Better performance**: Vulkan is optimized for modern GPU architectures, providing better performance than OpenGl.



Here is a table summarizing the main differences between OpenGL and Vulkan:

| **Feature** | **OpenGL: A Simple Rendering lib with a lot of abstractions** | **Vulkan: A More complex lib that give low level access with little to no abstractions** |
| --- | --- | --- |
| **Memory Management** | Automatic memory management through shaders | Manual memory management using buffers, images, and descriptors |
| **API Complexity** | Higher-level API with more abstraction (easier to learn but less efficient) | Lower-level API with more direct control (more efficient but harder to learn) |
| **Hardware Acceleration** | Limited to CPU acceleration only | Hardware-accelerated through GPU or ASIC |
| **Performance** | Slower due to overhead of high-level abstractions | Faster due to reduced overhead and manual memory management |
| **Multi-Threading** | Thread-safety issues and difficulty in managing threads | Built-in support for multi-threading, including thread groups and barriers |
| **Error Handling** | Less explicit error handling; errors are reported as exceptions or failures | More explicit error handling through error codes and reports |
| **Async Programming** | Asynchronous programming can be complex and error-prone | Built-in support for asynchronous programming through commands like `vkQueueSubmit` |
| **Resource Management** | Automatic resource management through the OpenGL API | Manual resource management through Vulkan resources (e.g., buffers, images) |
| **Platform Support** | More platform-specific implementation details in the API | More standardized across platforms, with fewer platform-specific details |
| **Versioning** | Major version changes every 2-3 years; backwards compatibility is generally good | New features are added to existing versions through extensions and updates |

In summary, Vulkan provides a lower-level API that requires manual memory management but offers better performance, multi-threading support, and asynchronous programming capabilities. OpenGL, on the other hand, has a higher-level API with more abstraction but requires more overhead and is less efficient.



Here's an example of how you can use GLSL (OpenGL Shading Language) for basic shaders in OpenGL.

### Vertex Shader
```glsl
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTextureCoord;

out vec2 textureCoordinate;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    textureCoordinate = aTextureCoord;
}
```

### Fragment Shader
```glsl
#version 330 core

in vec2 textureCoordinate;

out vec4 FragColor;

uniform sampler2D texture;
uniform float time;

void main()
{
    float c = sin(time + textureCoordinate.x * 10.0);
    vec4 col = texture(texture, textureCoordinate) * vec4(1.0, 1.0, 1.0, c);
    FragColor = col;
}
```

### Main Program

```cpp
#include <GL/glew.h>
#include <iostream>

int main()
{
    // Initialize OpenGL window and create a context.
    if (!InitWindow())
        return 1;

    // Load the shader program from file
    char vertexShaderSource[] = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec2 aTextureCoord;

        out vec2 textureCoordinate;

        void main()
        {
            gl_Position = vec4(aPos, 1.0);
            textureCoordinate = aTextureCoord;
        }
    )";

    char fragmentShaderSource[] = R"(
        #version 330 core

        in vec2 textureCoordinate;

        out vec4 FragColor;

        uniform sampler2D texture;
        uniform float time;

        void main()
        {
            float c = sin(time + textureCoordinate.x * 10.0);
            vec4 col = texture(texture, textureCoordinate) * vec4(1.0, 1.0, 1.0, c);
            FragColor = col;
        }
    )";

    // Create a program and set the shader source
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertexSource = vertexShaderSource;
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    // Compile fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragmentSource = fragmentShaderSource;
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    // Create a program
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    // Check for errors in the shader compilation
    if (glGetError() != GL_NO_ERROR)
        std::cerr << "error" << std::endl;

    // Create a texture and attach it to the program
    GLuint texture = LoadTexture("path_to_your_texture.png");
    glAttachShader(program, GL_TEXTURE2D);

    // Set the uniform variables
    glUniform1f(glGetUniformLocation(program, "time"), 0.0);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Draw a rectangle
    glBegin(GL_QUADS);
        glVertex2f(-0.5, -0.5); // top left
        glVertex2f(0.5, -0.5);   // top right
        glVertex2f(0.5, 0.5);    // bottom right
        glVertex2f(-0.5, 0.5);   // bottom left
    glEnd();

    // Render the scene
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(program);

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up
    glDeleteProgram(program);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteTextures(1, &texture);

    return 0;
}
```

### Initialization Function

```cpp
#include <GL/glew.h>
#include <iostream>

bool InitWindow()
{
    // Initialize OpenGL
    if (glewInit() != GLEW_OK)
        return false;

    // Create a window
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Window", NULL, NULL);
    if (!window)
        return false;

    glfwMakeContextCurrent(window);

    return true;
}
```

### Loading Texture

```cpp
#include <GL/glew.h>
#include <stb_image.h>

GLuint LoadTexture(const char* path)
{
    int width, height, channels;
    unsigned char* data = stbi_load(path, &width, &height, &channels, 0);
    if (!data)
        return 0;

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);

    return textureID;
}
```

This code will create a window and display a rectangle with a texture. The vertex shader specifies the position of each vertex in 3D space, while the fragment shader calculates the color of each pixel based on its coordinates.

Please replace "path_to_your_texture.png" with your own image path.

To compile and run this program, you can use GCC as follows:

```bash
gcc -o main main.cpp -lGL -lGLEW -lopengl32 -lm
./main
```

This is a very basic example of how to create an OpenGL window and display a texture. In a real application, you would likely want to handle errors more robustly, use more advanced techniques for loading textures and shaders, and add additional features such as animations and user input handling.