#pragma once
// Example of usage of the template would be for an Audio service
// T = Audio
// DEFAULT = NullAudio, an audio class that does nothing and inherits from Audio.
// All services to be using a Service Locator should have a DEFAULT option!
template<class T, class DEFAULT>
class ServiceLocator
{
public:
	static void Init() 
	{
		m_Service = &m_DefaultService;
	}
	static T& GetService() {  return *m_Service; }
	static void RegisterAudioService(T* service)
	{
		m_Service = (service == nullptr) ? &m_DefaultService : service;
	}

private:
	static T* m_Service;
	static DEFAULT m_DefaultService;
};


template<class T, class DEFAULT>
T* ServiceLocator<T, DEFAULT>::m_Service = nullptr;
template<class T, class DEFAULT>
DEFAULT ServiceLocator<T, DEFAULT>::m_DefaultService{};