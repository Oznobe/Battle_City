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
		ResourceManager() = delete;
		~ResourceManager() = delete;

		ResourceManager(const ResourceManager&) = delete;
		ResourceManager& operator=(const ResourceManager&) = delete;
		ResourceManager& operator=(ResourceManager&&) = delete;
		ResourceManager(ResourceManager&&) = delete;

		static void SetExecutablePath(const std::string& executablePath);
		static void unloadAllResources();

		static std::shared_ptr <Render::ShaderProgram> loadShaders(const std::string& shaderName, 
															const std::string& vertexPath, 
															const std::string& fragmentPath);
		static std::shared_ptr <Render::ShaderProgram> getShaderProgram(const std::string& shaderName);


		static std::shared_ptr <Render::Texture2D> loadTexture(const std::string& textureName, const std::string& texturePath);
		static std::shared_ptr <Render::Texture2D> getTextures(const std::string& textureName);

		static std::shared_ptr <Render::Sprite> loadSprite(const std::string& spriteName,
												const std::string& textureName, 
												const std::string& shaderName,
												const unsigned int spriteWidth,
												const unsigned int spriteHeight,
												const std::string& subTextureName = "default");
		static std::shared_ptr <Render::Sprite> getSprite(const std::string& spriteName);

		static std::shared_ptr <Render::Texture2D> loadTextureAtlas(const std::string textureName,
														     const std::string texturePath,
															 const std::vector<std::string> subTextures,
															 const unsigned int subTextureWidth,
															 const unsigned int subTextureHeight);

		static std::shared_ptr <Render::AnimatedSprite> loadAnimatedSprite(const std::string& spriteName,
			const std::string& textureName,
			const std::string& shaderName,
			const unsigned int spriteWidth,
			const unsigned int spriteHeight,
			const std::string& subTextureName = "default");
		static std::shared_ptr <Render::AnimatedSprite> getAnimatedSprite(const std::string& spriteName);

	private:
		static std::string getFileString(const std::string& relativeFilePath);

		typedef std::map<const std::string, std::shared_ptr<Render::ShaderProgram>> ShaderProgramsMap;
		static ShaderProgramsMap m_shaderPrograms;

		typedef std::map<const std::string, std::shared_ptr<Render::Texture2D>> TexturesMap;
		static TexturesMap m_textures;

		typedef std::map<const std::string, std::shared_ptr<Render::Sprite>> SpritesMap;
		static SpritesMap m_sprites;

		typedef std::map<const std::string, std::shared_ptr<Render::AnimatedSprite>> AnimatedSpritesMap;
		static AnimatedSpritesMap m_animatedSprites;

		static std::string m_path;
	};
}