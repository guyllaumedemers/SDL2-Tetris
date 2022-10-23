#include "../include/TextureLoader.h"
#include "../include/StringExtension.h"
#include <filesystem>

std::unordered_map<std::string, std::unique_ptr<SDL_Texture, FreeSDLTexture>> TextureLoader::GetTextures(std::function<SDL_Texture* (std::string, SDL_Renderer*)> FuncPtr,
	SDL_Renderer* Renderer)
{
	std::unordered_map<std::string, std::unique_ptr<SDL_Texture, FreeSDLTexture>> TextureMap;
	std::filesystem::path BuildPath = std::filesystem::current_path().parent_path();
	std::string ResPath = BuildPath.generic_string() + "/res";
	for (const auto& file : std::filesystem::directory_iterator(ResPath)) {
		std::filesystem::path FilePath = file.path();
		std::string SFilePath = FilePath.generic_string();
		SDL_Texture* TargetTexture = FuncPtr(SFilePath, Renderer);
		std::vector<std::string> Tokens;
		Tokens = StringExtension::Split(SFilePath, '_');
		if (TargetTexture != nullptr) TextureMap.insert(std::make_pair(Tokens.at(Tokens.size() - 1), TargetTexture));
	}
	return TextureMap;
}