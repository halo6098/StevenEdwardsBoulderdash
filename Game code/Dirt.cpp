// Project Includes
#include "Dirt.h"
#include "../Framework/AssetManager.h"

Dirt::Dirt()
	: GridObject()
{
	m_sprite.setTexture(AssetManager::GetTexture("graphics/Dirt.png"));
	m_blocksMovement = false;
	m_collectable = true;
}