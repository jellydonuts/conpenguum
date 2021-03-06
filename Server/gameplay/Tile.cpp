#include "general.h"
#include "Tile.h"

using namespace std;
/*-----------------------------------------------------------------------------
--  FUNCTION:   Tile::add
--
--  DATE:       January 27, 2010
--
--  REVISIONS:  v0.1 - pinch of  code, mostly comments.
--
--  DESIGNER:   Gameplay/Physics Team
--
--  PROGREMMER: Gameplay/Physics Team
--
--  INTERFACE:  Tile(Ship *ship)
--              Ship *ship : ship to add to the tile
--
--  NOTES:      Adds a ship to the tile.
--
--  RETURNS:    void.
--
------------------------------------------------------------------------------*/
void Tile::add(Ship *ship)
{
	for (list<Ship*>::iterator it = ships.begin(); it != ships.end(); ++it)
	{
		if ((*it)->id == ship->id)
			return;
	}
    ships.push_back(ship);
}

/*-----------------------------------------------------------------------------
--  FUNCTION:   Tile::add
--
--  DATE:       January 27, 2010
--
--  REVISIONS:  v0.1 - pinch of  code, mostly comments.
--
--  DESIGNER:   Gameplay/Physics Team
--
--  PROGREMMER: Gameplay/Physics Team
--
--  INTERFACE:  Tile(Shot *shot)
--              Shot *shot : ship to add to the tile
--
--  NOTES:      Adds a shot to the tile.
--
--  RETURNS:    void.
--
------------------------------------------------------------------------------*/
void Tile::add(Shot *shot)
{
	for (list<Shot*>::iterator it = shots.begin(); it != shots.end(); ++it)
	{
		if ((*it)->id == shot->id)
			return;
	}
    shots.push_back(shot);
}

/*-----------------------------------------------------------------------------
--  FUNCTION:   Tile::remove
--
--  DATE:       January 27, 2010
--
--  REVISIONS:  v0.1 - pinch of  code, mostly comments.
--
--  DESIGNER:   Gameplay/Physics Team
--
--  PROGREMMER: Gameplay/Physics Team
--
--  INTERFACE:  Tile(Ship *ship)
--              Ship *ship : ship to remove from the tile
--
--  NOTES:      Removes a ship from the tile.
--
--  RETURNS:    void.
--
------------------------------------------------------------------------------*/

void Tile::remove(Ship *ship)
{
	for (list<Ship*>::iterator it = ships.begin(); it != ships.end(); ++it)
	{
		if ((*it)->id == ship->id)
		{
			ships.erase(it);
			return;
		}
	}
}

/*-----------------------------------------------------------------------------
--  FUNCTION:   Tile::remove
--
--  DATE:       January 27, 2010
--
--  REVISIONS:  v0.1 - pinch of  code, mostly comments.
--
--  DESIGNER:   Gameplay/Physics Team
--
--  PROGREMMER: Gameplay/Physics Team
--
--  INTERFACE:  Tile(Shot *shot)
--              Shot *shot : shot to remove from the tile
--
--  NOTES:      Removes a shot from the tile.
--
--  RETURNS:    void.
--
------------------------------------------------------------------------------*/
void Tile::remove(Shot *shot)
{
	for (list<Shot*>::iterator it = shots.begin(); it != shots.end(); ++it)
	{
		if ((*it)->id == shot->id)
		{
			shots.erase(it);
			return;
		}
	}
}

/*-----------------------------------------------------------------------------
--  FUNCTION:   Tile::numShips
--
--  DATE:       January 27, 2010
--
--  REVISIONS:  v0.1 - pinch of  code, mostly comments.
--
--  DESIGNER:   Gameplay/Physics Team
--
--  PROGREMMER: Gameplay/Physics Team
--
--  INTERFACE:  numShips()
--
--  NOTES:      Counts the number of ships in the tile.
--
--  RETURNS:    The number of ships in the tile.
--
------------------------------------------------------------------------------*/
int Tile::numShips()
{
    return ships.size();
}

/*-----------------------------------------------------------------------------
--  FUNCTION:   Tile::numShots
--
--  DATE:       January 27, 2010
--
--  REVISIONS:  v0.1 - pinch of  code, mostly comments.
--
--  DESIGNER:   Gameplay/Physics Team
--
--  PROGREMMER: Gameplay/Physics Team
--
--  INTERFACE:  numShots()
--
--  NOTES:      Counts the number of shots in the tile.
--
--  RETURNS:    The number of shots in the tile.
--
------------------------------------------------------------------------------*/
int Tile::numShots() {
    return shots.size();
}

/*-----------------------------------------------------------------------------
--  FUNCTION:   Tile::empty
--
--  DATE:       January 27, 2010
--
--  REVISIONS:  v0.1 - pinch of  code, mostly comments.
--
--  DESIGNER:   Gameplay/Physics Team
--
--  PROGREMMER: Gameplay/Physics Team
--
--  INTERFACE:  int empty()
--
--  NOTES:      Checks if the tile is empty (and not a wall).
--
--  RETURNS:    true if the tile is empty and not a wall, else false.
--
------------------------------------------------------------------------------*/
bool Tile::empty()
{
    return (!isWall() && numShots() == 0 && numShips() == 0);
}

/*-----------------------------------------------------------------------------
--  FUNCTION:   Tile::getShips
--
--  DATE:       January 27, 2010
--
--  REVISIONS:  v0.1 - pinch of  code, mostly comments.
--
--  DESIGNER:   Gameplay/Physics Team
--
--  PROGREMMER: Gameplay/Physics Team
--
--  INTERFACE:  std::list<Ship*> Tile::getShips()
--
--  NOTES:      Gets all the ship objects.
--
--  RETURNS:    std::list of Ship pointers.
--
------------------------------------------------------------------------------*/
std::list<Ship*> Tile::getShips()
{
    return ships;
}

/*-----------------------------------------------------------------------------
--  FUNCTION:   Tile::getShots
--
--  DATE:       January 27, 2010
--
--  REVISIONS:  v0.1 - pinch of  code, mostly comments.
--
--  DESIGNER:   Gameplay/Physics Team
--
--  PROGREMMER: Gameplay/Physics Team
--
--  INTERFACE:  std::list<Shot*> Tile::getShots()
--
--  NOTES:      Gets all the shot objects.
--
--  RETURNS:    std::list of Shot pointers.
--
------------------------------------------------------------------------------*/
std::list<Shot*> Tile::getShots()
{
    return shots;
}
