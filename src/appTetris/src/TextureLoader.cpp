#include "../include/TextureLoader.h"
#include "../include/ExtensionString.h"
#include "../include/SDLlogHelper.h"

#ifndef SDL_LIB
#define SDL_LIB
#include <SDL.h>
#endif

#ifndef FILESYSTEM_H
#define FILESYSTEM_H
#include <filesystem>
#endif

UnorderedMap<std::string, std::unique_ptr<SDL_Texture, SDLWrapper::FreeSDLTexture>> TextureLoader::GetTextures(
	std::function<SDL_Texture* (const std::string&, SDL_Renderer* const)> FuncPtrArg, SDL_Renderer* RendererPtrArg)
{
	UnorderedMap<std::string, std::unique_ptr<SDL_Texture, SDLWrapper::FreeSDLTexture>> TextureMap;
	if (!RendererPtrArg)
	{
		SDLlogHelper::Print(PrefixErrorType::LibraryInitFailed, "TextureLoader");
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
				SDLlogHelper::Print(PrefixErrorType::InvalidPtr, "TextureLoader");
				continue;
			}

			std::vector<std::string>&& Tokens = ExtensionString::Split(SFilePath, '/');
			if (Tokens.empty())
			{
				SDLlogHelper::Print(PrefixErrorType::InvalidPtr, "TextureLoader");
				continue;
			}

			TextureMap.insert(std::make_pair(Tokens.at(Tokens.size() - 1), TargetTexturePtr));
		}
	}
	catch (const std::exception& e)
	{
		SDLlogHelper::Print(PrefixErrorType::CollectionAccessFailed, "TextureLoader", e);
	}
	return TextureMap;
}