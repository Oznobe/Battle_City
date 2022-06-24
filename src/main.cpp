#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <chrono>

#include"Render/ShaderProgram.h"
#include"Render/Texture2D.h"
#include"Render/Sprite.h"
#include"Render/AnimatedSprite.h"

#include"Res/ResManager.h"

GLfloat point[] = {
    0.0f, 50.f, 0.0f,
    50.f, -50.f, 0.0f,
    -50.f, -50.f, 0.0f
};

GLfloat color[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
};

GLfloat texCoord[] = {
    0.5f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f
};

glm::ivec2 g_WindowSize(640, 480);


void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height) {
    g_WindowSize.x = width;
    g_WindowSize.y = height;
    glViewport(0, 0, g_WindowSize.x, g_WindowSize.y);
}

void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }
}

int main(int argc, char** argv) {
    /* Initialize the library */
    if (!glfwInit()) {
        std::cout << "glfwInit failed\n";
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* pWindow = glfwCreateWindow(g_WindowSize.x, g_WindowSize.y, "Battle City", nullptr, nullptr);
    if (!pWindow) {
        std::cout << "glfwCreateWindow failed\n";
        glfwTerminate();
        return -1;
    }

    glfwSetWindowSizeCallback(pWindow, glfwWindowSizeCallback);
    glfwSetKeyCallback(pWindow, glfwKeyCallback);
    /* Make the window's context current */
    glfwMakeContextCurrent(pWindow);

    if (!gladLoadGL()) {
        std::cout << "Can`t load GLAD\n";
        return -1;
    }
    std::cout << "Render:\t" << glGetString(GL_RENDERER) << "\n";
    std::cout << "OpenGL version:\t" << glGetString(GL_VERSION) << "\n";

    glClearColor(1, 1, 0, 1);

    {
        Resources::ResourceManager resourceManager(argv[0]);

        auto pDefaultShaderProgram = resourceManager.loadShaders("DefaultShader", "resources/shaders/vertex.txt", "resources/shaders/fragment.txt");
        if (!pDefaultShaderProgram) {
            std::cerr << "Can`t load shader program: DefaultShader\n";
            return -1;
        }

        auto pSpriteShaderProgram = resourceManager.loadShaders("SpriteShader", "resources/shaders/vSprite.txt", "resources/shaders/fSprite.txt");
        

        if (!pSpriteShaderProgram) {
            std::cerr << "Can`t load shader program: SpriteShader\n";
            return -1;
        }

        auto tex = resourceManager.loadTexture("DefaultTexture", "resources/textures/map_16x16.png");

        std::vector<std::string> subTextureNames = {
            "block", 
            "topBlock",
            "bottomBlock", 
            "leftBlock", 
            "rightBlock", 
            "topLeftBlock", 
            "topRightBlock", 
            "bottomLeftBlock", 
            "bottomRightBlock", 

            "beton",
            "topBeton",
            "bottomBeton",
            "leftBeton",
            "rightBeton",
            "topLeftBeton",
            "topRightBeton",
            "bottomLeftBeton",
            "bottomRightBeton",

            "water1",
            "water2",
            "water3",

            "trees",

            "ice",

            "wall",

            "eagle",
            "deadEagle",

            "nothing",

            "respawn1",
            "respawn2",
            "respawn3",
            "respawn4"
        };
        auto pTextureAtlas = resourceManager.loadTextureAtlas("DefaultTextureAtlas", "resources/textures/map_16x16.png", std::move(subTextureNames), 16, 16);

        auto pSprite = resourceManager.loadSprite("NewSprite", "DefaultTextureAtlas", "SpriteShader", 100, 100, "beton");
        pSprite->setPosition(glm::vec2(300, 100));

        auto pAnimatedSprite = resourceManager.loadAnimatedSprite("NewAnimatedSprite", "DefaultTextureAtlas", "SpriteShader", 100, 100, "nothing");
        pAnimatedSprite->setPosition(glm::vec2(300, 300));
        std::vector<std::pair<std::string, uint64_t>> waterState;

        waterState.emplace_back(std::make_pair<std::string, uint64_t>("water1", 1e9));
        waterState.emplace_back(std::make_pair<std::string, uint64_t>("water2", 1e9));
        waterState.emplace_back(std::make_pair<std::string, uint64_t>("water3", 1e9));

        std::vector<std::pair<std::string, uint64_t>> eagleState;

        eagleState.emplace_back(std::make_pair<std::string, uint64_t>("eagle", 1e9));
        eagleState.emplace_back(std::make_pair<std::string, uint64_t>("deadEagle", 1e9));

        pAnimatedSprite->insertState("waterState", std::move(waterState));
        pAnimatedSprite->insertState("eagleState", std::move(eagleState));

        pAnimatedSprite->setState("waterState");

        GLuint points_vbo = 0;
        glGenBuffers(1, &points_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);

        GLuint colors_vbo = 0;
        glGenBuffers(1, &colors_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);

        GLuint texCoord_vbo = 0;
        glGenBuffers(1, &texCoord_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, texCoord_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(texCoord), texCoord, GL_STATIC_DRAW);

        GLuint vao = 0;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, texCoord_vbo);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

        pDefaultShaderProgram->use();
        pDefaultShaderProgram->setInt("tex", 0);

        glm::mat4 modelMatrix_1 = glm::mat4(1.f);
        modelMatrix_1 = glm::translate(modelMatrix_1, glm::vec3(100.0f, 50.0f, 0.f));

        glm::mat4 modelMatrix_2 = glm::mat4(1.f);
        modelMatrix_2 = glm::translate(modelMatrix_2, glm::vec3(590.0f, 50.0f, 0.f));

        glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(g_WindowSize.x), 0.f, static_cast<float>(g_WindowSize.y), -100.f, 100.f);

        pDefaultShaderProgram->setMatrix4("projectionMat", projectionMatrix);

        pSpriteShaderProgram->use();
        pSpriteShaderProgram->setInt("tex", 0);
        pSpriteShaderProgram->setMatrix4("projectionMat", projectionMatrix);

        auto lastTime = std::chrono::high_resolution_clock::now();

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(pWindow)) {

            auto currentTime = std::chrono::high_resolution_clock::now();
            uint64_t duration = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - lastTime).count();
            lastTime = currentTime;
            pAnimatedSprite->update(duration);

            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            pDefaultShaderProgram->use();
            glBindVertexArray(vao);
            tex->bind();

            pDefaultShaderProgram->setMatrix4("modelMat", modelMatrix_1);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            pDefaultShaderProgram->setMatrix4("modelMat", modelMatrix_2);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            pSprite->render();

            pAnimatedSprite->render();

            /* Swap front and back buffers */
            glfwSwapBuffers(pWindow);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
    glfwTerminate();
    return 0;
}
