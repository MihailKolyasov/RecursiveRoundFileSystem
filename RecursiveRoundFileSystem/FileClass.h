#pragma once
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
#include <fstream>
#include <iostream>
#include <string>
namespace fs = std::experimental::filesystem;

class File
{
public:
	File();
	File(std::string path);
	File(std::string path, std::string name, unsigned long long int size, std::string content);
	std::string GetName();
	std::string GetPath();
	std::string GetContent();
	unsigned long long int GetSize();
	~File();
private:
	std::string _path;
	std::string _name;
	unsigned long long int _size;
	std::string _content;
};
