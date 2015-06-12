#include "framework/DataSaver.hpp"


namespace DataSaver
{
	bool readInt(std::istream& is, int& toRead)
	{
		int value = 0;
		while(true)
		{
			if(is >> value)
			{
				toRead = value;
				return true;
			}
			else if(is.fail())
			{
				is.clear();
				char ch;
				is >> ch;
				if(!(ch == separator) || !is)
				{
					return false;
				}
			}				
		}
	}

	int endecode(int n)
	{
		return n ^ key;
	}


	bool retrieveDatas (MetaData& metaData)
	{
		std::ifstream ist(filePath);
		
		if(!ist)
			return false;

		int value;
		if(!readInt(ist, value))
			return false;
		metaData.highScore = endecode(value);

		if(!readInt(ist, value))
			return false;
		metaData.inventory.addCoins(endecode(value));
		
		for(int i=1; i<PowerUpID::PowerUpCount; ++i)
		{
			if(!readInt(ist, value) ||
			   !metaData.inventory.setPowerUp(PowerUpID::ID(i), endecode(value)))
				return false;
		}
		return true;
	}

	bool saveDatas (const MetaData& metaData)
	{
		std::ofstream ost(filePath);
			
		if(!ost)
			return false;

		ost << endecode(metaData.highScore) << separator;
		ost << endecode(metaData.inventory.getCoins()) << separator;
		
		for(int i=1; i<PowerUpID::PowerUpCount; ++i)
		{
			ost << endecode(metaData.inventory.getPowerUp(PowerUpID::ID(i))) << separator;
		}

		return true;
	}		

	void makeDefaultFile()
	{
		MetaData metaData;
		metaData.highScore = 0;
		saveDatas(metaData);
	}
}
