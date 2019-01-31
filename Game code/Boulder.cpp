#include "Boulder.h"
#include "../Framework/AssetManager.h"
#include "Level.h"
#include "player.h"
#include "Diamond.h"


Boulder::Boulder()
	: GridObject()
	, m_timer(0.0f)
	, m_timerMax(1.0f)
{
	m_sprite.setTexture(AssetManager::GetTexture("graphics/Sand.png"));
	m_blocksMovement = true;
	m_collectable = false;
}

bool Boulder::AttemptFall(sf::Vector2i _direction, sf::Time _frametime)
{
	sf::Vector2i targetPos = m_gridPosition + _direction;

	std::vector<GridObject*> targetCellContents = m_level->GetObjectAt(targetPos);

	GridObject* blocker = nullptr;

	bool blocked = false;
	for (int i = 0; i < targetCellContents.size(); ++i)
	{
		blocker = targetCellContents[i];
		if (targetCellContents[i]->GetCollectable() == true || targetCellContents[i]->GetBlocksMovement() == true)
		{
			blocked = true;

		}

	}

	// if there is no space below, move down
	if (blocked == false)
	{
		if (m_timer >= m_timerMax)
		{
			m_level->MoveObjectTo(this, targetPos);
			m_timerMax = m_timer + 1.0f;

			Player* player = dynamic_cast<Player*>(blocker);
			if (player != nullptr)
			{
				m_level->ReloadLevel();
			}
		}
		else
		{
			m_timer += _frametime.asSeconds();
		}
	}
	else
	{
		if (_direction.x == 0)
		{
			Diamond* diamond = dynamic_cast<Diamond*>(blocker);
			Boulder* boulder = dynamic_cast<Boulder*>(blocker);
			if (diamond != nullptr || boulder != nullptr)
			{


				bool slideRightSucceed = AttemptFall((sf::Vector2i(1, 1)), _frametime);

				if (!slideRightSucceed)
				{
					AttemptFall((sf::Vector2i(-1, 1)), _frametime);
					return false;
				}
				else
				{
					return true;

				}
			}

		}
	}
	return false;
}


bool Boulder::AttemptPush(sf::Vector2i _direction)
{
	sf::Vector2i targetPos = m_gridPosition + _direction;

	std::vector<GridObject*> targetCellContents = m_level->GetObjectAt(targetPos);

	//Check if Objects block the movement of boulder
	bool blocked = false;
	for (int i = 0; i < targetCellContents.size(); ++i)
	{
		if (targetCellContents[i]->GetCollectable() == true || targetCellContents[i]->GetBlocksMovement() == true)
		{
			blocked = true;
		}
	}


	if (blocked == false)
	{
		m_level->MoveObjectTo(this, targetPos);
		return true;
	}


	return false;
}

void Boulder::Update(sf::Time _frametime)
{
	AttemptFall(sf::Vector2i(0, 1), _frametime);
}