#ifndef INCLUDED_STRING_EXTENSION
#define INCLUDED_STRING_EXTENSION

#ifndef INCLUDED_STRINGSTREAM
#define INCLUDED_STRINGSTREAM
#include <sstream>
#endif

#ifndef INCLUDED_STRING
#define INCLUDED_STRING
#include <string>
#endif

#ifndef INCLUDED_COLLECTION_VECTOR
#define INCLUDED_COLLECTION_VECTOR
#include <vector>
#endif

class StringExtension
{
public:
	StringExtension(const StringExtension&) = delete;
	StringExtension(StringExtension&&) = delete;
	StringExtension() = default;
	~StringExtension() = default;
	StringExtension& operator=(const StringExtension&) = delete;
	StringExtension& operator=(StringExtension&&) = delete;
	static std::vector<std::string> Split(std::string input, const char& delim);
};

inline std::vector<std::string> StringExtension::Split(std::string input, const char& delim)
{
	std::stringstream ss(input);
	std::string line;
	std::vector<std::string> tokens;
	while (getline(ss, line))
	{
		size_t pos, prev = 0;
		while ((pos = line.find_first_of(delim, prev)) != std::string::npos)
		{
			tokens.push_back(line.substr(prev, pos - prev));
			prev = pos + 1;
		}
		if (prev < line.length())
		{
			tokens.push_back(line.substr(prev, line.find_last_of('.') - prev));
		}
	}
	return tokens;
}
#endif