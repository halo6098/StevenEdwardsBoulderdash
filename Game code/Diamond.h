#pragma once
#include "GridObject.h"

class Diamond : public GridObject
{

public:
	Diamond();
	bool AttemptFall(sf::Vector2i _direction, sf::Time _frametime);
	void Update(sf::Time _frameTime);

private:
	float m_timer;
	float m_timerMax;
};