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
        void dump(QDataStream&);
        void blank();

        struct TileProperties {
            bool solid[4];
            bool sloped[4];
            unsigned int y0[4];
            int run[4];
            unsigned int surfaceType[4];
            unsigned int behavior;

            TileProperties() {
                for(int i=0; i<4; ++i) {
                    solid[i] = false;
                    sloped[i] = false;
                    y0[i] = 0;
                    run[i] = 0;
                    surfaceType[i] = 0;
                }
                behavior = 0;
            }
        };


		TileProperties& tile(int);
		void markTileUpdated(int);

        TileInfoModel(QObject* parent=0): QAbstractTableModel(parent) {}

        enum {
            FIELD_BEHAVIOR,
            FIELD_SOLID,
            FIELD_SLOPED = 5,
            FIELD_Y0 = 7,
            FIELD_RUN = 13,
            FIELD_SURFACE_TYPE = 17,
        };
        enum {TOP, RIGHT, BOTTOM, LEFT};

	private:
        static const int _MAX_TILES = 910;
		TileProperties _tileProperties[_MAX_TILES];

};
#endif
