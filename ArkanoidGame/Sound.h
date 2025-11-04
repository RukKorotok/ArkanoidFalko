#pragma once
#include "SFML/Audio.hpp"
//For next rev
namespace Arkanoid
{
	class Game;

	enum SoundType
	{
		Music = 0,
		Sound
	};

	enum SoundFile
	{
		HitSound = 0,
		DeathSound,
		IntroSound,
		PauseSound
	};
	class Audio
	{
	public:

		static Audio& GetInstance()
		{
			static Audio audio;
			return audio;
		}

		void InitAudio();
		void PlaySound(SoundFile sound);
		void StopSound(SoundFile sound);

	private:

		Audio() {}
		~Audio() {}

		Audio(const Audio&) = delete;
		Audio& operator = (Audio const&) = delete;

		std::pair<sf::Sound, SoundType> m_hit;
		std::pair<sf::Sound, SoundType> m_death;
		std::pair<sf::Sound, SoundType> m_intro;
		std::pair<sf::Sound, SoundType> m_pause;

		sf::SoundBuffer m_hitWave;
		sf::SoundBuffer m_deathWave;
		sf::SoundBuffer m_introWave;
		sf::SoundBuffer m_pauseWave;
	};
}
