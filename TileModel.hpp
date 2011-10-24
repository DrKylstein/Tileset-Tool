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

#ifndef TILESET_HPP
#define TILESET_HPP
#include <QAbstractTableModel>
#include <QVector>
#include <QImage>
#include <QFile>
class TileInfoModel;
class TileAnimModel;
class TileGfxModel;
class TilePaletteModel;
class TileModel: public QObject /*QAbstractTableModel*/ {
	Q_OBJECT

	public:
		TileModel(QObject* parent = 0);
		//int rowCount(const QModelIndex & parent = QModelIndex()) const;
		//int columnCount(const QModelIndex & parent = QModelIndex()) const;
		//QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
		//QVariant data (const QModelIndex & index, int role = Qt::DisplayRole) const;
		//Qt::ItemFlags flags (const QModelIndex & index) const;
		//bool setData (const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

		//bool insertRows (int row, int count, const QModelIndex & parent = QModelIndex());
		//bool removeRows (int row, int count, const QModelIndex & parent = QModelIndex());

		int frameCount() const;
		void setFrameCount(int);
		const QImage& graphics();

		void blank();
		bool open(const QString&);
		bool save(const QString&) const;
		bool importImage(const QString&);
		bool fixPalette(const QString& = QString());
		bool loadPalette(const QString&);
		bool savePalette(const QString&);

		TileInfoModel* tileInfo(void);
		friend class TileInfoModel;
		TileAnimModel* tileAnim(void);
		friend class TileAnimModel;
		TileGfxModel* tileGfx(void);
		friend class TileGfxModel;
		TilePaletteModel* tilePalette(void);
		friend class TilePaletteModel;

	signals:
		void frameCountChanged(int);
		void graphicsChanged(const QImage&);

	private:
		bool _openLemm(QFile&);
		//bool _openVorticon(QFile&);
		bool _openVorticon(QFile&);
		bool _saveLemm(QFile&);
		bool _saveVorticon(QFile&);
		QVector<int> blankTile;
		QVector<QVector<int> > tiles; // [tile][field]
		QImage _graphics;
		int numFrames;
		enum {FORMAT_VORTICON, FORMAT_LEMM, FORMAT_KRD} format;
		TileInfoModel* _tileInfo;
		TileAnimModel* _tileAnim;
		TileGfxModel* _tileGfx;
		TilePaletteModel* _tilePalette;
		QVector<QRgb> _defaultPal;
		void _extendPalette();
		bool _extractPalette(QImage&, QVector<unsigned int>&);
};
#endif
