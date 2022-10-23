#include "../include/TextureLoader.h"
#include "../include/StringExtension.h"

#ifndef INCLUDED_STD_FILESYSTEM
#define INCLUDED_STD_FILESYSTEM
#include <filesystem>
#endif

std::unordered_map<std::string, std::unique_ptr<SDL_Texture, FreeSDLTexture>> TextureLoader::GetTextures(std::function<SDL_Texture* (std::string, SDL_Renderer* const)> FuncPtr,
	SDL_Renderer* Renderer)
{
	std::unordered_map<std::string, std::unique_ptr<SDL_Texture, FreeSDLTexture>> TextureMap;
	if (!Renderer)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: TEXTURE_LOADER INIT FAILED!");
		return TextureMap;
	}

	std::filesystem::path BuildPath = std::filesystem::current_path().parent_path();
	std::string ResPath = BuildPath.generic_string();

	// bad solution which doesnt allow me to work in VSC Editor but allow open file mode
#ifndef NDEBUG
	ResPath += "/res";
#else
	ResPath += "/SDL2-TetrisApp/res";
#endif

	for (const auto& file : std::filesystem::directory_iterator(ResPath))
	{
		std::filesystem::path FilePath = file.path();
		std::string SFilePath = FilePath.generic_string();

		SDL_Texture* const TargetTexture = FuncPtr(SFilePath, Renderer);
		if (!TargetTexture)
		{
			std::printf("\n Texture Invalid");
			SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: TEXTURE_TARGET INVALID!");
			continue;
		}
		std::vector<std::string> Tokens = StringExtension::Split(SFilePath, '/');
		if (Tokens.empty())
		{
			SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: PATH_FORMAT DELIMITER INVALID!");
			continue;
		}
		TextureMap.insert(std::make_pair(Tokens.at(Tokens.size() - 1), TargetTexture));
	}
	return TextureMap;
}