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
	File()
		:_path(""), _size(0), _name(""), _content("")
	{
	}
	File(std::string path)
		:_path(path), _size(fs::file_size(path))
	{
		fs::path currentPath = path;
		currentPath = currentPath.filename();
		_name = currentPath.string();
		std::ifstream FILE(_path);
		if (FILE.is_open())
		{
			std::string line = "";
			while (!FILE.eof())
			{
				getline(FILE, line);
				_content += line;
			}
		}
		else
		{
			_content = "";
		}
	}
	File(std::string path,std::string name,unsigned long long int size,std::string content)
		:_path(path), _name(name), _content(content), _size(size)
	{
	}
	std::string GetName()
	{
		return _name;
	}

	std::string GetPath()
	{
		return _path;
	}

	std::string GetContent()
	{
		return _content;
	}

	unsigned long long int GetSize()
	{
		return _size;
	}

	~File()
	{
		_path = "";
		_name = "";
		_content = "";
		_size = 0;
	}
private:
	std::string _path;
	std::string _name;
	unsigned long long int _size;
	std::string _content;
};
