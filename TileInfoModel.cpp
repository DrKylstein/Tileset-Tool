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

#include "TileInfoModel.hpp"
#include "TileModel.hpp"
#include <QtGui>

bool TileInfoModel::load(QDataStream& stream) {
	for(unsigned int i = 0; i != rowCount(); ++i) {
        if(stream.atEnd()) return false;
        qint8 b;
		stream >> b;
        if(b == -1) {
            b = 29;
        }
        if(b == -2) {
            b = 30;
        }
        _tileProperties[i].behavior = b;
	}
    quint8 b;
    for(unsigned int side=0; side!=4; ++side) {
        for(unsigned int tile=0; tile!=rowCount(); ++tile) {
            if(stream.atEnd()) return false;
            stream >> b;
            _tileProperties[tile].solid[side] = b & 0x03;
            _tileProperties[tile].surfaceType[side] = (b & 0x03) - 1;
            if(b & 0x80) {
                _tileProperties[tile].sloped[side] = 1;
                _tileProperties[tile].offset[side] = b & 0x0C;
                _tileProperties[tile].steepness[side] = (b & 0x30) >> 4;
                if(b & 0x40) {
                    _tileProperties[tile].steepness[side] *= -1;
                    _tileProperties[tile].offset[side] += 4;
                }
            } else {
                _tileProperties[tile].sloped[side] = false;
                _tileProperties[tile].offset[side] = 0;
                _tileProperties[tile].steepness[side] = 0;
            }
        }
    }
	emit dataChanged(createIndex(0, 0), createIndex(rowCount() - 1, columnCount() - 1));
    return true;
}

void TileInfoModel::dump(QDataStream& stream) {
	for(int tile=0;tile<TileInfo::MAX_TILES;++tile) {
        qint8 b = _tileProperties[tile].behavior;
        if(b == 29) {
            b = -1;
        }
        if(b == 30) {
            b = -2;
        }
		stream << b;
	}
    quint8 b = 0;
    for(unsigned int side=0; side!=4; ++side) {
        for(int tile=0;tile<rowCount();++tile) {
            b = 0;
            if(_tileProperties[tile].solid[side]) {
                b = (_tileProperties[tile].surfaceType[side] + 1);
                if(_tileProperties[tile].sloped[side]) {
                    b |= 0x80 | abs(_tileProperties[tile].steepness[side]) << 4;
                    if(_tileProperties[tile].steepness[side] < 0) {
                        b |= 0x40 | (_tileProperties[tile].offset[side] - 4);
                    } else {
                        b |= _tileProperties[tile].offset[side];
                    }
                }
            }
            stream << b;
        }
    }
}

void TileInfoModel::blank() {
    for(int i=0; i<rowCount();++i) {
        _tileProperties[i] = TileProperties();
    }
}

int TileInfoModel::rowCount(const QModelIndex & parent) const {
	return _MAX_TILES;
}
int TileInfoModel::columnCount(const QModelIndex & parent) const {
	return TileInfo::FIELD_COUNT - 8;
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
        if(index.column() == FIELD_BEHAVIOR) {
            return _tileProperties[index.row()].behavior;
        } else if(index.column() >= FIELD_SOLID && index.column() < FIELD_SLOPED) {
            return _tileProperties[index.row()].solid[index.column()-FIELD_SOLID];
        } else if(index.column() >= FIELD_SLOPED && index.column() < FIELD_OFFSET) {
            return _tileProperties[index.row()].sloped[index.column()-FIELD_SLOPED];
        } else if(index.column() >= FIELD_OFFSET && index.column() < FIELD_STEEPNESS) {
            return _tileProperties[index.row()].steepness[index.column()-FIELD_OFFSET];
        } else if(index.column() >= FIELD_STEEPNESS && index.column() < FIELD_SURFACE_TYPE) {
            return _tileProperties[index.row()].steepness[index.column()-FIELD_STEEPNESS];
        } else {
            return _tileProperties[index.row()].surfaceType[index.column()-FIELD_SURFACE_TYPE];
        }
	}
	return QVariant();
}
Qt::ItemFlags TileInfoModel::flags (const QModelIndex & index) const {
	if(!index.isValid()) return Qt::ItemIsEnabled;
	return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}
bool TileInfoModel::setData (const QModelIndex & index, const QVariant & value, int role) {
	if(role != Qt::EditRole) return false;
	if(!index.isValid()) return false;
        if(index.column() == FIELD_BEHAVIOR) {
            _tileProperties[index.row()].behavior = value.toInt();
        } else if(index.column() >= FIELD_SOLID && index.column() < FIELD_SLOPED) {
            _tileProperties[index.row()].solid[index.column()-FIELD_SOLID] = value.toBool();
        } else if(index.column() >= FIELD_SLOPED && index.column() < FIELD_OFFSET) {
            _tileProperties[index.row()].sloped[index.column()-FIELD_SLOPED] = value.toBool();
        } else if(index.column() >= FIELD_OFFSET && index.column() < FIELD_STEEPNESS) {
            _tileProperties[index.row()].steepness[index.column()-FIELD_OFFSET] = value.toInt();
        } else if(index.column() >= FIELD_STEEPNESS && index.column() < FIELD_SURFACE_TYPE) {
            _tileProperties[index.row()].steepness[index.column()-FIELD_STEEPNESS] = value.toInt();
        } else {
            _tileProperties[index.row()].surfaceType[index.column()-FIELD_SURFACE_TYPE] = value.toInt();
        }
    emit dataChanged(index, index);
	return true;
}