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

		void blank();
		bool open(const QString&);
		bool openClassic(const QString&);
		bool save(const QString&);
		bool importImage(const QString&);
		bool importEgaHead(const QString&);
		bool fixPalette(const QString& = QString());
		bool loadPalette(const QString&);
		bool savePalette(const QString&);
        QVector<QRgb> getPaletteRow(int);

		TileInfoModel* tileInfo(void);
		TileAnimModel* tileAnim(void);
		TileGfxModel* tileGfx(void);
		TilePaletteModel* tilePalette(void);

	signals:
		void graphicsChanged(const QImage&);
		void modificationStateChanged(bool);

	private slots:
		void _dataChanged(void);

	private:
		bool _saveLemm(QFile&);
		bool _saveVorticon(QFile&);
		TileInfoModel* _tileInfo;
		TileAnimModel* _tileAnim;
		TileGfxModel* _tileGfx;
		TilePaletteModel* _tilePalette;
		bool _doLoadPalette(const QString&);
};
#endif
