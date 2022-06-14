#include "ResManager.h"
#include "../Render/ShaderProgram.h"
#include "../Render/Texture2D.h"
#include "../Render/Sprite.h"

#include <sstream>
#include <fstream>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb_image.h"

Resources::ResourceManager::ResourceManager(const std::string& executablePath) {
	size_t found = executablePath.find_last_of("/\\");
	m_path = executablePath.substr(0, found);
}

std::shared_ptr<Render::ShaderProgram> Resources::ResourceManager::loadShaders(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath) {
	std::string vertexString = getFileString(vertexPath);
	if (vertexString.empty()) {
		std::cerr << "No vertex shader\n";
		return nullptr;
	}
	std::string fragmentString = getFileString(fragmentPath);
	if (fragmentString.empty()) {
		std::cerr << "No fragment shader\n";
		return nullptr;
	}
	std::shared_ptr<Render::ShaderProgram>& newShader = m_shaderPrograms.emplace(shaderName, std::make_shared<Render::ShaderProgram>(vertexString, fragmentString)).first->second;
	if (newShader->isCompiled()) {
		return newShader;
	}
	std::cerr << "Can`t load shder program:\nVertex: " + vertexPath + "\nFragment: " + fragmentPath + "\n";
	return nullptr;
}

std::shared_ptr<Render::ShaderProgram> Resources::ResourceManager::getShaderProgram(const std::string& shaderName) {
	ShaderProgramsMap::const_iterator it = m_shaderPrograms.find(shaderName);
	if (it != m_shaderPrograms.end()) {
		return it->second;
	}
	std::cerr << "Can`t find the shader pragram: " + shaderName + "\n";
	return nullptr;
}

std::shared_ptr<Render::Texture2D> Resources::ResourceManager::loadTexture(const std::string& textureName, const std::string& texturePath) {
	int channels = 0;
	int width = 0;
	int height = 0;

	stbi_set_flip_vertically_on_load(true);
	unsigned char* pixels = stbi_load(std::string(m_path + "/" + texturePath).c_str(), &width, &height, &channels, 0);

	if (!pixels) {
		std::cerr << "Can`t oad image: " + texturePath + "\n";
		return nullptr;
	}
	std::shared_ptr<Render::Texture2D> newTextrue = m_textures.emplace(textureName, std::make_shared<Render::Texture2D>(width, height, pixels, channels, GL_NEAREST, GL_CLAMP_TO_EDGE)).first->second;
	stbi_image_free(pixels);
	return newTextrue;
}

std::shared_ptr<Render::Texture2D> Resources::ResourceManager::getTextures(const std::string& textureName) {
	TexturesMap::const_iterator it = m_textures.find(textureName);
	if (it != m_textures.end()) {
		return it->second;
	}
	std::cerr << "Can`t find the textrue: " + textureName + "\n";
	return nullptr;
}

std::shared_ptr<Render::Sprite> Resources::ResourceManager::loadSprite(const std::string& spriteName, 
																	   const std::string& textureName, 
																	   const std::string& shaderName, 
																	   const unsigned int spriteWidth, 
																	   const unsigned int spriteHeight,
																	   const std::string& subTextureName) {
	auto pTexture = getTextures(textureName);
	if (!pTexture) {
		std::cerr <<  "Can`t open the texture" + textureName + " for the sprite: " + spriteName + "\n";
	}
	auto pShader = getShaderProgram(shaderName);
	if (!pShader) {
		std::cerr << "Can`t open the shader" + shaderName + " for the sprite: " + spriteName + "\n";
	}
	std::shared_ptr<Render::Sprite> newSprite = m_sprites.emplace(textureName, 
																  std::make_shared<Render::Sprite>(pTexture, 
																								   subTextureName,
																								   pShader, 
																								   glm::vec2(0.f, 0.f), 
																								   glm::vec2(spriteWidth, spriteHeight))).first->second;
	return newSprite;
}

std::shared_ptr<Render::Sprite> Resources::ResourceManager::getSprite(const std::string& spriteName)
{
	SpritesMap::const_iterator it = m_sprites.find(spriteName);
	if (it != m_sprites.end()) {
		return it->second;
	}
	std::cerr << "Can`t find the sprite: " + spriteName + "\n";
	return nullptr;
}

std::shared_ptr<Render::Texture2D> Resources::ResourceManager::loadTextureAtlas(const std::string textureName,
																				const std::string texturePath,
																				const std::vector<std::string> subTextures,
																				const unsigned int subTextureWidth, 
																				const unsigned int subTextureHeight) {
	auto pTexture = loadTexture(std::move(textureName), std::move(texturePath));

	if (pTexture) {
		const unsigned int textureWidth = pTexture->width();
		const unsigned int textureHeight = pTexture->height();

		unsigned int currentTextureOffsetX = 0;
		unsigned int currentTextureOffsetY = textureHeight;

		for (const auto& currentSubTextureName : subTextures) {

			glm::vec2 leftBottomUV(static_cast<float>(currentTextureOffsetX) / textureWidth, static_cast<float>(currentTextureOffsetY - subTextureHeight) / textureHeight);
			glm::vec2 rightTopUV(static_cast<float>(currentTextureOffsetX + subTextureWidth) / textureWidth, static_cast<float>(currentTextureOffsetY) / textureHeight);
			pTexture->addSubTexture(std::move(currentSubTextureName), leftBottomUV, rightTopUV);

			currentTextureOffsetX += subTextureWidth;

			if (currentTextureOffsetX >= textureWidth) {
				currentTextureOffsetX = 0;
				currentTextureOffsetY -= subTextureHeight;
			}
			
		}
	}
	return pTexture;
}

std::string Resources::ResourceManager::getFileString(const std::string& relativeFilePath) const {
	std::ifstream f;
	f.open(m_path + "/" + relativeFilePath.c_str(), std::ios::in | std::ios::binary);
	if (!f.is_open()) {
		std::cerr << "Failed to open file: " + relativeFilePath + "\n";
		return std::string{};
	}
	std::stringstream buffer;
	buffer << f.rdbuf();

	return buffer.str();
}
