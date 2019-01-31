#pragma once

// Project Includes
#include "../Framework/SpriteObject.h"

// Forward Declaration
// (only works with pointers/references)
class Level;

class GridObject : public SpriteObject
{

public:

	GridObject();

	void SetGridPosition(sf::Vector2i _newPosition);
	void SetGridPosition(int _x, int _y);

	void SetLevel(Level* _newLevel);

	sf::Vector2i GetGridPosition();
	bool GetCollectable();
	bool GetBlocksMovement();
	bool GetComplete();
protected:

	sf::Vector2i m_gridPosition;
	Level* m_level;
	bool m_collectable;
	bool m_blocksMovement;
	bool m_complete;
};