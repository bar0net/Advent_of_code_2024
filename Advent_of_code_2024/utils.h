#pragma once

#include <fstream>
#include <iostream>

static size_t LoadFileToBuffer(const char* path, char** buffer)
{
	std::ifstream file(path, std::ios::binary | std::ios::ate);
	if (!file.is_open())
	{
		std::cerr << "Error: Could not open '" << path << "'\n";
		return 0;
	}

	long long size = file.tellg();
	if (size <= 0)
	{
		std::cerr << "Error: The file is empty.\n";
		return 0;
	}

	*buffer = new char[static_cast<size_t>(size) + 1];

	file.seekg(0, std::ios::beg);
	if (!file.read(*buffer, size))
	{
		std::cerr << "Error: Could not read the file.\n";
		delete[] * buffer;
		*buffer = nullptr;
		file.close();
		return 0;
	}

	(*buffer)[size] = '\0';
	file.close();
	return static_cast<size_t>(size) + 1;
}

static void ReleaseBuffer(char** buffer)
{
	if (*buffer != nullptr)
	{
		delete[] * buffer;
		*buffer = nullptr;
	}
}