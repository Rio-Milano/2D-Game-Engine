#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include<unordered_map>
#include<string>

class SoundManager
{
public:

	static SoundManager& GetInstance();
	void LoadSound(const std::string& soundName, const std::string& soundDir);
	const std::string& GetSoundDir(const std::string& soundName);
	void PlaySound(const std::string& soundName);

private:
	SoundManager() = default;
	~SoundManager() = default;

	std::unordered_map<std::string, std::string> m_SoundNameToSoundDirMap;
	
};


#endif