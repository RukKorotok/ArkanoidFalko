#include "Block.h"
#include "Game.h"

namespace Arkanoid
{
	Block::Block(IndexEvents type, int index)
	{
		assert(m_frameClearTexture.loadFromFile(RESOURCES_PATH + "FrameClear.png") );
		assert(m_frameCrackTexture.loadFromFile(RESOURCES_PATH + "FrameCrack.png") );
		assert(m_frameBrokenTexture.loadFromFile(RESOURCES_PATH + "FrameBroken.png") );

		UI::GetInstance().InitSprite(m_frameSprite, m_frameClearTexture, 10, 10);
		UI::GetInstance().InitShape(m_blockShape, 10.0f, 10.0f, 1.0f, m_color, 50.0f, 50.0f);
		Math::GetInstance().SetSpriteSize(m_frameSprite, CELL_SIZE, CELL_SIZE);

		switch (type)
		{
		case Normal:
		{
			m_blockShape.setFillColor(sf::Color::White);
			m_frameSprite.setColor(sf::Color::White);
			break;
		}
		case ScoreUp:
		{
			m_blockShape.setFillColor(sf::Color::Magenta);
			m_frameSprite.setColor(sf::Color::Magenta);
			break;
		}
		case Poison:
		{
			m_blockShape.setFillColor(sf::Color::Green);
			m_frameSprite.setColor(sf::Color::Green);
			break;
		}
		case Disorient:
		{
			m_blockShape.setFillColor(sf::Color::Blue);
			m_frameSprite.setColor(sf::Color::Blue);
			break;
		}
		}

	}
	//-----------------------------------------------------------------------------------------------------------
	bool Block::HitBlock(Game& game, int appleIndex)
	{
		m_health--;
		if (m_health <=  0)
		{
			return true;
		}
		return false;
	}
	//-----------------------------------------------------------------------------------------------------------
	void Block::Draw(sf::RenderWindow& window)
	{
		window.draw(m_blockShape);
		window.draw(m_frameSprite);
	}
}
