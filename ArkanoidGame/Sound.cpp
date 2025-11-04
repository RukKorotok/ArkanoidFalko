#include "Sound.h"
#include "Game.h"

namespace Arkanoid
{
	//For next rev
	//------------------------------------------------------------------------------------------------------------
	void Audio::InitAudio()
	{

		assert(m_hitWave.loadFromFile(RESOURCES_PATH + "Hit.wav"));
		assert(m_deathWave.loadFromFile(RESOURCES_PATH + "Death.wav"));
		assert(m_introWave.loadFromFile(RESOURCES_PATH + "Intro.wav"));
		assert(m_pauseWave.loadFromFile(RESOURCES_PATH + "Pause.wav"));

		m_hit.first.setBuffer(m_hitWave);
		m_hit.second = Sound;
		m_pause.first.setBuffer(m_pauseWave);
		m_pause.second = Sound;
		m_death.first.setBuffer(m_deathWave);
		m_death.second = Sound;
		m_intro.first.setBuffer(m_introWave);
		m_intro.second = Music;
	}
	//------------------------------------------------------------------------------------------------------------
	void Audio::PlaySound(SoundFile sound)
	{
		std::pair<sf::Sound, SoundType> playedSound;

		switch (sound)
		{
		case HitSound:
		{
			playedSound = m_hit;
			break;
		}
		case DeathSound:
		{
			playedSound = m_death;
			break;
		}
		case IntroSound:
		{
			playedSound = m_intro;
			break;
		}
		case PauseSound:
		{
			playedSound = m_pause;
			break;
		}
		}

		switch (playedSound.second)
		{
		case Sound:
		{
			if (Game::GetInstance().GetSetings() & static_cast<uint32_t>(SettingsMode::SoundOn))
			{
				playedSound.first.play();
			}
		}
		case Music:
		{
			if (Game::GetInstance().GetSetings() & static_cast<uint32_t>(SettingsMode::MusicOn))
			{
				playedSound.first.play();
			}
		}
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void Audio::StopSound(SoundFile sound)
	{
		switch (sound)
		{
		case HitSound:
		{
			m_hit.first.stop();
			break;
		}
		case DeathSound:
		{
			m_death.first.stop();
			break;
		}
		case IntroSound:
		{
			m_intro.first.stop();
			break;
		}
		case PauseSound:
		{
			m_pause.first.stop();
			break;
		}
		}
	}
}
