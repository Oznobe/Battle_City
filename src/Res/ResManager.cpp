#include "ResManager.h"
#include "../Render/ShaderProgram.h"
#include "../Render/Texture2D.h"

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
