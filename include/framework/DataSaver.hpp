#ifndef DATA_SAVER_HPP
#define DATA_SAVER_HPP


#include <iostream>
#include <fstream>
#include <string>

#include "core/MetaData.hpp"

// Quick functions to save and load MetaData on HDD/SSD.
namespace DataSaver
{
	// Key for the XOR encoding, just not to be too obvious in the data file.
	const int key = 42;
	
	const std::string filePath = "./DATA.db";
	const char separator = ',';

	bool retrieveDatas(MetaData& metaData);
	bool saveDatas(const MetaData& metaData);
	void salvageDatas();
	MetaData makeDefaultFile();

	bool readInt(std::istream& is, int& toRead);
	bool toBool(int n);
	int toInt(bool b);
	int endecode(int n);
   
}


#endif
