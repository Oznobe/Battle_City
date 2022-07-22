#include "Game.h"

#include "../Res/ResManager.h"
#include "../Render/ShaderProgram.h"
#include "../Render/Texture2D.h"
#include "../Render/Sprite.h"
#include "../Render/AnimatedSprite.h"

#include<iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>


Game::Game(const glm::ivec2& windowSize)
    : m_eCurrentGameState(EGameState::Active)
    , m_windowSize(windowSize)
{
    m_Keys.fill(false);
}

Game::~Game()
{
}

void Game::render()
{
    Resources::ResourceManager::getAnimatedSprite("NewAnimatedSprite")->render();
}

void Game::update(const uint64_t delta)
{
    Resources::ResourceManager::getAnimatedSprite("NewAnimatedSprite")->update(delta);
}

void Game::setKey(const int key, const int action)
{
	m_Keys[key] = action;
}

bool Game::init()
{
    auto pDefaultShaderProgram = Resources::ResourceManager::loadShaders("DefaultShader", "resources/shaders/vertex.txt", "resources/shaders/fragment.txt");
    if (!pDefaultShaderProgram) {
        std::cerr << "Can`t load shader program: DefaultShader\n";
        return false;
    }

    auto pSpriteShaderProgram = Resources::ResourceManager::loadShaders("SpriteShader", "resources/shaders/vSprite.txt", "resources/shaders/fSprite.txt");


    if (!pSpriteShaderProgram) {
        std::cerr << "Can`t load shader program: SpriteShader\n";
        return false;
    }

    auto tex = Resources::ResourceManager::loadTexture("DefaultTexture", "resources/textures/map_16x16.png");

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
    auto pTextureAtlas = Resources::ResourceManager::loadTextureAtlas("DefaultTextureAtlas", "resources/textures/map_16x16.png", std::move(subTextureNames), 16, 16);

    auto pAnimatedSprite = Resources::ResourceManager::loadAnimatedSprite("NewAnimatedSprite", "DefaultTextureAtlas", "SpriteShader", 100, 100, "nothing");
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

    pDefaultShaderProgram->use();
    pDefaultShaderProgram->setInt("tex", 0);

    glm::mat4 modelMatrix_1 = glm::mat4(1.f);
    modelMatrix_1 = glm::translate(modelMatrix_1, glm::vec3(100.0f, 50.0f, 0.f));

    glm::mat4 modelMatrix_2 = glm::mat4(1.f);
    modelMatrix_2 = glm::translate(modelMatrix_2, glm::vec3(590.0f, 50.0f, 0.f));

    glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(m_windowSize.x), 0.f, static_cast<float>(m_windowSize.y), -100.f, 100.f);

    pDefaultShaderProgram->setMatrix4("projectionMat", projectionMatrix);

    pSpriteShaderProgram->use();
    pSpriteShaderProgram->setInt("tex", 0);
    pSpriteShaderProgram->setMatrix4("projectionMat", projectionMatrix);

    return true;
}
