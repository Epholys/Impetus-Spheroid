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

	// Not really necessary, but I'm sured of the result with that
	bool toBool(int n)
	{
		return n != 0;
	}

	int toInt(bool b)
	{
		return b ? 1 : 0;
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
		metaData.firstTimeExecuted = toBool(endecode(value));
		
		if(!readInt(ist, value))
			return false;
		metaData.highScore = endecode(value);

		if(!readInt(ist, value))
			return false;
		metaData.inventory.addCoins(endecode(value));
		
		for(int i=0; i<PowerUpID::PowerUpCount; ++i)
		{
			if(!readInt(ist, value) ||
			   !metaData.inventory.setPowerUp(PowerUpID::ID(i), endecode(value)))
				return false;
		}
		for(int i=0; i<PowerUpID::PowerUpCount; ++i)
		{
			if(!readInt(ist, value))
				return false;
			metaData.isPowerUpUnlocked[i] = toBool(endecode(value));
		}

		for(int i=0; i<ImprovementID::ImprovementCount; ++i)
		{
			if(!readInt(ist, value))
				return false;
			metaData.improvementValue[i] = endecode(value);
		}
			    
		return true;
	}

	bool saveDatas (const MetaData& metaData)
	{
		std::ofstream ost(filePath);
			
		if(!ost)
			return false;

		ost << endecode(toInt(metaData.firstTimeExecuted)) << separator;
		ost << endecode(metaData.highScore) << separator;
		ost << endecode(metaData.inventory.getCoins()) << separator;
		
		for(int i=0; i<PowerUpID::PowerUpCount; ++i)
		{
			ost << endecode(metaData.inventory.getPowerUp(PowerUpID::ID(i))) << separator;
		}
		for(int i=0; i<PowerUpID::PowerUpCount; ++i)
		{
			ost << endecode(toInt(metaData.isPowerUpUnlocked[i])) << separator;
		}
		for(int i=0; i<ImprovementID::ImprovementCount; ++i)
		{
			ost << endecode(metaData.improvementValue[i]) << separator;
		}

		return true;
	}		

	void salvageDatas()
	{
		MetaData metaData = makeDefaultFile();
		retrieveDatas(metaData);
		saveDatas(metaData);
	}

	namespace
	{
		auto improvementDatas = genImprovementData();
	}
	
	MetaData makeDefaultFile()
	{
		using namespace PowerUpID;
		using namespace ImprovementID;
		MetaData metaData;

		metaData.firstTimeExecuted = true;
		metaData.highScore = 0;
		
		metaData.isPowerUpUnlocked = std::vector<bool>(PowerUpCount, true);
		std::vector<PowerUpID::ID> locked {BallDuplication, CancelEvents, AddTime, AddTarget, PointMultiplier, BallTouchDouble, AddCannon};
		for(auto id : locked)
		{
			metaData.isPowerUpUnlocked[id] = false;
		}

		metaData.improvementValue = std::vector<int>(ImprovementCount, 1);
		for(int i=0; i<ImprovementCount; ++i)
		{
			metaData.improvementValue.at(i) = improvementDatas.at(i).baseValue;
		}

		return metaData;
	}
}
