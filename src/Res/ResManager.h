#pragma once

#include <string>
#include <memory>
#include <map>
#include <vector>

namespace Render {
	class ShaderProgram;
	class Texture2D;
	class Sprite;
	class AnimatedSprite;
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

		std::shared_ptr <Render::ShaderProgram> loadShaders(const std::string& shaderName, 
															const std::string& vertexPath, 
															const std::string& fragmentPath);
		std::shared_ptr <Render::ShaderProgram> getShaderProgram(const std::string& shaderName);


		std::shared_ptr <Render::Texture2D> loadTexture(const std::string& textureName, const std::string& texturePath);
		std::shared_ptr <Render::Texture2D> getTextures(const std::string& textureName);

		std::shared_ptr <Render::Sprite> loadSprite(const std::string& spriteName,
												const std::string& textureName, 
												const std::string& shaderName,
												const unsigned int spriteWidth,
												const unsigned int spriteHeight,
												const std::string& subTextureName = "default");
		std::shared_ptr <Render::Sprite> getSprite(const std::string& spriteName);

		std::shared_ptr <Render::Texture2D> loadTextureAtlas(const std::string textureName,
														     const std::string texturePath,
															 const std::vector<std::string> subTextures,
															 const unsigned int subTextureWidth,
															 const unsigned int subTextureHeight);

		std::shared_ptr <Render::AnimatedSprite> loadAnimatedSprite(const std::string& spriteName,
			const std::string& textureName,
			const std::string& shaderName,
			const unsigned int spriteWidth,
			const unsigned int spriteHeight,
			const std::string& subTextureName = "default");
		std::shared_ptr <Render::AnimatedSprite> getAnimatedSprite(const std::string& spriteName);

	private:
		std::string getFileString(const std::string& relativeFilePath) const;

		typedef std::map<const std::string, std::shared_ptr<Render::ShaderProgram>> ShaderProgramsMap;
		ShaderProgramsMap m_shaderPrograms;

		typedef std::map<const std::string, std::shared_ptr<Render::Texture2D>> TexturesMap;
		TexturesMap m_textures;

		typedef std::map<const std::string, std::shared_ptr<Render::Sprite>> SpritesMap;
		SpritesMap m_sprites;

		typedef std::map<const std::string, std::shared_ptr<Render::AnimatedSprite>> AnimatedSpritesMap;
		AnimatedSpritesMap m_animatedSprites;

		std::string m_path;
	};
}