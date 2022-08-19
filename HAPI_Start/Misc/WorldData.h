#ifndef WORLDDATA_H
#define WORLDDATA_H

class WorldData
{
public:
	static WorldData& GetInstance()
	{
		static WorldData instance;
		return instance;
	}

	float DamageMultiplyer{ 1.0f };



private:
	WorldData() = default;
	~WorldData() = default;
};



#endif