#ifndef LOGHELPER_H
#define LOGHELPER_H

#ifndef STRING_H
#define STRING_H
#include <string>
#endif

#ifndef PREFIXERROR_ENUM
#define PREFIXERROR_ENUM
enum class PrefixErrorType
{
	None = 0,
	InvalidPtr = 1,
	InvalidPtrInDelegate = 2,
	InvalidPtrInTimerDelegate = 3,
	LibraryInitFailed = 4,
	CollectionAccessFailed = 5,
};
#endif

class SDLlogHelper
{
public:
	SDLlogHelper(const SDLlogHelper&) = delete;
	SDLlogHelper(SDLlogHelper&&) = delete;
	SDLlogHelper() = default;
	~SDLlogHelper() = default;
	SDLlogHelper& operator==(const SDLlogHelper&) = delete;
	SDLlogHelper& operator==(SDLlogHelper&&) = delete;
	static void Print(PrefixErrorType ErrorType, std::string ClassName, const std::exception& TryCatchException = std::exception());
};
#endif
