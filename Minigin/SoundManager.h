#pragma once
#include <map>
class SoundStream;
class SoundEffect;

class SoundManager
{
public:
	enum class Action
	{
		Play, Pause, Stop, Resume
	};

	SoundManager() = default;
	~SoundManager();
	SoundManager(const SoundManager&) = delete;
	SoundManager(SoundManager&&) = delete;
	SoundManager& operator=(const SoundManager&) = delete;
	SoundManager& operator=(SoundManager&&) = delete;

	void AddSound(unsigned int id, const std::string& path, const std::string& soundName, bool isStream = true);

	//Play and stop functions
	void EditSound(const std::string& sound, Action action, int loops = 0, bool isStream = true) const;
	void EditSound(unsigned int id, Action action, int loops = 0, bool isStream = true) const;
	void SetSoundVolume(const std::string& sound, int volume, bool isStream = true);
	void SetAllVolume(int volume);
	void StopAllStreams() const;
	void StopAllEffects() const;

private:
	void AdjustSoundEffect(SoundEffect* soundEffect, Action action, int loops) const;
	void AdjustSoundStream(SoundStream* soundStream, Action action) const;

	const std::string m_Path{ "Resources/Sounds/" };
	std::vector<SoundStream*> m_SoundStreams;
	std::vector<SoundEffect*> m_SoundEffects;
	
	std::map<std::string, size_t> m_SoundEffectNames;
	std::map<std::string, size_t> m_SoundStreamNames;
	std::map<unsigned int, size_t> m_SoundEffectIds;
	std::map<unsigned int, size_t> m_SoundStreamIds;
};