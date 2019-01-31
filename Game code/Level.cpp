// Project Includes
#include "Level.h"
#include "../Framework/AssetManager.h"
#include "Wall.h"
#include "Player.h"
#include "Dirt.h"
#include "Diamond.h"
#include "Boulder.h"
#include "Exit.h"
// Library Includes
#include <iostream>
#include <fstream>

Level::Level()
	: m_cellSize(64.0f)
	, m_currentLevel(0)
	, m_pendingLevel(0)
	, m_background()
	, m_contents()
	, m_pendingReset(false)
	, m_Complete(false)
{
	LoadLevel(1);
}


void Level::Draw(sf::RenderTarget& _target)
{
	// Create and update
	sf::View camera = _target.getDefaultView();

	// Adjust camera as needed
	camera.setCenter( m_background[0].size() * m_cellSize / 2 , 
		m_background.size() * m_cellSize / 2);

	// Draw game world to the window
	_target.setView(camera);
	for (int y = 0; y < m_background.size(); ++y)
	{
		for (int x = 0; x < m_background[y].size(); ++x)
		{
			_target.draw(m_background[y][x]);
		}
	}

	// rows
	for (int y = 0; y < m_contents.size(); ++y)
	{
		// cells
		for (int x = 0; x < m_contents[y].size(); ++x)
		{
			// sticky outies (grid objects)
			for (int z = 0; z < m_contents[y][x].size(); ++z)
			{
				m_contents[y][x][z]->Draw(_target);
			}
		}
	}

	// Reset view
	_target.setView(_target.getDefaultView());
}


void Level::Update(sf::Time _frameTime)
{
	// rows
	for (int y = 0; y < m_contents.size(); ++y)
	{
		// cells
		for (int x = 0; x < m_contents[y].size(); ++x)
		{
			// sticky outies (grid objects)
			for (int z = 0; z < m_contents[y][x].size(); ++z)
			{
				m_contents[y][x][z]->Update(_frameTime);
			}
		}
	}

	// IF there is a pending level waiting...
	if (m_pendingLevel != 0)
	{
		// Load it
		LoadLevel(m_pendingLevel);
		// remove pending level
		m_pendingLevel = 0;
	}
}

void Level::Input(sf::Event _gameEvent)
{
	// rows
	for (int y = 0; y < m_contents.size(); ++y)
	{
		// cells
		for (int x = 0; x < m_contents[y].size(); ++x)
		{
			// sticky outies (grid objects)
			for (int z = 0; z < m_contents[y][x].size(); ++z)
			{
				m_contents[y][x][z]->Input(_gameEvent);
			}
		}
	}

	if (m_pendingReset == true)
	{
		m_pendingReset = false;
		ReloadLevel();
	}

	if (m_readyToLoad == true)
	{
		m_readyToLoad = false;
		LoadNextLevel();
	}

}

void Level::SetReset()
{
	m_pendingReset = true;
}

void Level::SetReadyToLoad()
{
	m_readyToLoad = true;
}

void Level::LoadLevel(int _levelToLoad)
{
	// Clean up the old level

	// Delete any data already in the level
	// rows
	for (int y = 0; y < m_contents.size(); ++y)
	{
		// cells
		for (int x = 0; x < m_contents[y].size(); ++x)
		{
			// sticky outies (grid objects)
			for (int z = 0; z < m_contents[y][x].size(); ++z)
			{
				delete m_contents[y][x][z];
			}
		}
	}

	// Clear out our lists
	m_background.clear();
	m_contents.clear();


	// Set the current level
	m_currentLevel = _levelToLoad;


	// Set up the new level

	// Open our file for reading
	std::ifstream inFile;
	std::string fileName = "levels/Level" + std::to_string(m_currentLevel) + ".txt";
	inFile.open(fileName);

	// Make sure the file was opened
	if (!inFile) {
		std::cerr << "Unable to open file " + fileName;
		exit(1); // Call system to stop program with error
	}

	// Set the starting x and y coordinates used to position level objects
	int x = 0;
	int y = 0;

	// Create the first row in our grid
	m_background.push_back(std::vector<sf::Sprite>());
	m_contents.push_back(std::vector<std::vector<GridObject*> >());
	   
	// Read each character one by one from the file...
	char ch;
	// Each time, try to read the next character
	// If successful, execute body of loop
	// the "noskipws" means our input from the file will include 
	// the white space (spaces and new lines)
	while (inFile >> std::noskipws >> ch)
	{
		// Perform actions based on what character was read in

		if (ch == ' ')
		{
			++x;
		}
		else if (ch == '\n')
		{
			++y;
			x = 0;

			// Create a new row in our grid
			m_background.push_back(std::vector<sf::Sprite>());
			m_contents.push_back(std::vector<std::vector<GridObject*> >());
		}
		else
		{
			// This is going to be some object (or empty space), so we need a background
			// Create Background Sprite
			m_background[y].push_back(sf::Sprite(AssetManager::GetTexture("graphics/ground.png")));
			m_background[y][x].setPosition(x*m_cellSize, y*m_cellSize);

			// Create an empty vector for our grid contents in this cell
			m_contents[y].push_back(std::vector<GridObject*>());

			if (ch == '-')
			{
				Dirt* dirt = new Dirt();
				dirt->SetLevel(this);
				dirt->SetGridPosition(x, y);
				m_contents[y][x].push_back(dirt);
			}
			else if (ch == 'W')
			{
				Wall* wall = new Wall();
				wall->SetLevel(this);
				wall->SetGridPosition(x, y);
				m_contents[y][x].push_back(wall);
			}
			else if (ch == 'P')
			{
				Player* player = new Player();
				player->SetLevel(this);
				player->SetGridPosition(x, y);
				m_contents[y][x].push_back(player);
			}
			else if (ch == 'D')
			{
				Diamond* diamond = new Diamond();
				diamond->SetLevel(this);
				diamond->SetGridPosition(x, y);
				m_contents[y][x].push_back(diamond);
			}
			else if (ch == 'B')
			{
				Boulder* boulder = new Boulder();
				boulder->SetLevel(this);
				boulder->SetGridPosition(x, y);
				m_contents[y][x].push_back(boulder);
			}
			else if (ch == 'E')
			{
				Exit* exit = new Exit();
				exit->SetLevel(this);
				exit->SetGridPosition(x, y);
				m_contents[y][x].push_back(exit);
			}
			else
			{
				std::cerr << "Unrecognised character in level file: " << ch;
			}
		} 
	}

	// Close the file now that we are done with it
	inFile.close();

}

void Level::ReloadLevel()
{
	LoadLevel(m_currentLevel);
}

void Level::LoadNextLevel()
{
	LoadLevel(m_currentLevel + 1);
}

bool Level::CheckDiamonds()
{
	// rows
	for (int y = 0; y < m_contents.size(); ++y)
	{
		// collums
		for (int x = 0; x < m_contents[y].size(); ++x)
		{
			// z axis stuff (grid objects)
			for (int z = 0; z < m_contents[y][x].size(); ++z)
			{
				GridObject* thisObject = m_contents[y][x][z];

				Diamond* diamondObject = dynamic_cast<Diamond*>(thisObject);
				if (diamondObject != nullptr)
				{
					// return false if any diamonds on the map

					return false;
				}

					
			}
		}
	}
	return true;
}

float Level::GetCellSize()
{
	return m_cellSize;
}

bool Level::MoveObjectTo(GridObject* _toMove, sf::Vector2i _targetPos)
{
	// Don't trust other code
	// Make sure _toMove is a valid pointer
	// Also check our target position is within the grid
	if (   _toMove != nullptr
		&& _targetPos.y >= 0 && _targetPos.y < m_contents.size()
		&& _targetPos.x >= 0 && _targetPos.x < m_contents[_targetPos.y].size())
	{
		// Get the current position of the grid object
		sf::Vector2i oldPos = _toMove->GetGridPosition();

		// Find the object in the list using an iterator 
		// and the find method
		auto it = std::find(m_contents[oldPos.y][oldPos.x].begin(),
							m_contents[oldPos.y][oldPos.x].end(),
							_toMove);

		// If we found the object at this location,
		// it will NOT equal the end of the vector
		if (it != m_contents[oldPos.y][oldPos.x].end())
		{
			// We found the object!

			// Add it to its new position
			m_contents[_targetPos.y][_targetPos.x].push_back(_toMove);
			// Remove it from the old position
			m_contents[oldPos.y][oldPos.x].erase(it);



			// Tell the object it's new position
			_toMove->SetGridPosition(_targetPos);

			// Return success
			return true;
		}
	}

	// return failure
	return false;
}


bool Level::AttemptDelete(GridObject* _toMove)
{
	// don't trust other code. make sure _toMove is a valid pointer
	if (_toMove != nullptr)
	{
		// Check if it is a box via dynamic cast

		// get the current position of the grid object
		sf::Vector2i oldPos = _toMove->GetGridPosition();

		// Find the object in the list using an iterator
		//and find method
		auto it = std::find(m_contents[oldPos.y][oldPos.x].begin(),
			m_contents[oldPos.y][oldPos.x].end(),
			_toMove);
		// if we found the object at this location it will not
		// equal the the end of the vector
		if (it != m_contents[oldPos.y][oldPos.x].end())
		{
			// we found the object
			delete *it;
			m_contents[oldPos.y][oldPos.x].erase(it);

			return true;
		}
	}

	return false;
}

std::vector< GridObject* > Level::GetObjectAt(sf::Vector2i _targetPos)
{
	// Don't trust the data!
	// Make sure the coordinates are within the vector size
	if (   _targetPos.y >= 0 && _targetPos.y < m_contents.size()
		&& _targetPos.x >= 0 && _targetPos.x < m_contents[_targetPos.y].size())
	{
		// Get the contents
		// return them
		return m_contents[_targetPos.y][_targetPos.x];
	}

	// Default return
	// Return an empty vector with nothing in it
	// (default constructor)
	return std::vector<GridObject*>();
}