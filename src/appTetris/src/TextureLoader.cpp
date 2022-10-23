#include "../include/TextureLoader.h"
#include "../include/StringExtension.h"

// exceptional Include guard in translation unit
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
		return TextureMap;
	}

	std::filesystem::path BuildPath = std::filesystem::current_path().parent_path();
	std::string ResPath = BuildPath.generic_string() + "/SDL2-TetrisApp/res";

	for (const auto& file : std::filesystem::directory_iterator(ResPath))
	{
		std::filesystem::path FilePath = file.path();
		std::string SFilePath = FilePath.generic_string();

		SDL_Texture* const TargetTexture = FuncPtr(SFilePath, Renderer);
		if (!TargetTexture)
		{
			continue;
		}
		std::vector<std::string> Tokens = StringExtension::Split(SFilePath, '/');
		if (Tokens.empty())
		{
			continue;
		}
		TextureMap.insert(std::make_pair(Tokens.at(Tokens.size() - 1), TargetTexture));
	}
	return TextureMap;
}