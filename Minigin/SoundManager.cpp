#include "MiniginPCH.h"
#include "SoundManager.h"
#include "SoundStream.h"
#include "SoundEffect.h"

SoundManager::~SoundManager()
{
	for (int i{}; i < m_SoundStreams.size(); i++)
	{
		delete m_SoundStreams[i];
	}
	for (int i{}; i < m_SoundEffects.size(); i++)
	{
		delete m_SoundEffects[i];
	}
	m_SoundStreams.clear();
	m_SoundEffects.clear();
}

void SoundManager::EditSound(const std::string& sound, Action action, int loops, bool isStream) const
{
	if (isStream)
	{
		auto it = m_SoundStreamNames.find(sound);
		if (it != m_SoundStreamNames.cend())
		{
			AdjustSoundStream(m_SoundStreams[it->second], action);
		}
		return;
	}
	auto it = m_SoundEffectNames.find(sound);
	if (it != m_SoundEffectNames.cend())
	{
		AdjustSoundEffect(m_SoundEffects[it->second], action, loops);
	}
}

void SoundManager::EditSound(unsigned int id, Action action, int loops, bool isStream) const
{
	if (isStream)
	{
		auto it = m_SoundStreamIds.find(id);
		if (it != m_SoundStreamIds.cend())
		{
			AdjustSoundStream(m_SoundStreams[it->second], action);
		}
		return;
	}
	auto it = m_SoundEffectIds.find(id);
	if (it != m_SoundEffectIds.cend())
	{
		AdjustSoundEffect(m_SoundEffects[it->second], action, loops);
	}
}

void SoundManager::SetSoundVolume(const std::string& sound, int volume, bool isStream)
{
	if (isStream)
	{
		SoundStream::SetVolume(volume);
		return;
	}
	auto it = m_SoundEffectNames.find(sound);
	if (it != m_SoundEffectNames.cend())
	{
		m_SoundEffects[it->second]->SetVolume(volume);
	}
}

void SoundManager::SetAllVolume(int volume)
{
	SoundStream::SetVolume(volume);
	for (auto* effect : m_SoundEffects)
		effect->SetVolume(volume);
}

void SoundManager::AdjustSoundStream(SoundStream* pSoundStream, Action action) const
{
	SoundEffect sound{""};
	if (pSoundStream->IsLoaded())
	{
		switch (action)
		{
		case Action::Play:
			if (!pSoundStream->IsPlaying())
			{
				pSoundStream->Play(true);
			}
			break;
		case Action::Stop:
			if (pSoundStream->IsPlaying())
			{
				pSoundStream->Stop();
			}
			break;
		case Action::Pause:
			if (pSoundStream->IsPlaying())
			{
				pSoundStream->Pause();
			}
			break;
		case Action::Resume:
			if (pSoundStream->IsPlaying())
			{
				pSoundStream->Resume();
			}
			break;
		}
	}
}

void SoundManager::StopAllStreams() const
{
	for (auto* soundStream : m_SoundStreams)
		if (soundStream->IsPlaying())
			soundStream->Stop();
}

void SoundManager::AdjustSoundEffect(SoundEffect* pSoundEffect, Action action, int loops) const
{
	if (pSoundEffect->IsLoaded())
	{
		switch (action)
		{
		case Action::Play:
			pSoundEffect->Play(loops);
			break;
		}
	}
}

void SoundManager::StopAllEffects() const
{
	SoundEffect::StopAll();
}

void SoundManager::AddSound(unsigned int id, const std::string& path, const std::string& soundName, bool isStream)
{
	if (isStream)
	{
		if (m_SoundStreamNames.find(soundName) == m_SoundStreamNames.cend()
			&& m_SoundStreamIds.find(id) == m_SoundStreamIds.cend())
		{
			auto soundStream = new SoundStream(path);
			m_SoundStreams.push_back(soundStream);
			m_SoundStreamNames.emplace(soundName, m_SoundStreams.size()-1);
			m_SoundStreamIds.emplace(id, m_SoundStreams.size() - 1);
		}
		return;
	}
	if (m_SoundEffectNames.find(soundName) == m_SoundEffectNames.cend()
		&& m_SoundEffectIds.find(id) == m_SoundEffectIds.cend())
	{
		m_SoundEffects.push_back(new SoundEffect(path));
		m_SoundEffectNames.emplace(soundName, m_SoundEffects.size() - 1);
		m_SoundEffectIds.emplace(id, m_SoundEffects.size() - 1);
	}
}
