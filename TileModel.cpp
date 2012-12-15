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

#include "TileModel.hpp"
#include "TileInfo.h"
#include "TileInfoModel.hpp"
#include "TileAnimModel.hpp"
#include "TileGfxModel.hpp"
#include "TilePaletteModel.hpp"
#include <QtGui>
#include <cassert>
#include "TileInfoHeuristics.hpp"
using namespace TileInfoHeuristics;

void TileModel::_dataChanged(void) {
	emit modificationStateChanged(true);
}

TileModel::TileModel(QObject* parent): QObject(parent) {
	_tileInfo = new TileInfoModel(this);
	_tileAnim = new TileAnimModel(this);
	_tileGfx = new TileGfxModel(this);
	_tilePalette = new TilePaletteModel(this);
	connect(_tileInfo, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(_dataChanged(void)));
	connect(_tileAnim, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(_dataChanged(void)));
	connect(_tileGfx, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(_dataChanged(void)));
	connect(_tilePalette, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(_dataChanged(void)));
}
TileInfoModel* TileModel::tileInfo(void) {
	return _tileInfo;
}
TileAnimModel* TileModel::tileAnim(void) {
	return _tileAnim;
}
TileGfxModel* TileModel::tileGfx(void) {
	return _tileGfx;
}
TilePaletteModel* TileModel::tilePalette(void) {
	return _tilePalette;
}

int TileModel::frameCount() const {
	return TileInfo::MAX_FRAMES;
}
int TileModel::tileCount() const {
	return TileInfo::MAX_TILES;
}

void TileModel::blank() {
	_tileInfo->blank();
	_tileAnim->blank();
	_tileGfx->blank();
	_tilePalette->reset();
}
bool TileModel::open(const QString& filename) {
	QFile file(filename);
	if(!file.open(QIODevice::ReadOnly)) return false;
	if(filename.endsWith(".tls", Qt::CaseInsensitive)) {
		if(!_openLemm(file)) {
			return false;
		}
	} else {
		if(!_openVorticon(file)) {
			return false;
		}
	}
	//emit modificationStateChanged(false);
	return true;
}
bool TileModel::_openLemm(QFile& file) {
    quint8 b;
    quint16 w;

	if(file.size() < 18) return false;
	QDataStream stream(&file);
	stream.setByteOrder(QDataStream::LittleEndian);
	quint16 tileCount, frameCount, gfxLength;
	quint32 infoLoc, animLoc, gfxLoc, palLoc;
	stream >> tileCount >> frameCount >> gfxLength >> infoLoc >> animLoc >> gfxLoc >> palLoc;
	if(file.size() < 18 + (tileCount * 5) + (tileCount * TileInfo::MAX_FRAMES * 2) + (gfxLength * 4)) return false;

	file.seek(infoLoc);
    _tileInfo->load(stream);

	file.seek(animLoc);
    _tileAnim->load(stream);

	file.seek(palLoc);
	if((file.size() >= 18 + (tileCount * 5) + (tileCount * frameCount * 2) + (gfxLength * 4) + 768) &&
		palLoc > 64 &&
		palLoc <= (file.size() - 768) ) {
		if(!_tilePalette->load(stream)) return false;
	} else {
		_tilePalette->reset();
	}

	file.seek(gfxLoc);
	if(!_tileGfx->load(stream, gfxLength, _tilePalette->colorTable(0))) {
		return false;
	}

	_tileAnim->markAllNew();
	return true;
}
bool TileModel::_openVorticon(QFile& file) {
	QDataStream stream(&file);
	int tileCount = 0;
	if(file.fileName().endsWith(".exe", Qt::CaseInsensitive)) {
		tileCount = analyzeExe(stream);
		if(tileCount == 0) {
			qDebug() << "Bad exe.";
			return false;
		}
	} else {
		tileCount = tliLength(file);
	}
    if(!_tileAnim->loadClassic(stream, tileCount)){
		qDebug() << "Ran out of animation data.";
		return false;
    }
    if(!_tileInfo->loadClassic(stream, tileCount)) {
		qDebug() << "Ran out of info data.";
		return false;
    }
    return true;
}
bool TileModel::save(const QString& filename) {
	QFile file(filename);
	if(!file.open(QIODevice::WriteOnly)) {
		return false;
	}
	QDataStream stream(&file);
	stream.setByteOrder(QDataStream::LittleEndian);
	quint16 tileCount = 910;
	quint16 frames = 8;
	quint16 gfxLength = _tileGfx->length();
	quint32 infoLoc = 64;
	quint32 animLoc = infoLoc + tileCount * 5;
	quint32 gfxLoc = animLoc + tileCount * TileInfo::MAX_FRAMES * 2;
	quint32 palLoc = gfxLoc + (gfxLength * 4);

	stream << tileCount << frames << gfxLength << infoLoc << animLoc << gfxLoc << palLoc;
	stream << quint32(0) << quint32(0) << quint32(0) << quint32(0) << quint32(0) <<
		quint32(0) << quint32(0) << quint32(0) << quint32(0) << quint32(0) << quint16(0);
    _tileInfo->dump(stream);
    _tileAnim->dump(stream);
	_tileGfx->dump(stream);
	_tilePalette->dump(stream);
	//emit modificationStateChanged(false);
	return true;
}
bool TileModel::importImage(const QString& filename) {
	QImage newGfx(filename);
	if(newGfx.isNull()) return false;
	if(!_tilePalette->load(newGfx)) return false;
	if(!_tileGfx->load(newGfx, _tilePalette->colorTable(0))) return false;
	_tileAnim->markAllNew();
	return true;
}

bool TileModel::importEgaHead(const QString& filename) {
	QFile headFile(filename);
	if(!headFile.open(QIODevice::ReadOnly)) {
		return false;
	}
	QDataStream headStream(&headFile);
	headStream.setByteOrder(QDataStream::LittleEndian);
	quint32 latchPlaneSize;
	quint16 tileCount;
	quint32 tileLocation;
	quint16 compression;
	headStream >> latchPlaneSize;
	headFile.seek(28);
	headStream >> tileCount >> tileLocation;
	headFile.seek(46);
	headStream >> compression;

	if(compression) return false;

	_tilePalette->reset();

	QString extension = filename.right(3);
	QString path = QFileInfo(filename).path();
	QDir dir(path);
	QStringList files = dir.entryList( QStringList( QString("EGALATCH.%1").arg(extension) ) );
	if(files.count() == 0) return false;
	QString latchFilename = path;
	latchFilename.append(QString("/%1").arg(files[0]) );

	QFile latchFile(latchFilename);
	if(!latchFile.open(QIODevice::ReadOnly)) {
		return false;
	}

	latchFile.seek(tileLocation);
	QDataStream latchStream(&latchFile);
	_tileGfx->load(latchStream, tileCount * 32, _tilePalette->colorTable(0), latchPlaneSize-(tileCount*32));

	_tileAnim->markAllNew();
	return true;
}

bool TileModel::_doLoadPalette(const QString& filename) {
	if(filename.endsWith(".bmp", Qt::CaseInsensitive) || filename.endsWith(".png", Qt::CaseInsensitive) ||
		filename.endsWith(".tiff", Qt::CaseInsensitive)) {
		QImage donor(filename);
		if(!_tilePalette->load(donor)) return false;
	} else {
		QFile file(filename);
		if(!file.open(QIODevice::ReadOnly)) return false;
		QDataStream stream(&file);
		if(!_tilePalette->load(stream)) return false;
	}
	return true;
}
bool TileModel::loadPalette(const QString& filename) {
	if(!_doLoadPalette(filename)) return false;
	_tileGfx->setColorTable(_tilePalette->colorTable(0));
	_tileAnim->markAllNew();
	return true;
}
bool TileModel::savePalette(const QString& filename) {
	QFile file(filename);
	if(!file.open(QIODevice::WriteOnly)) {
		return false;
	}
	QDataStream out(&file);
	_tilePalette->dump(out);
	return true;
}
bool TileModel::fixPalette(const QString& filename) {
	if(filename == QString()) {
		_tilePalette->reset();
	} else {
		if(!_doLoadPalette(filename)) return false;
	}
	_tileGfx->load(_tileGfx->image(), _tilePalette->colorTable(0));
	_tileAnim->markAllNew();
	return true;
}
QVector<QRgb> TileModel::getPaletteRow(int page) {
    return _tilePalette->colorTable(page);
}
