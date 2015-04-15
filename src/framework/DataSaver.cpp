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


	bool retrieveDatas (TransGamesData& datas)
	{
		std::ifstream ist(filePath);
		
		if(!ist)
			return false;

		int value;
		if(!readInt(ist, value))
			return false;
		datas.highScore = endecode(value);

		if(!readInt(ist, value))
			return false;
		datas.inventory.addCoins(endecode(value));
		
		for(int i=1; i<PowerUpID::PowerUpCount; ++i)
		{
			if(!readInt(ist, value) ||
			   !datas.inventory.setPowerUp(PowerUpID::ID(i), endecode(value)))
				return false;
		}
		return true;
	}

	bool saveDatas (const TransGamesData& datas)
	{
		std::ofstream ost(filePath);
			
		if(!ost)
			return false;

		ost << endecode(datas.highScore) << separator;
		ost << endecode(datas.inventory.getCoins()) << separator;
		
		for(int i=1; i<PowerUpID::PowerUpCount; ++i)
		{
			ost << endecode(datas.inventory.getPowerUp(PowerUpID::ID(i))) << separator;
		}

		return true;
	}		

	void makeDefaultFile()
	{
		TransGamesData datas;
		datas.highScore = 0;
		saveDatas(datas);
	}
}
