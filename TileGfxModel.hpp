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

#ifndef TILEGFXMODEL_HPP
#define TILEGFXMODEL_HPP
#include <QAbstractTableModel>
#include <QColor>
#include <QVector>
#include <QImage>
class TileGfxModel: public QAbstractListModel {
	Q_OBJECT

	public:
		bool load(QDataStream&, int, const QVector<QRgb>&, int skip = 0);
		bool load(const QImage&, const QVector<QRgb>&);
		void dump(QDataStream&) const;
		int length(void) const;
		void setColorTable(const QVector<QRgb>&);
		void blank(void);
		const QImage image(void) const;

		int rowCount(const QModelIndex & parent = QModelIndex()) const;
		QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
		QVariant data (const QModelIndex & index, int role = Qt::DisplayRole) const;
		Qt::ItemFlags flags (const QModelIndex & index) const;
		TileGfxModel(QObject* parent);

	private:
		QImage _graphics;
};
#endif
