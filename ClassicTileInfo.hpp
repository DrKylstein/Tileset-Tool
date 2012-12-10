/*
 *  Tileset Tool, an editor for Commander Keen: Invasion of the Vorticons
 *  Tilesets.
 *  Copyright (C) 2010-2012  Kyle Delaney
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

#ifndef CLASSICTILEINFO_HPP
#define CLASSICTILEINFO_HPP

#include <QDataStream>
#include "FourWay.hpp"

class ClassicTileInfo {
	public:
        bool load(QDataStream&, int size);
        void dump(QDataStream&);
        void blank(int size);
        int tileCount(void);

        static int findTileInfo(QDataStream&);

        struct Tile {
            FourWay<quint16> style;
            qint16 behavior;
            quint16 frames;
        };
        Tile *tiles;
		int _tileCount;
};
#endif
