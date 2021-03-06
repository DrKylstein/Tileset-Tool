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

#include <QtGui>
#include "TileInfoModel.hpp"
#include "V2TileInfo.hpp"
#include "ClassicTileInfo.hpp"
bool TileInfoModel::load(QDataStream& stream) {
	V2TileInfo tileInfo;
	if(!tileInfo.load(stream)) {
		return false;
	}

	int m;
	unsigned int b;
	bool neg;

	for(int i = 0; i < _TILES; ++i) {
		//behavior
		_tiles[i].behavior = tileInfo.tileProperties[i].behavior;

		//slope
		if(tileInfo.tileProperties[i].slopeEnabled.top()) {
			_tiles[i].slopedSide = TOP_SLOPE;
			m = tileInfo.tileProperties[i].m.top();
			b = tileInfo.tileProperties[i].b.top();
			neg = tileInfo.tileProperties[i].negativeSlope.top();
		} else if(tileInfo.tileProperties[i].slopeEnabled.bottom()) {
			_tiles[i].slopedSide = BOTTOM_SLOPE;
			m = tileInfo.tileProperties[i].m.bottom();
			b = tileInfo.tileProperties[i].b.bottom();
			neg = tileInfo.tileProperties[i].negativeSlope.bottom();
		} else {
			_tiles[i].slopedSide = NO_SLOPE;
			m = 0;
			b = 0;
			neg = false;
		}
		_tiles[i].y0 = b << 2;
		_tiles[i].run = pow(2, m);
		if(neg) {
			_tiles[i].y0 += 4;
			_tiles[i].run *= -1;
		}

		//blocking and type
		for(int side = 0; side < 4; ++side) {
			if(tileInfo.tileProperties[i].style[side] != 0) {
				_tiles[i].blocking[side] = true;
				_tiles[i].style[side] = tileInfo.tileProperties[i].style[side] - 1;
			} else {
				_tiles[i].blocking[side] = false;
				_tiles[i].style[side] = 0;
			}
		}

	}
	emit dataChanged(createIndex(0, 0), createIndex(rowCount() - 1, columnCount() - 1));
    return true;
}

bool TileInfoModel::loadClassic(QDataStream& stream, int size) {
	ClassicTileInfo tileInfo;
	if(!tileInfo.load(stream, size)) return false;
	for(int i = 0; i < size; ++i) {
		//behavior
		_tiles[i].behavior = tileInfo.tiles[i].behavior;
		//blocking and type
		for(int side = 0; side < 4; ++side) {
			if(tileInfo.tiles[i].style[side] != 0) {
				_tiles[i].blocking[side] = true;
				_tiles[i].style[side] = tileInfo.tiles[i].style[side] - 1;
			} else {
				_tiles[i].blocking[side] = false;
				_tiles[i].style[side] = 0;
			}
		}
	}
	emit dataChanged(createIndex(0, 0), createIndex(size - 1, columnCount() - 1));
    return true;
}

static quint8 topBit(quint8 i) {
	if(i == 0) {
		return 0;
	}
	quint8 bitVal = 0x80, bitPos = 7;
	while((i & bitVal) == 0) {
		bitVal = bitVal >> 1;
		--bitPos;
	}
	return bitPos;
}

void TileInfoModel::dump(QDataStream& stream) {
	V2TileInfo tileInfo;

	int m;
	unsigned int b;
	bool neg;

	for(int i = 0; i < _TILES; ++i) {
		//behavior
		tileInfo.tileProperties[i].behavior = _tiles[i].behavior;

		//blocking and style
		for(int side = 0; side < 4; ++side) {
			if(_tiles[i].blocking[side]) {
				tileInfo.tileProperties[i].style[side] = 1 + _tiles[i].style[side];
			} else {
				tileInfo.tileProperties[i].style[side] = 0;
			}
		}

		//slope
		quint8 m, b;
		bool neg = _tiles[i].run < 0;
		m = topBit(abs(_tiles[i].run));
		if(neg) {
			b = (_tiles[i].y0 - 4) >> 2;
		} else {
			b = _tiles[i].y0 >> 2;
		}
		if(_tiles[i].slopedSide == TOP_SLOPE) {
			tileInfo.tileProperties[i].slopeEnabled.setTop(true);
			tileInfo.tileProperties[i].m.setTop(m);
			tileInfo.tileProperties[i].negativeSlope.setTop(neg);
			tileInfo.tileProperties[i].b.setTop(b);
			if(tileInfo.tileProperties[i].style.top() == 0) {
				tileInfo.tileProperties[i].style.setTop(1);
			}
		} else if(_tiles[i].slopedSide == BOTTOM_SLOPE) {
			tileInfo.tileProperties[i].slopeEnabled.setBottom(true);
			tileInfo.tileProperties[i].m.setBottom(m);
			tileInfo.tileProperties[i].negativeSlope.setBottom(neg);
			tileInfo.tileProperties[i].b.setBottom(b);
			if(tileInfo.tileProperties[i].style.bottom() == 0) {
				tileInfo.tileProperties[i].style.setBottom(1);
			}
			if(neg) {
				tileInfo.tileProperties[i].slopeEnabled.setLeft(true);
				tileInfo.tileProperties[i].m.setLeft(m);
				tileInfo.tileProperties[i].negativeSlope.setLeft(neg);
				tileInfo.tileProperties[i].b.setLeft(b);
				if(tileInfo.tileProperties[i].style.left() == 0) {
					tileInfo.tileProperties[i].style.setLeft(1);
				}
			} else {
				tileInfo.tileProperties[i].slopeEnabled.setRight(true);
				tileInfo.tileProperties[i].m.setRight(m);
				tileInfo.tileProperties[i].negativeSlope.setRight(neg);
				tileInfo.tileProperties[i].b.setRight(b);
				if(tileInfo.tileProperties[i].style.right() == 0) {
					tileInfo.tileProperties[i].style.setRight(1);
				}
			}
		}

	}
	tileInfo.dump(stream);
}

void TileInfoModel::blank() {
    for(int i=0; i<rowCount();++i) {
        _tiles[i].behavior = 0;
        _tiles[i].slopedSide = NO_SLOPE;
        _tiles[i].run = 1;
        _tiles[i].y0 = 0;
        for(int j = 0; j < 4; ++j) {
			_tiles[i].blocking[j] = false;
			_tiles[i].style[j] = 0;
        }
    }
}

int TileInfoModel::rowCount(const QModelIndex & parent) const {
	return _TILES;
}
int TileInfoModel::columnCount(const QModelIndex & parent) const {
	return FIELD_COUNT;
}
QVariant TileInfoModel::headerData (int section, Qt::Orientation orientation, int role) const {
	if(role == Qt::DisplayRole) {
		if(orientation == Qt::Horizontal) {
			return section;
		} else {
			return QString(tr("Tile %1")).arg(section);
		}
	}
	return QVariant();
}
QVariant TileInfoModel::data (const QModelIndex & index, int role) const {
	if( (role == Qt::DisplayRole || role == Qt::EditRole) && (index.isValid())) {
		switch(index.column()) {
			case FIELD_BEHAVIOR:
				if(_tiles[index.row()].behavior == REAL_FOREGROUND) {
					return FAKE_FOREGROUND;
				} else if(_tiles[index.row()].behavior == REAL_MASKED) {
					return FAKE_MASKED;
				} else {
					return _tiles[index.row()].behavior;
				}
			case FIELD_TOP_BLOCKING:
			case FIELD_RIGHT_BLOCKING:
			case FIELD_BOTTOM_BLOCKING:
			case FIELD_LEFT_BLOCKING:
				return _tiles[index.row()].blocking[index.column()-FIELD_TOP_BLOCKING];
			case FIELD_TOP_STYLE:
			case FIELD_RIGHT_STYLE:
			case FIELD_BOTTOM_STYLE:
			case FIELD_LEFT_STYLE:
				return _tiles[index.row()].style[index.column()-FIELD_TOP_STYLE];
			case FIELD_SLOPED_SIDE:
				return _tiles[index.row()].slopedSide;
			case FIELD_Y0:
				return _tiles[index.row()].y0;
			case FIELD_RUN:
				return _tiles[index.row()].run;
			default:
				return QVariant();
		}
	}
	return QVariant();
}
Qt::ItemFlags TileInfoModel::flags (const QModelIndex & index) const {
	if(!index.isValid()) return Qt::ItemIsEnabled;
	return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}
bool TileInfoModel::setData (const QModelIndex & index, const QVariant & value, int role) {
	if( (role == Qt::DisplayRole || role == Qt::EditRole) && (index.isValid())) {
		switch(index.column()) {
			case FIELD_BEHAVIOR:
				if(value.toInt() >= 0 && value.toInt() < 255) {
					if(value.toInt() == FAKE_FOREGROUND) {
						if(_tiles[index.row()].behavior == REAL_FOREGROUND) {
							return true;
						}
						_tiles[index.row()].behavior = REAL_FOREGROUND;
					} else if(value.toInt() == FAKE_MASKED) {
						if(_tiles[index.row()].behavior == REAL_MASKED) {
							return true;
						}
						_tiles[index.row()].behavior = REAL_MASKED;
					} else {
						if(_tiles[index.row()].behavior == value.toInt()) {
							return true;
						}
						_tiles[index.row()].behavior = value.toInt();
					}
					emit dataChanged(index, index);
					return true;
				}
				break;
			case FIELD_TOP_BLOCKING:
			case FIELD_RIGHT_BLOCKING:
			case FIELD_BOTTOM_BLOCKING:
			case FIELD_LEFT_BLOCKING:
				if(_tiles[index.row()].blocking[index.column()-FIELD_TOP_BLOCKING] == value.toBool()) {
					return true;
				}
				_tiles[index.row()].blocking[index.column()-FIELD_TOP_BLOCKING] = value.toBool();
				emit dataChanged(index, index);
				return true;
			case FIELD_TOP_STYLE:
			case FIELD_RIGHT_STYLE:
			case FIELD_BOTTOM_STYLE:
			case FIELD_LEFT_STYLE:
				if(value.toInt() >= 0 && value.toInt() < 4) {
					if(_tiles[index.row()].style[index.column()-FIELD_TOP_STYLE] == value.toInt()) {
						return true;
					}
					_tiles[index.row()].style[index.column()-FIELD_TOP_STYLE] = value.toInt();
					emit dataChanged(index, index);
					return true;
				}
				break;
			case FIELD_SLOPED_SIDE:
				if(value.toInt() >= 0 && value.toInt() < 3) {
					if(_tiles[index.row()].slopedSide == slope_side(value.toInt())) {
						return true;
					}
					_tiles[index.row()].slopedSide = slope_side(value.toInt());
				}
				break;
			case FIELD_Y0:
				if(value.toInt() >= 0 && value.toInt() <= 16) {
					if(value.toInt() >= 16) {
						if(_tiles[index.row()].y0 == 16) {
							return true;
						}
						_tiles[index.row()].y0 = 16;
					} else {
						if(_tiles[index.row()].y0 == value.toInt() & 0xC0) {
							return true;
						}
						_tiles[index.row()].y0 = value.toInt() & 0xC0;
					}
					emit dataChanged(index, index);
					return true;
				}
				break;
			case FIELD_RUN:
				switch(value.toInt()) {
					case 1:
					case 2:
					case 4:
					case 8:
					case -1:
					case -2:
					case -4:
					case -8:
						if(_tiles[index.row()].run == value.toInt()) {
							return true;
						}
						_tiles[index.row()].run = value.toInt();
						emit dataChanged(index, index);
						return true;
					default:
					break;
				}
				break;
			default:
				break;
		}
	}
	return false;
}
