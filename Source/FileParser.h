#pragma once

#include <string>
#include <stdexcept>

class FileParseError : public std::runtime_error
{
public:
	FileParseError(const std::string& fileName, const std::string& what = std::string())
		: std::runtime_error("Failed to parse file\n" + what), m_FileName(fileName) {}

	const std::string& GetFileName() const noexcept { return m_FileName; }

private:
	std::string m_FileName;
};

class FileParser
{
public:
	static std::string ParseToString(const std::string& filePath);
};