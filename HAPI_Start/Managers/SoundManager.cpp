#include "SoundManager.h"

#include<HAPI_lib.h>
using namespace HAPISPACE;

SoundManager& SoundManager::GetInstance()
{
	static SoundManager instance;
	return instance;
}

void SoundManager::LoadSound(const std::string& soundName, const std::string& soundDir)
{
	HAPI.LoadSound(soundDir);
	this->m_SoundNameToSoundDirMap[soundName] = soundDir;
}

const std::string& SoundManager::GetSoundDir(const std::string& soundName)
{
	static std::string empty{ "" };

	if (this->m_SoundNameToSoundDirMap.find(soundName) != this->m_SoundNameToSoundDirMap.end())
		return this->m_SoundNameToSoundDirMap[soundName];
	else
		return empty;
}

void SoundManager::PlaySound(const std::string& soundName)
{
	if (this->m_SoundNameToSoundDirMap.find(soundName) != this->m_SoundNameToSoundDirMap.end())
	{
		HAPI.PlaySound(this->m_SoundNameToSoundDirMap[soundName]);
	}
}
