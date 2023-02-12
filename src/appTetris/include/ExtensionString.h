#ifndef EXTSTRING_H
#define EXTSTRING_H

#ifndef STRINGSTREAM_H
#define STRINGSTREAM_H
#include <sstream>
#endif

#ifndef STRING_H
#define STRING_H
#include <string>
#endif

#ifndef VECTOR_H
#define VECTOR_H
#include <vector>
#endif

class ExtensionString
{
public:
	ExtensionString(const ExtensionString&) = delete;
	ExtensionString(ExtensionString&&) = delete;
	ExtensionString() = default;
	~ExtensionString() = default;
	ExtensionString& operator=(const ExtensionString&) = delete;
	ExtensionString& operator=(ExtensionString&&) = delete;
	static std::vector<std::string> Split(std::string input, const char& delim);
};

inline std::vector<std::string> ExtensionString::Split(std::string input, const char& delim)
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