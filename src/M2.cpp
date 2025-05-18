#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float rotationX = 0.0f, rotationY = 0.0f, rotationZ = 0.0f;
glm::vec3 position = glm::vec3(0.0f);
float scale = 1.0f;

const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 vertexColor;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    vertexColor = aColor;
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
in vec3 vertexColor;
out vec4 FragColor;
void main()
{
    FragColor = vec4(vertexColor, 1.0);
}
)";

// Cubo com cores por face
float vertices[] = {
    // frente 
    -0.5f, -0.5f,  0.5f, 1, 0, 0,
     0.5f, -0.5f,  0.5f, 1, 0, 0,
     0.5f,  0.5f,  0.5f, 1, 0, 0,
     0.5f,  0.5f,  0.5f, 1, 0, 0,
    -0.5f,  0.5f,  0.5f, 1, 0, 0,
    -0.5f, -0.5f,  0.5f, 1, 0, 0,

    // trás 
    -0.5f, -0.5f, -0.5f, 0, 1, 0,
     0.5f, -0.5f, -0.5f, 0, 1, 0,
     0.5f,  0.5f, -0.5f, 0, 1, 0,
     0.5f,  0.5f, -0.5f, 0, 1, 0,
    -0.5f,  0.5f, -0.5f, 0, 1, 0,
    -0.5f, -0.5f, -0.5f, 0, 1, 0,

    // esquerda 
    -0.5f,  0.5f,  0.5f, 0, 0, 1,
    -0.5f,  0.5f, -0.5f, 0, 0, 1,
    -0.5f, -0.5f, -0.5f, 0, 0, 1,
    -0.5f, -0.5f, -0.5f, 0, 0, 1,
    -0.5f, -0.5f,  0.5f, 0, 0, 1,
    -0.5f,  0.5f,  0.5f, 0, 0, 1,

    // direita 
     0.5f,  0.5f,  0.5f, 1, 1, 0,
     0.5f,  0.5f, -0.5f, 1, 1, 0,
     0.5f, -0.5f, -0.5f, 1, 1, 0,
     0.5f, -0.5f, -0.5f, 1, 1, 0,
     0.5f, -0.5f,  0.5f, 1, 1, 0,
     0.5f,  0.5f,  0.5f, 1, 1, 0,

    // topo 
    -0.5f,  0.5f, -0.5f, 0, 1, 1,
     0.5f,  0.5f, -0.5f, 0, 1, 1,
     0.5f,  0.5f,  0.5f, 0, 1, 1,
     0.5f,  0.5f,  0.5f, 0, 1, 1,
    -0.5f,  0.5f,  0.5f, 0, 1, 1,
    -0.5f,  0.5f, -0.5f, 0, 1, 1,

    // fundo 
    -0.5f, -0.5f, -0.5f, 1, 0, 1,
     0.5f, -0.5f, -0.5f, 1, 0, 1,
     0.5f, -0.5f,  0.5f, 1, 0, 1,
     0.5f, -0.5f,  0.5f, 1, 0, 1,
    -0.5f, -0.5f,  0.5f, 1, 0, 1,
    -0.5f, -0.5f, -0.5f, 1, 0, 1
};

// Cria diferentes cubos
std::vector<glm::vec3> cubePositions = {
    {0.0f, 0.0f, 0.0f},
    {2.0f, 0.0f, -5.0f},
    {-2.0f, 1.0f, -3.0f},
};

// Input do teclado
void processInput(GLFWwindow* window) {
    float moveSpeed = 0.05f;
    float scaleSpeed = 0.02f;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) position.z -= moveSpeed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) position.z += moveSpeed;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) position.x -= moveSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) position.x += moveSpeed;
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) position.y += moveSpeed;
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) position.y -= moveSpeed;
    if (glfwGetKey(window, GLFW_KEY_LEFT_BRACKET) == GLFW_PRESS) scale -= scaleSpeed;
    if (glfwGetKey(window, GLFW_KEY_RIGHT_BRACKET) == GLFW_PRESS) scale += scaleSpeed;

    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) rotationX += 1.0f;
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) rotationY += 1.0f;
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) rotationZ += 1.0f;
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Cubo Interativo - Gabriel Brasil", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glEnable(GL_DEPTH_TEST);

    // Shaders
    int vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertexShaderSource, nullptr);
    glCompileShader(vs);
    int fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fs);
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vs);
    glAttachShader(shaderProgram, fs);
    glLinkProgram(shaderProgram);
    glDeleteShader(vs);
    glDeleteShader(fs);

    // Buffers
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -8.0f));
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH/SCR_HEIGHT, 0.1f, 100.0f);

        for (const auto& offset : cubePositions) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, position + offset);
            model = glm::rotate(model, glm::radians(rotationX), glm::vec3(1, 0, 0));
            model = glm::rotate(model, glm::radians(rotationY), glm::vec3(0, 1, 0));
            model = glm::rotate(model, glm::radians(rotationZ), glm::vec3(0, 0, 1));
            model = glm::scale(model, glm::vec3(scale));

            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
