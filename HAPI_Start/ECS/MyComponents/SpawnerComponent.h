#ifndef SPAWNERCOMPONENT_H
#define SPAWNERCOMPONENT_H

#include<string>
#include"../ECS/Base/BaseComponent.h"

struct SpawnerComponent final : public BaseComponent
{
	std::string m_entityTagName_ofEntityToBeSpawned{ "" };
	
	unsigned int m_spawnAmount{ 0 };
	
	float
		m_spawnFrequency{ 0 },
		m_spawnFrequencyCount{ 0 };

	bool
		m_isSpawnerActive{false};

	void InitializeSpawnerComponent(const std::string& spawnEntityTagName, const unsigned int& spawnAmount, const float& spawnFrequency)
	{
		m_entityTagName_ofEntityToBeSpawned = spawnEntityTagName;
		m_spawnAmount = spawnAmount;
		m_spawnFrequency = spawnFrequency;
	};

};

#endif