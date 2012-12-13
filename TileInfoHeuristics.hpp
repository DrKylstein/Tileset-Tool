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

#ifndef TILEINFOHEURISTICS_HPP
#define TILEINFOHEURISTICS_HPP

namespace TileInfoHeuristics {
	int tliLength(QFile& file) {
		return file.size() / (6 * 2);
	}
	int analyzeExe(QDataStream& stream) { //moves stream to start of animation data, returns tile count
		stream.setByteOrder(QDataStream::LittleEndian);
		quint16 mz;
		stream >> mz;
		if(mz != 0x5A4D){
			qDebug() << "No Mz.";
			return 0;
		}
		quint16 lastBlockLength, blockCount, relocationCount, headerParagraphs;
		stream >> lastBlockLength >> blockCount >> relocationCount >> headerParagraphs;
		unsigned int exeOffset = headerParagraphs * 16;
		unsigned int exeSize = ( ( (blockCount - 1) * 512) + lastBlockLength) - exeOffset;
		unsigned int tileInfoOffset;
		int tileCount = 0;
		switch(exeSize) {
			case 99762: // ep 1 v1.1
				tileInfoOffset = 0x131F8;
				tileCount = 611;
				break;
			case 99972: // ep 1 v1.31
				tileInfoOffset = 0x130F8;
				tileCount = 611;
				break;
			case 118114: // ep 2 v1.1
				tileInfoOffset = 0x17938;
				tileCount = 689;
				break;
			case 118160: // ep2 v1.31
				tileInfoOffset = 0x17828;
				tileCount = 689;
				break;
			case 127086: // ep3 v1.1
				tileInfoOffset = 0x199F8;
				tileCount = 715;
				break;
			case 127104: // ep3 v1.31
				tileInfoOffset = 0x198C8;
				tileCount = 715;
				break;
			default:
				qDebug() << "Unrecognized signature.";
				return 0;
		};
		stream.skipRawData(exeOffset + tileInfoOffset - 0xA);
		return tileCount;
	}
}
#endif
