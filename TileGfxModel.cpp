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

#include "TileGfxModel.hpp"
#include "TileModel.hpp"
#include <QtGui>

/*bool TileGfxModel::load(QDataStream& stream, int length) {
	delete _image;
	_image = new QImage(16, gfxLength >> 1, QImage::Format_Indexed8);
	stream.setByteOrder(QDataStream::BigEndian);
	for(unsigned int p = 0; p != 4; ++p) {
		for(unsigned int y = 0; y != length >> 1; ++y) {
			stream >> w;
			for(unsigned int x = 0; x != 16; ++x) {
				_image->setPixel(x, y, sheetImage.pixelIndex(x, y) | ( (w & (1 << (15 - x) ) ) >> (15 - x) ) << p);
			}
		}
	}

}

void TileGfxModel::dump(QDataStream& stream) {

}*/

TileGfxModel::TileGfxModel(TileModel* parent): QAbstractListModel(parent) {
	_parent = parent;
	//_image = new QImage();
}
int TileGfxModel::rowCount(const QModelIndex & parent) const {
	return _parent->_graphics.height() / 16;
}
QVariant TileGfxModel::headerData (int section, Qt::Orientation orientation, int role) const {
	if(role == Qt::DisplayRole) {
		if(orientation == Qt::Horizontal) {
			return section;
		} else {
			return QString(tr("Tile %1")).arg(section);
		}
	}
	return QVariant();
}
QVariant TileGfxModel::data (const QModelIndex & index, int role) const {
	if(index.isValid()) {
		QPixmap icon(16, 16);
		QPainter painter(&icon);
		if(!_parent->_graphics.isNull()) {
			painter.drawImage(QRect(0, 0, 16, 16), _parent->_graphics, QRect(0, index.row() * 16, 16, 16));
		}
		return icon;

	}
	return QVariant();
}
Qt::ItemFlags TileGfxModel::flags (const QModelIndex & index) const {
	if(!index.isValid()) return Qt::ItemIsEnabled;
	return QAbstractListModel::flags(index);
}
void TileGfxModel::markAllNew(void) {
	beginRemoveRows(QModelIndex(), 0, rowCount());
	endRemoveRows();
	beginInsertRows(QModelIndex(), 0, rowCount());
	endInsertRows();
	emit dataChanged(createIndex(0, 0), createIndex(rowCount() - 1, 0));
}

