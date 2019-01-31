#pragma once

// Project Includes
#include "GridObject.h"

class Boulder : public GridObject
{

public:
	Boulder();
	bool AttemptFall(sf::Vector2i _direction, sf::Time _frametime);
	bool AttemptPush(sf::Vector2i _direction);
	void Update(sf::Time _frameTime);

private:
	float m_timer;
	float m_timerMax;
};