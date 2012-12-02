/*
 *  Tileset Tool, an editor for Commander Keen: Invasion of the Vorticons
 *  Tilesets.
 *  Copyright (C) 2010-2011  Kyle Delaney
 *
 *  This file is a part of Tileset Tool.
 *
 *  Tileset Tool is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Tileset Tool is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  You may contact the author at <dr.kylstein@gmail.com>
 */

#ifndef TILEINFO_H
#define TILEINFO_H
namespace TileInfo {
enum {
	FIELD_TOP,				FIELD_RIGHT,		FIELD_BOTTOM,			FIELD_LEFT,
	FIELD_SLOPE_ENABLED, 	FIELD_SLOPE,		FIELD_SLOPE_OFFSET,		FIELD_SLOPE_SIDE,
	FIELD_BEHAVIOR,			FIELD_SURFACE_TYPE,	FIELD_BOTTOM_BEHAVIOR,
	FIELD_FRAME1,			FIELD_FRAME2,		FIELD_FRAME3,			FIELD_FRAME4,
	FIELD_FRAME5,			FIELD_FRAME6,		FIELD_FRAME7,			FIELD_FRAME8
};
const int MAX_FRAMES = 8;
const int MAX_TILES = 910;
const int FIELD_COUNT = 19;
enum { SLOPED_TOP, SLOPED_BOTTOM };
//enum { NO_ANIMATION, FORWARD_ANIMATION, BACKWARD_ANIMATION, OSCILLATING_ANIMATION, CUSTOM_ANIMATION };
enum {
	BEHAVE_NOTHING,
	BEHAVE_KILLS,
	BEHAVE_DOOR_ONE,
	BEHAVE_DOOR_TWO,
	BEHAVE_DOOR_THREE,
	BEHAVE_DOOR_FOUR,
	BEHAVE_FIVE_HUNDRED,
	BEHAVE_ONE_HUNDRED,
	BEHAVE_TWO_HUNDRED,
	BEHAVE_ONE_THOUSAND,
	BEHAVE_FIVE_THOUSAND,
	BEHAVE_JOYSTICK,
	BEHAVE_BATTERY,
	BEHAVE_VACUUM,
	BEHAVE_WHISKEY,
	BEHAVE_RAYGUN,
	BEHAVE_POGO,
	BEHAVE_EXIT,
	BEHAVE_KEY_ONE,
	BEHAVE_KEY_TWO,
	BEHAVE_KEY_THREE,
	BEHAVE_KEY_FOUR,
	BEHAVE_MESSAGE,
	BEHAVE_LIGHTSWITCH,
	BEHAVE_TELEPORT,
	BEHAVE_SWITCH_ON,
	BEHAVE_SWITCH_OFF,
	BEHAVE_ANKH,
	BEHAVE_AMMO,
	BEHAVE_FOREGROUND,
	BEHAVE_MASKED
};
}
// seperate top and bottom slopes? top bottom right and left types? (for future expansion and use in custom format)
#endif
