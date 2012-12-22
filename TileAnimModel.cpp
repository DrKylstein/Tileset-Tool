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

#include "TileAnimModel.hpp"
#include "TileModel.hpp"
#include "TileInfo.h"
#include <QtGui>

int TileAnimModel::rowCount(const QModelIndex & parent) const {
	return _MAX_TILES;
}
int TileAnimModel::columnCount(const QModelIndex & parent) const {
	return _MAX_FRAMES;
}
QVariant TileAnimModel::headerData (int section, Qt::Orientation orientation, int role) const {
	if(role == Qt::DisplayRole) {
		if(orientation == Qt::Horizontal) {
			return section;
		} else {
			return QString(tr("Tile %1")).arg(section);
		}
	}
	return QVariant();
}
QVariant TileAnimModel::data (const QModelIndex & index, int role) const {
	if( (role == Qt::DisplayRole || role == Qt::EditRole) && (index.isValid())) {
		return _frames[index.column()][index.row()];
	}
	return QVariant();
}
Qt::ItemFlags TileAnimModel::flags (const QModelIndex & index) const {
	if(!index.isValid()) return Qt::ItemIsEnabled;
	return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}
bool TileAnimModel::setData (const QModelIndex & index, const QVariant & value, int role) {
	if(role != Qt::EditRole) return false;
	if(!index.isValid()) return false;
	if(_frames[index.column()][index.row()] == value.toInt()) {
		return true;
	}
    _frames[index.column()][index.row()] = value.toInt();
    emit dataChanged(index, index);
	return true;
}
bool TileAnimModel::load(QDataStream& stream) {
    quint16 w;
	for(unsigned int frame=0;frame!=_MAX_FRAMES;++frame) {
		for(unsigned int tile=0;tile!=_MAX_TILES;tile++) {
            if(stream.atEnd()) return false;
			stream >> w;
			_frames[frame][tile] = w >> 5;
		}
	}
	emit dataChanged(createIndex(0, 0), createIndex(rowCount() - 1, columnCount() - 1));
    return true;
}
bool TileAnimModel::loadClassic(QDataStream& stream, int size) {
    quint16 w;
    int framesLeft = 1;
	for(unsigned int tile=0;tile!=size;tile++) {
		if(stream.atEnd()) return false;
		stream >> w;
		if(w < 2) {
			framesLeft = 1;
		} else if(framesLeft == 1) {
			framesLeft = w;
		} else {
			--framesLeft;
		}
		for(unsigned int frame = 0; frame != _MAX_FRAMES; ++frame) {
			_frames[frame][tile] = tile - (w - framesLeft) + ((frame - framesLeft) % w);
		}
	}
	emit dataChanged(createIndex(0, 0), createIndex(size - 1, columnCount() - 1));
    return true;
}

void TileAnimModel::dump(QDataStream& stream) {
    quint16 w;
	for(unsigned int frame=0;frame!=_MAX_FRAMES;++frame) {
		for(unsigned int tile=0;tile!=_MAX_TILES;tile++) {
			w = _frames[frame][tile] << 5;
			stream << w;
		}
	}
}

void TileAnimModel::blank() {
	for(unsigned int frame=0;frame!=_MAX_FRAMES;++frame) {
		for(unsigned int tile=0;tile!=_MAX_TILES;tile++) {
			_frames[frame][tile] = 0;
		}
	}
}

void TileAnimModel::markAllNew() {
	emit dataChanged(createIndex(0, 0), createIndex(rowCount() - 1, columnCount() - 1));
}
