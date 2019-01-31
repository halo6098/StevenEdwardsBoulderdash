// Project Includes
#include "Diamond.h"
#include "../Framework/AssetManager.h"

Diamond::Diamond()
	: GridObject()
{
	m_sprite.setTexture(AssetManager::GetTexture("graphics/Diamond.png"));
	m_blocksMovement = false;
	m_collectable = true;
}