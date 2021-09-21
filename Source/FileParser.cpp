#include "FileParser.h"

#include <fstream>
#include <memory>
#include <iostream>

std::string FileParser::ParseToString(const std::string& filePath)
{
	std::ifstream ifile;
	ifile.exceptions(std::ios::badbit);

	try
	{
		ifile.open(filePath);

		ifile.seekg(0, std::ios::end);
		std::size_t fileSize = ifile.tellg();
		ifile.seekg(0, std::ios::beg);

		std::string fileStr;
		fileStr.resize(fileSize);

		ifile.read(fileStr.data(), fileStr.size());

		return fileStr;
	}
	catch (const std::ios::failure& e)
	{
		throw FileParseError(filePath, e.what());
	}
}
