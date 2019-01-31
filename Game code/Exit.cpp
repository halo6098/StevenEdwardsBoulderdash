//project includes
#include "Exit.h"
#include "../Framework/AssetManager.h"
#include "Level.h"

Exit::Exit()
	: GridObject()
{
	m_sprite.setTexture(AssetManager::GetTexture("graphics/DoorClosed.png"));
	m_blocksMovement = false;
	m_collectable = false;
	m_complete = false;
}

void Exit::Complete()
{
	m_complete = m_level->CheckDiamonds();
}

void Exit::Update(sf::Time _frameTime)
{
	Complete();

	if (m_complete == false)
	{
		m_sprite.setTexture(AssetManager::GetTexture("graphics/DoorClosed.png"));
	}
	if (m_complete == true)
	{
		m_sprite.setTexture(AssetManager::GetTexture("graphics/DoorOpen.png"));
	}
}
