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

#include "V2TileInfo.hpp"

bool V2TileInfo::load(QDataStream& stream) {
	for(int i = 0; i < TILES; ++i) {
        if(stream.atEnd()) return false;
		stream >> tileProperties[i].behavior;
	}
    quint8 b;
    for(int side = 0; side < 4; ++side) {
        for(int tile = 0; tile < TILES; ++tile) {
            if(stream.atEnd()) return false;
            stream >> b;
            tileProperties[tile].style[side] = (b & 0x03);
			tileProperties[tile].slopeEnabled[side] = b & 0x80;
			tileProperties[tile].b[side] = (b & 0x0C) >> 2;
			tileProperties[tile].m[side] = (b & 0x30) >> 4;
			tileProperties[tile].negativeSlope[side] = b & 0x40;
        }
    }
    return true;
}

void V2TileInfo::dump(QDataStream& stream) {
	for(int tile = 0; tile < TILES; ++tile) {
		stream << tileProperties[tile].behavior;
	}
    quint8 b = 0;
    for(int side = 0; side < 4; ++side) {
        for(int tile = 0; tile < TILES; ++tile) {
            b = 0;
			b = tileProperties[tile].style[side] & 0x03;
			if(tileProperties[tile].slopeEnabled[side]) {
				b |= 0x80;
			}
			b |= (tileProperties[tile].b[side] & 0x03) << 2;
			b |= (tileProperties[tile].m[side] & 0x03) << 4;
			if(tileProperties[tile].negativeSlope[side]) {
				b |= 0x40;
			}
            stream << b;
        }
    }
}

V2TileInfo::V2TileInfo() {
    for(int tile = 0; tile < TILES; ++tile) {
        tileProperties[tile].behavior = 0;
		for(int side = 0; side < 4; ++side) {
            tileProperties[tile].style[side] = 0;
			tileProperties[tile].slopeEnabled[side] = false;
			tileProperties[tile].b[side] = 0;
			tileProperties[tile].m[side] = 0;
			tileProperties[tile].negativeSlope[side] = false;
        }
    }
}
