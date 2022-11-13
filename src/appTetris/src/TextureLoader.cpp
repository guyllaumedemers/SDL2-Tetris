#include "../include/TextureLoader.h"
#include "../include/StringExtension.h"

#ifndef INCLUDED_STD_FILESYSTEM
#define INCLUDED_STD_FILESYSTEM
#include <filesystem>
#endif

std::unordered_map<std::string, std::unique_ptr<SDL_Texture, FreeSDLTexture>> TextureLoader::GetTextures(std::function<SDL_Texture* (const std::string&, SDL_Renderer* const)> FuncPtrArg,
	SDL_Renderer* RendererPtrArg)
{
	std::unordered_map<std::string, std::unique_ptr<SDL_Texture, FreeSDLTexture>> TextureMap;
	if (!RendererPtrArg)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: TEXTURE_LOADER INIT FAILED!");
		return TextureMap;
	}

	try
	{
		std::filesystem::path&& BuildPath = std::filesystem::current_path().parent_path();
		std::string&& ResPath = BuildPath.generic_string();

		{
			// Workaround for ressources access using msvc open mode and debug
			std::string&& EditorResPATH = ResPath + std::string("/SDL2-TetrisApp/res");
			std::string&& OpenFileModePATH = ResPath + std::string("/res");

			std::filesystem::is_directory(EditorResPATH) ? ResPath = EditorResPATH : ResPath = OpenFileModePATH;
		}

		for (const auto& file : std::filesystem::directory_iterator(ResPath))
		{
			const std::filesystem::path& FilePath = file.path();
			const std::string&& SFilePath = FilePath.generic_string();

			SDL_Texture* const TargetTexturePtr = FuncPtrArg(SFilePath, RendererPtrArg);
			if (!TargetTexturePtr)
			{
				SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: TEXTURE_TARGET INVALID!");
				continue;
			}

			std::vector<std::string>&& Tokens = StringExtension::Split(SFilePath, '/');
			if (Tokens.empty())
			{
				SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: PATH_FORMAT DELIMITER INVALID!");
				continue;
			}

			TextureMap.insert(std::make_pair(Tokens.at(Tokens.size() - 1), TargetTexturePtr));
		}
	}
	catch (const std::exception& e)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: TEXTURE_LOADER FAILED! %s", e.what());
	}
	return TextureMap;
}