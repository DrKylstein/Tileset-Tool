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

#ifndef TILEINFOMODEL_HPP
#define TILEINFOMODEL_HPP
#include <QAbstractTableModel>
#include "FourWay.hpp"

class TileInfoModel: public QAbstractTableModel {
	Q_OBJECT

	public:
		int rowCount(const QModelIndex & parent = QModelIndex()) const;
		int columnCount(const QModelIndex & parent = QModelIndex()) const;
		QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
		QVariant data (const QModelIndex & index, int role = Qt::DisplayRole) const;
		Qt::ItemFlags flags (const QModelIndex & index) const;
		bool setData (const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

        bool load(QDataStream&);
        bool loadClassic(QDataStream&, int);
        void dump(QDataStream&);
        void blank();

        TileInfoModel(QObject* parent=0): QAbstractTableModel(parent) {blank();}

        enum {
            FIELD_BEHAVIOR,
            FIELD_SLOPED_SIDE,
            FIELD_Y0,
            FIELD_RUN,
            FIELD_TOP_BLOCKING,
            FIELD_RIGHT_BLOCKING,
            FIELD_BOTTOM_BLOCKING,
            FIELD_LEFT_BLOCKING,
            FIELD_TOP_STYLE,
            FIELD_RIGHT_STYLE,
            FIELD_BOTTOM_STYLE,
            FIELD_LEFT_STYLE
        };
        static const int FIELD_COUNT = 12;
        typedef enum{NO_SLOPE, TOP_SLOPE, BOTTOM_SLOPE} slope_side;

	private:
		static const int _TILES = 910;

		static const int REAL_FOREGROUND = -1;
		static const int REAL_MASKED = -2;
		static const int FAKE_FOREGROUND = 28;
		static const int FAKE_MASKED = 29;

		struct {
            unsigned int behavior;
            slope_side slopedSide;
            unsigned int y0;
            signed int run;
            FourWay<bool> blocking;
            FourWay<unsigned int> style;
		} _tiles[_TILES];

};
#endif
