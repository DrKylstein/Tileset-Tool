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
#include "TileInfo.h"
class TileInfoModel;
class TileAnimModel;
class TileGfxModel;
class TilePaletteModel;
class TileModel: public QObject /*QAbstractTableModel*/ {
	Q_OBJECT

	public:
		TileModel(QObject* parent = 0);

		int frameCount() const;
        int tileCount() const;

        QRgb getColor(int index, int row=0);
        void setColor(QRgb, int index, int row=0);

		void setFrameCount(int);
		const QImage& graphics();

		void blank();
		bool open(const QString&);
		bool save(const QString&) const;
		bool importImage(const QString&);
		bool fixPalette(const QString& = QString());
		bool loadPalette(const QString&);
		bool savePalette(const QString&);
        QVector<QRgb> getPaletteRow(int);

		TileInfoModel* tileInfo(void);
		TileAnimModel* tileAnim(void);
		TileGfxModel* tileGfx(void);
		friend class TileGfxModel;
		TilePaletteModel* tilePalette(void);

	signals:
		void frameCountChanged(int);
		void graphicsChanged(const QImage&);

	private:
		bool _openLemm(QFile&);
		bool _openVorticon(QFile&);
		bool _saveLemm(QFile&);
		bool _saveVorticon(QFile&);
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
