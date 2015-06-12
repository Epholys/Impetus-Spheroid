#ifndef DATA_SAVER_HPP
#define DATA_SAVER_HPP


#include <iostream>
#include <fstream>
#include <string>

#include "core/MetaData.hpp"
#include "core/Identifiers.hpp"

// Quick functions to save important datas to have a working prototype.
namespace DataSaver
{
	// Key for the XOR encoding, just not to be too obvious in the data file.
	const int key = 42;
	
	const std::string filePath = "./DATA.db";
	const char separator = ',';

	bool retrieveDatas(MetaData& metaData);
	bool saveDatas(const MetaData& metaData);
	void makeDefaultFile();

	bool readInt(std::istream& is, int& toRead);
	int endecode(int n);
   
}


#endif
