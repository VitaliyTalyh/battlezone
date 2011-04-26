/*	This file is part of WIA Battlezone.
 *
 *	WIA Battlezone is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	 WIA Battlezone is distributed in the hope that it will be useful,
 *	but lWITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with WIA Battlezone.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 *	@file Player.cpp
 *	@headerfile "Player.h"
 *	@author Justin Barnard
 *	@date 4/21/2011
 *	@version 1.0
 */
#include "Player.h"

/**
 * Default Constructor 
 */
Player::Player()
{
	lives = 3;
	score = 0;
	tank = Tank();
	
};
bool Player::canFire()
{
	tank.canFire();
};
