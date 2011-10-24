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

TileAnimModel::TileAnimModel(TileModel* parent): QAbstractTableModel(parent) {
	_parent = parent;
}
int TileAnimModel::rowCount(const QModelIndex & parent) const {
	return _parent->tiles.size();
}
int TileAnimModel::columnCount(const QModelIndex & parent) const {
	return _parent->numFrames;
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
		return _parent->tiles[index.row()][index.column()+TileInfo::FIELD_FRAME1];
	}
	/*if(role == Qt::DecorationRole && index.row() < rowCount() && !_parent->_graphics.isNull()) {
		QPixmap icon(16, 16);
		QPainter painter(&icon);
		if((_parent->tiles[index.row()][index.column()+TileInfo::FIELD_FRAME1] + index.row() ) * 16 >= _parent->_graphics.height()) {
			return QVariant();
		}
		painter.drawImage(QRect(0, 0, 16, 16), _parent->_graphics, QRect(0, (_parent->tiles[index.row()][index.column()+TileInfo::FIELD_FRAME1] + index.row() ) * 16, 16, 16));
		return icon;
	}*/
	return QVariant();
}
Qt::ItemFlags TileAnimModel::flags (const QModelIndex & index) const {
	if(!index.isValid()) return Qt::ItemIsEnabled;
	return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}
/*bool TileInfoModel::insertRows (int row, int count, const QModelIndex & parent) {
	if(count < 0) return false;
	beginInsertRows(parent, row, row + count - 1);
	_parent->tiles.insert(row, count, blankTile);
	endInsertRows();
	return true;
}
bool TileInfoModel::removeRows (int row, int count, const QModelIndex & parent) {
	if(count < 0) return false;
	beginRemoveRows(parent, row, row + count - 1);
	_parent->tiles.remove(row, count);
	endRemoveRows();
	return true;
}*/
bool TileAnimModel::setData (const QModelIndex & index, const QVariant & value, int role) {
	if(role != Qt::EditRole) return false;
	if(!index.isValid()) return false;
	if(value.toInt() != _parent->tiles[index.row()][index.column()+TileInfo::FIELD_FRAME1]) {
		_parent->tiles[index.row()][index.column()+TileInfo::FIELD_FRAME1] = value.toInt();
		emit dataChanged(index, index);
	}
	return true;
}
void TileAnimModel::markAllNew(void) {
	emit dataChanged(createIndex(0, 0), createIndex(rowCount() - 1, columnCount() - 1));
}
void TileAnimModel::changeFrameCount(int i) {
	if(i == 4) {
		beginRemoveColumns(QModelIndex(), 4, 7);
		endRemoveColumns();
	}
	if(i == 8) {
		beginInsertColumns(QModelIndex(), 4, 7);
		endInsertColumns();
	}
}
