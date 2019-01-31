#pragma once
//Project includes
#include "GridObject.h"

class Exit : public GridObject
{
public:
	Exit();
	void Update(sf::Time _frameTime);
	void Complete();

private:
};