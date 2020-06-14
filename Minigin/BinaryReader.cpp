#include "MiniginPCH.h"
#include "BinaryReader.h"
#include <type_traits>
#include <iostream>

void BinaryReader::ReadString(std::string& string)
{
	if (file.is_open())
	{
		int size{};
		Read<int>(size);
		string.resize(size);
		file.read(&string[0], size);
		return;
	}
	std::cout << "Failed to read from file," << std::endl << "file not opened." << std::endl << std::endl;
}

bool BinaryReader::OpenFile(const std::string& fileName)
{
	file.open(fileName, std::ios::in | std::ios::binary);
	return file.is_open(); // let user know if opening was succesful or not
}
bool BinaryReader::CloseFile()
{
	file.close();
	return !file.is_open();// let user know if the closing was succesful or not
}