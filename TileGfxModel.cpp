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

bool TileGfxModel::load(const QImage& source, const QVector<QRgb>& colors) {
	if(source.width() % 16) return false;
	if(source.height() % 16) return false;
	int length = (source.width() / 16) * (source.height() / 16);
	QImage converted = QImage(16, length * 16, QImage::Format_RGB32);
	converted.fill(0);
	QPainter painter(&converted);
	int x = 0, y = 0;
	for(int i = 0; i < length; ++i) {
		painter.drawImage(QRect(0, i * 16, 16, 16), source, QRect(x, y, 16, 16));
		x += 16;
		if(x >= source.width()) {
			x = 0;
			y += 16;
		}
	}
	painter.end();
	_graphics = converted.convertToFormat(QImage::Format_Indexed8, colors);
	beginRemoveRows(QModelIndex(), 0, rowCount());
	endRemoveRows();
	beginInsertRows(QModelIndex(), 0, rowCount());
	endInsertRows();
	emit dataChanged(createIndex(0, 0), createIndex(rowCount() - 1, 0));
	return true;
}

bool TileGfxModel::load(QDataStream& stream, int gfxLength, const QVector<QRgb>& colors, int skip) {
	QImage newGfx = QImage(16, gfxLength >> 1, QImage::Format_Indexed8);
	newGfx.setColorTable(colors);
	newGfx.fill(0);
	stream.setByteOrder(QDataStream::BigEndian);
	quint16 w = 0;
	for(unsigned int p = 0; p < 4; ++p) {
		for(unsigned int y = 0; y < gfxLength >> 1; ++y) {
			stream >> w;
			for(unsigned int x = 0; x < 16; ++x) {
				newGfx.setPixel(x, y, newGfx.pixelIndex(x, y) | ( (w & (1 << (15 - x) ) ) >> (15 - x) ) << p);
			}
		}
		stream.skipRawData(skip);
	}
	_graphics = newGfx;
	beginRemoveRows(QModelIndex(), 0, rowCount());
	endRemoveRows();
	beginInsertRows(QModelIndex(), 0, rowCount());
	endInsertRows();
	emit dataChanged(createIndex(0, 0), createIndex(rowCount() - 1, 0));
	return true;
}

void TileGfxModel::dump(QDataStream& stream) const {
	stream.setByteOrder(QDataStream::BigEndian);
	quint16 w = 0;
	for(int p = 0; p < 4; ++p) {
		for(int y = 0; y < _graphics.height(); ++y) {
			w = 0;
			for(int x = 0; x < _graphics.width(); ++x) {
				w |= ( ( (_graphics.pixelIndex(x, y) & (1 << p) ) != 0) << (15 - x) );
			}
			stream << w;
		}
	}
}

const QImage TileGfxModel::image(int width) const {
	qDebug() << width;
	int length = _graphics.height() / 16;
	QImage pic(width * 16, (length / width) * 16, QImage::Format_RGB32);
	pic.fill(0);
	QPainter painter(&pic);
	int x = 0, y = 0;
	for(int i = 0; i < length; ++i) {
		painter.drawImage(QRect(x, y, 16, 16), _graphics, QRect(0, i * 16, 16, 16));
		x += 16;
		if(x >= pic.width()) {
			x = 0;
			y += 16;
		}
	}
	painter.end();

	return pic.convertToFormat(QImage::Format_Indexed8, _graphics.colorTable());
}

void TileGfxModel::blank(void) {
	_graphics.fill(0);
}

void TileGfxModel::setColorTable(const QVector<QRgb>& colors) {
	_graphics.setColorTable(colors);
	emit dataChanged(createIndex(0, 0), createIndex(rowCount() - 1, 0));
}

int TileGfxModel::length(void) const {
	return (_graphics.width() * _graphics.height()) / 8;
}

TileGfxModel::TileGfxModel(QObject* parent): QAbstractListModel(parent) {
	//_graphics = QImage(16, 208, QImage::Format_Indexed8);
}
int TileGfxModel::rowCount(const QModelIndex & parent) const {
	return _graphics.height() / 16;
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
		if(!_graphics.isNull()) {
			painter.drawImage(QRect(0, 0, 16, 16), _graphics, QRect(0, index.row() * 16, 16, 16));
		}
		return icon;

	}
	return QVariant();
}
Qt::ItemFlags TileGfxModel::flags (const QModelIndex & index) const {
	if(!index.isValid()) return Qt::ItemIsEnabled;
	return QAbstractListModel::flags(index);
}
