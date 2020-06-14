#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

class BinaryReader
{
public:
	BinaryReader() = default;
	~BinaryReader() = default;

	template <class myType>
	void Read(myType& myPod)
	{
		if (!std::is_pod<myType>())
		{
			std::cout << "Failed to read from file," << std::endl << "type was not pod" << std::endl << std::endl;
			return;
		}
		if (std::is_pointer<myType>())
		{
			std::cout << "Failed to read from file," << std::endl << "type was pointer" << std::endl << std::endl;
			return;
		}
		if (file.is_open())
		{
			file.read((char*)& myPod, sizeof(myType));
			return;
		}
		std::cout << "Failed to read from file," << std::endl << "file not opened." << std::endl << std::endl;
		return;
	}
	void ReadString(std::string& string);
	bool OpenFile(const std::string& fileName);
	bool CloseFile();

private:
	std::ifstream file;
};


