#include "Base.h"
#include "Game.h"

namespace Arkanoid
{
	//-----------------------------------------------------------------------------------------------------------
	BaseSegment::BaseSegment(int index, bool edge)
	{
		if (edge)
		{
			assert(m_segmentTexture.loadFromFile(RESOURCES_PATH + "PlatformEdge.png"));
		}
		else
		{
			assert(m_segmentTexture.loadFromFile(RESOURCES_PATH + "PlatformMiddle.png"));
		}
		m_sprite.setTexture(m_segmentTexture);
		if (index < 0)
		{
			Math::GetInstance().SetSpriteSize(m_sprite, BASE_SEGMENT_SIZE, BASE_SEGMENT_SIZE);
		}
		else
		{
			Math::GetInstance().SetSpriteSize(m_sprite, BASE_SEGMENT_SIZE * -1, BASE_SEGMENT_SIZE);
		}
		Math::GetInstance().SetSpriteRelativeOrigin(m_sprite, 0.5f, 0.5f);
		m_position = { SCREEN_WIDTH * 0.5f + float(index) * BASE_SEGMENT_SIZE, SCREEN_HEIGHT - m_Y_POSITION_OFFSET };
		m_sprite.setPosition(m_position.x, m_position.y);
		m_index = index;
	}
	//-----------------------------------------------------------------------------------------------------------
	int BaseSegment::GetIndex()
	{
		return m_index;
	}
	//-----------------------------------------------------------------------------------------------------------
	void BaseSegment::SetPosition(float xPos)
	{
		m_position = { xPos + float(m_index) * BASE_SEGMENT_SIZE, m_position.y };
		m_sprite.setPosition(m_position.x, m_position.y);
	}
	//-----------------------------------------------------------------------------------------------------------
	Position2D BaseSegment::GetPosition()
	{
		return m_position;
	}
	//-----------------------------------------------------------------------------------------------------------
	sf::Sprite BaseSegment::GetSprite()
	{
		return m_sprite;
	}
	//-----------------------------------------------------------------------------------------------------------
	void BaseSegment::SetColor(sf::Color color)
	{
		m_sprite.setColor(color);
	}
	//-----------------------------------------------------------------------------------------------------------
	void BaseSegment::DrawSegment(sf::RenderWindow& window)
	{
		window.draw(m_sprite);
	}
	//-----------------------------------------------------------------------------------------------------------
	Base::Base()
	{
		BaseSegment* currentSegment;
		sf::Vector2f location;
		currentSegment = new BaseSegment(0, false);
		m_segments.push_back(currentSegment);
		currentSegment = new BaseSegment(1, true);
		m_segments.push_back(currentSegment);
		currentSegment = new BaseSegment(-1, true);
		m_segments.push_back(currentSegment);
	}
	//-----------------------------------------------------------------------------------------------------------
	Base::~Base()
	{
		for (BaseSegment* segment : m_segments)
		{
			delete segment;
		}
		m_segments.clear();
	}
	//-----------------------------------------------------------------------------------------------------------
	void Base::UpdateBase(float xPosition)
	{
		for (int i = 0; i < m_segments.size(); i++)
		{
			m_segments[i]->SetPosition(xPosition);
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void Base::ChangeBaseColor(sf::Color color)
	{
		if (m_segments[0]->GetSprite().getColor() != color)
		{
			for (int i = 0; i < m_segments.size(); i++)
			{
				m_segments[i]->GetSprite().setColor(color);
			}
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	Position2D Base::GetPosition()
	{
		return m_segments[0]->GetPosition();
	}
	//-----------------------------------------------------------------------------------------------------------
	Position2D Base::GetBaseSize()
	{
		return { BASE_SEGMENT_SIZE * m_segments.size(), BASE_SEGMENT_SIZE };
	}
	//-----------------------------------------------------------------------------------------------------------
	void Base::DrawBase(sf::RenderWindow& window)
	{
		for (BaseSegment* currentSegment : m_segments)
		{
			currentSegment->DrawSegment(window);
		}
	}
}
