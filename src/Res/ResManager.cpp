#include "ResManager.h"
#include "../Render/ShaderProgram.h"

#include <sstream>
#include <fstream>
#include <iostream>

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
