#include "../include/SDLlogHelper.h"

#ifndef SDL_LIB
#define SDL_LIB
#include <SDL.h>
#endif

void SDLlogHelper::Print(PrefixErrorType ErrorType, std::string ClassName, const std::exception& TryCatchException)
{
	static std::string Prefix = std::string("Error: ");
	static std::string ErrorLine = std::string(/*__LINE__*/);
	switch (ErrorType)
	{
	case PrefixErrorType::None:
		break;
	case PrefixErrorType::InvalidPtr:
		Prefix += std::string("Invalid Ptr Arg on line " + ErrorLine + "in " + ClassName + " Class.");
		break;
	case PrefixErrorType::InvalidPtrInDelegate:
		Prefix += std::string("Invalid Ptr Arg on line " + ErrorLine + "in " + ClassName + " Delegate.");
		break;
	case PrefixErrorType::InvalidPtrInTimerDelegate:
		Prefix += std::string("Invalid Ptr Arg on line " + ErrorLine + "in " + ClassName + " Timer Delegate.");
		break;
	case PrefixErrorType::LibraryInitFailed:
		Prefix += std::string("Library Initialization Failed on line " + ErrorLine + "in " + ClassName + ".");
		break;
	case PrefixErrorType::CollectionAccessFailed:
		Prefix += std::string("Try-Catch - Collection Access Failed on line " + ErrorLine + "in " + ClassName + ". " + TryCatchException.what());
		break;
	default:
		break;
	}
	SDL_LogError(SDL_LOG_CATEGORY_ERROR, Prefix.c_str());
}