#pragma once

#include <string>
#include <memory>
#include <map>

namespace Render {
	class ShaderProgram;
	class Texture2D;
}

namespace Resources {
	class ResourceManager {
	public:
		ResourceManager(const std::string& executablePath);
		~ResourceManager() = default;

		ResourceManager(const ResourceManager&) = delete;
		ResourceManager& operator=(const ResourceManager&) = delete;
		ResourceManager& operator=(ResourceManager&&) = delete;
		ResourceManager(ResourceManager&&) = delete;

		std::shared_ptr <Render::ShaderProgram> loadShaders(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath);
		std::shared_ptr <Render::ShaderProgram> getShaderProgram(const std::string& shaderName);


		std::shared_ptr <Render::Texture2D> loadTexture(const std::string& textureName, const std::string& texturePath);
		std::shared_ptr <Render::Texture2D> getTextures(const std::string& textureName);


	private:
		std::string getFileString(const std::string& relativeFilePath) const;

		typedef std::map<const std::string, std::shared_ptr<Render::ShaderProgram>> ShaderProgramsMap;
		ShaderProgramsMap m_shaderPrograms;

		typedef std::map<const std::string, std::shared_ptr<Render::Texture2D>> TexturesMap;
		TexturesMap m_textures;

		std::string m_path;
	};
}