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
TileModel::TileModel(QObject* parent): /*QAbstractTableModel(parent)*/ QObject(parent), blankTile(TileInfo::FIELD_COUNT), tiles(910, blankTile), numFrames(4) {
	_tileInfo = new TileInfoModel(this);
	_tileAnim = new TileAnimModel(this);
	_tileGfx = new TileGfxModel(this);
	_tilePalette = new TilePaletteModel(this);
	for(int i = 0; i < 16; ++i) {
		_defaultPal	<< 0xFF000000 << 0xFF0000AA << 0xFF00AA00 << 0xFF00AAAA << 0xFFAA0000 << 0xFFAA00AA << 0xFFAA5500 << 0xFFAAAAAA
					<< 0xFF555555 << 0xFF5555FF << 0xFF55FF55 << 0xFF55FFFF << 0xFFFF5555 << 0xFFFF55FF << 0xFFFFFF55 << 0xFFFFFFFF;
	}
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
	return numFrames;
}
void TileModel::setFrameCount(int i) {
	if(i == 4 || i == 8) {
		numFrames = i;
		//emit frameCountChanged(numFrames);
		_tileAnim->changeFrameCount(i);
	}
}

const QImage& TileModel::graphics() {
	return _graphics;
}

void TileModel::blank() {
	/*_tileInfo->beginRemoveRows(QModelIndex(), 0, tiles.size() - 1);
	_tileAnim->beginRemoveRows(QModelIndex(), 0, tiles.size() - 1);*/
	tiles.clear();
	/*_tileInfo->endRemoveRows();
	_tileAnim->endRemoveRows();
	_tileInfo->beginInsertRows(QModelIndex(), 0, 909);
	_tileAnim->beginInsertRows(QModelIndex(), 0, 909);*/
	tiles = QVector<QVector<int> >(910, blankTile);
	_graphics = QImage();
	setFrameCount(4);
	emit graphicsChanged(_graphics);
	_tileInfo->markAllNew();
	_tileAnim->markAllNew();
	_tileGfx->markAllNew();
	_tilePalette->markAllNew();
}
bool TileModel::open(const QString& filename) {
	QFile file(filename);
	if(!file.open(QIODevice::ReadOnly)) return false;
	if(filename.endsWith(".tls", Qt::CaseInsensitive)) {
		return _openLemm(file);
	}
	return _openVorticon(file);
}
bool TileModel::_openLemm(QFile& file) {
	if(file.size() < 18) return false;
	QDataStream stream(&file);
	stream.setByteOrder(QDataStream::LittleEndian);
	quint16 tileCount, frameCount, gfxLength;
	quint32 infoLoc, animLoc, gfxLoc, palLoc;
	stream >> tileCount >> frameCount >> gfxLength >> infoLoc >> animLoc >> gfxLoc >> palLoc;
	if(file.size() < 18 + (tileCount * 5) + (tileCount * TileInfo::MAX_FRAMES * 2) + (gfxLength * 4)) return false;
	tiles.clear();
	tiles = QVector<QVector<int> >(tileCount, blankTile);

	file.seek(infoLoc); //stream.skipRawData(46);
	for(unsigned int i = 0; i != tileCount; ++i) {
		qint8 b;
		stream >> b;
		if(b == -1) {
			tiles[i][TileInfo::FIELD_BEHAVIOR] = TileInfo::BEHAVE_FOREGROUND;
		}else if(b == -2) {
			tiles[i][TileInfo::FIELD_BEHAVIOR] = TileInfo::BEHAVE_MASKED;
		} else {
			tiles[i][TileInfo::FIELD_BEHAVIOR] = b;
		}
	}
	for(unsigned int i = 0; i != tileCount; ++i) {
		qint8 b;
		stream >> b;
		unsigned int surf = b & 0x03;
		tiles[i][TileInfo::FIELD_TOP] = (b != 0);
		tiles[i][TileInfo::FIELD_SURFACE_TYPE] =  surf != 0 ? surf - 1 : 0;
		tiles[i][TileInfo::FIELD_SLOPE_ENABLED] = 0;
		tiles[i][TileInfo::FIELD_SLOPE_SIDE] = 0;
		tiles[i][TileInfo::FIELD_SLOPE] = 0;
		tiles[i][TileInfo::FIELD_SLOPE_OFFSET] = 0;
		if(b & 0x80) {
			tiles[i][TileInfo::FIELD_SLOPE_ENABLED] = 1;
			tiles[i][TileInfo::FIELD_SLOPE_SIDE] = TileInfo::SLOPED_TOP;
			tiles[i][TileInfo::FIELD_SLOPE_OFFSET] = b & 0x0C;
			tiles[i][TileInfo::FIELD_SLOPE] = pow(2, (b & 0x30) >> 4);
			if(b & 0x40) {
				tiles[i][TileInfo::FIELD_SLOPE] *= -1;
				tiles[i][TileInfo::FIELD_SLOPE_OFFSET] += 4;
			}
		}
	}
	for(unsigned int i = 0; i != tileCount; ++i) {
		qint8 b;
		stream >> b;
		tiles[i][TileInfo::FIELD_RIGHT] = b;
	}
	for(unsigned int i = 0; i != tileCount; ++i) {
		qint8 b;
		stream >> b;
		unsigned int surf = b & 0x03;
		tiles[i][TileInfo::FIELD_BOTTOM] = (b != 0);
		tiles[i][TileInfo::FIELD_BOTTOM_BEHAVIOR] =  surf != 0 ? surf - 1 : 0;
		if(b & 0x80 && !tiles[i][TileInfo::FIELD_SLOPE_ENABLED]) {
			tiles[i][TileInfo::FIELD_SLOPE_ENABLED] = 1;
			tiles[i][TileInfo::FIELD_SLOPE_SIDE] = TileInfo::SLOPED_BOTTOM;
			tiles[i][TileInfo::FIELD_SLOPE_OFFSET] = b & 0x0C;
			tiles[i][TileInfo::FIELD_SLOPE] = pow(2, (b & 0x30) >> 4);
			if(b & 0x40) {
				tiles[i][TileInfo::FIELD_SLOPE] *= -1;
				tiles[i][TileInfo::FIELD_SLOPE_OFFSET] += 4;
			}
		}
	}
	for(unsigned int i = 0; i != tileCount; ++i) {
		qint8 b;
		stream >> b;
		tiles[i][TileInfo::FIELD_LEFT] = b;
	}
	file.seek(animLoc);
	for(unsigned int i = 0; i != frameCount; ++i) {
		for(unsigned int j = 0; j != tileCount; j++) {
			quint16 w;
			stream >> w;
			tiles[j][TileInfo::FIELD_FRAME1 + i] = ((signed int)(w) >> 5) /*- j*/;
		}
	}
	file.seek(palLoc);
	QImage sheetImage(16, gfxLength >> 1, QImage::Format_Indexed8);
	QVector<QRgb> pal;
	if((file.size() < 18 + (tileCount * 5) + (tileCount * frameCount * 2) + (gfxLength * 4) + 768) || palLoc < 64 || palLoc > (file.size() - 768) ) {
		pal	= _defaultPal;
	} else {
		quint32 c;
		quint8 b;
		for(int i = 0; i < 256; i++) {
			c = 0xFF000000;
			stream >> b;
			c |= (quint32(b) << 18) & 0xFF0000;
			stream >> b;
			c |= (quint32(b) << 10) & 0xFF00;
			stream >> b;
			c |= (quint32(b) << 2) & 0xFF;
			pal << c;
		}
	}
	sheetImage.setColorTable(pal);
	sheetImage.fill(0);
	file.seek(gfxLoc);
	stream.setByteOrder(QDataStream::BigEndian);
	for(unsigned int p = 0; p != 4; ++p) {
		for(unsigned int y = 0; y != gfxLength >> 1; ++y) {
			quint16 w;
			stream >> w;
			for(unsigned int x = 0; x != 16; ++x) {
				sheetImage.setPixel(x, y, sheetImage.pixelIndex(x, y) | ( (w & (1 << (15 - x) ) ) >> (15 - x) ) << p);
			}
		}
	}
	_graphics = sheetImage;
	emit graphicsChanged(_graphics);
	setFrameCount(frameCount);
	emit frameCountChanged(frameCount);
	_tileAnim->changeFrameCount(frameCount);
	_tileInfo->markAllNew(); //emit dataChanged(createIndex(0, 0), createIndex(rowCount() - 1, columnCount() - 1));
	_tileAnim->markAllNew();
	_tileGfx->markAllNew();
	_tilePalette->markAllNew();
	return true;
}
bool TileModel::_openVorticon(QFile& file) {
	QDataStream stream(&file);
	stream.setByteOrder(QDataStream::LittleEndian);
	int tileCount;
	if(file.fileName().endsWith(".exe", Qt::CaseInsensitive)) {
		if(file.size() < 0x1C) return false;
		quint16 mz;
		stream >> mz;
		if(mz != 0x5A4D) return false;
		quint16 lastBlockLength, blockCount, relocationCount, headerParagraphs;
		stream >> lastBlockLength >> blockCount >> relocationCount >> headerParagraphs;
		unsigned int exeOffset = headerParagraphs * 16;
		unsigned int exeSize = ( ( (blockCount - 1) * 512) + lastBlockLength) - exeOffset;
		if(file.size() < exeOffset + exeSize) return false;
		unsigned int tileInfoOffset;
		switch(exeSize) {
			case 99762: // ep 1 v1.1
				tileInfoOffset = 0x131F8;
				tileCount = 611;
				break;
			case 99972: // ep 1 v1.31
				tileInfoOffset = 0x130F8;
				tileCount = 611;
				break;
			case 118114: // ep 2 v1.1
				tileInfoOffset = 0x17938;
				tileCount = 689;
				break;
			case 118160: // ep2 v1.31
				tileInfoOffset = 0x17828;
				tileCount = 689;
				break;
			case 127086: // ep3 v1.1
				tileInfoOffset = 0x199F8;
				tileCount = 715;
				break;
			case 127104: // ep3 v1.31
				tileInfoOffset = 0x198C8;
				tileCount = 715;
				break;
			default:
				return false;
		};
		stream.skipRawData(exeOffset + tileInfoOffset - 0xA);
	} else if(file.fileName().endsWith(".tli", Qt::CaseInsensitive)) {
		if(file.size() % 12) return false;
		tileCount = file.size() / 12;
	} else {
		return false;
	}

	tiles.clear();
	tiles = QVector<QVector<int> >(910, blankTile);

	for(unsigned int i = 0; i != tileCount; ++i) {
		quint16 w;
		stream >> w;
		switch(w) {
			case 2:
				tiles[i][TileInfo::FIELD_FRAME3] = tiles[i][TileInfo::FIELD_FRAME1] = i;
				tiles[i][TileInfo::FIELD_FRAME4] = tiles[i][TileInfo::FIELD_FRAME2] = i+1;
				++i;
				tiles[i][TileInfo::FIELD_FRAME3] = tiles[i][TileInfo::FIELD_FRAME1] = i;
				tiles[i][TileInfo::FIELD_FRAME4] = tiles[i][TileInfo::FIELD_FRAME2] = i-1;
				stream.skipRawData(2);
				break;

			case 4:
				tiles[i][TileInfo::FIELD_FRAME1] = i;
				tiles[i][TileInfo::FIELD_FRAME2] = i+1;
				tiles[i][TileInfo::FIELD_FRAME3] = i+2;
				tiles[i][TileInfo::FIELD_FRAME4] = i+3;
				++i;
				tiles[i][TileInfo::FIELD_FRAME1] = i;
				tiles[i][TileInfo::FIELD_FRAME2] = i+1;
				tiles[i][TileInfo::FIELD_FRAME3] = i+2;
				tiles[i][TileInfo::FIELD_FRAME4] = i-1;
				++i;
				tiles[i][TileInfo::FIELD_FRAME1] = i;
				tiles[i][TileInfo::FIELD_FRAME2] = i+1;
				tiles[i][TileInfo::FIELD_FRAME3] = i-2;
				tiles[i][TileInfo::FIELD_FRAME4] = i-1;
				++i;
				tiles[i][TileInfo::FIELD_FRAME1] = i;
				tiles[i][TileInfo::FIELD_FRAME2] = i-3;
				tiles[i][TileInfo::FIELD_FRAME3] = i-2;
				tiles[i][TileInfo::FIELD_FRAME4] = i-1;
				stream.skipRawData(6);
				break;

			default:
				tiles[i][TileInfo::FIELD_FRAME1] = i;
				tiles[i][TileInfo::FIELD_FRAME2] = i;
				tiles[i][TileInfo::FIELD_FRAME3] = i;
				tiles[i][TileInfo::FIELD_FRAME4] = i;
				break;
		}
	}
	for(unsigned int i = 0; i != tileCount; ++i) {
		qint16 w;
		stream >> w;
		if(w == -1) {
			tiles[i][TileInfo::FIELD_BEHAVIOR] = TileInfo::BEHAVE_FOREGROUND;
		}else if(w == -2) {
			tiles[i][TileInfo::FIELD_BEHAVIOR] = TileInfo::BEHAVE_MASKED;
		} else {
			tiles[i][TileInfo::FIELD_BEHAVIOR] = w;
		}
	}
	for(unsigned int i = 0; i != tileCount; ++i) {
		quint16 w;
		stream >> w;
		tiles[i][TileInfo::FIELD_TOP] = (w != 0);
		tiles[i][TileInfo::FIELD_SURFACE_TYPE] = w != 0 ? w - 1 : 0;
	}
	for(unsigned int i = 0; i != tileCount; ++i) {
		quint16 w;
		stream >> w;
		tiles[i][TileInfo::FIELD_RIGHT] = w;
	}
	for(unsigned int i = 0; i != tileCount; ++i) {
		quint16 w;
		stream >> w;
		tiles[i][TileInfo::FIELD_BOTTOM] = (w != 0);
		tiles[i][TileInfo::FIELD_BOTTOM_BEHAVIOR] = w != 0 ? w - 1 : 0;
	}
	for(unsigned int i = 0; i != tileCount; ++i) {
		quint16 w;
		stream >> w;
		tiles[i][TileInfo::FIELD_LEFT] = w;
	}
	setFrameCount(4);
	_tileInfo->markAllNew(); //emit dataChanged(createIndex(0, 0), createIndex(rowCount() - 1, columnCount() - 1));
	_tileGfx->markAllNew();
	_tilePalette->markAllNew();
	_tileAnim->markAllNew();

	//try to get graphics as well
	QString extension;
	switch(tileCount) {
		case 611:
			extension = "CK1";
			break;
		case 689:
			extension = "CK2";
			break;
		case 715:
			extension = "CK3";
			break;
		default:
			return true;
	}
	QString path = QFileInfo(file.fileName()).path();
	QDir dir(path);
	QStringList files = dir.entryList( QStringList( QString("EGAHEAD.%1").arg(extension) ) );
	if(files.count() == 0) return true;
	QString headFilename = path;
	headFilename.append( QString("/%1").arg(files[0]) );
	//qDebug() << headFilename;
	importImage(headFilename);

	return true;
}
bool TileModel::save(const QString& filename) const {
	QFile file(filename);
	if(!file.open(QIODevice::WriteOnly)) {
		return false;
	}
	QDataStream stream(&file);
	stream.setByteOrder(QDataStream::LittleEndian);
	quint16 tileCount = tiles.size();
	quint16 frames = frameCount();
	quint16 gfxLength = (_graphics.width() * _graphics.height()) / 8;
	quint32 infoLoc = 64;
//		int behave = infoLoc;
//		int top = behave + tileCount;
//		int right = top + tileCount;
//		int bottom = right + tileCount;
//		int left = bottom + tileCount;
	quint32 animLoc = infoLoc + tileCount * 5;
	quint32 gfxLoc = animLoc + tileCount * TileInfo::MAX_FRAMES * 2;
//		int blue = gfxLoc;
//		int green = blue + gfxLength;
//		int red = green + gfxLength;
//		int intensity = red + gfxLength;
	quint32 palLoc = gfxLoc + (gfxLength * 4);

	stream << tileCount << frames << gfxLength << infoLoc << animLoc << gfxLoc << palLoc;
	stream << quint32(0) << quint32(0) << quint32(0) << quint32(0) << quint32(0) << quint32(0) << quint32(0) << quint32(0) << quint32(0) << quint32(0) << quint16(0);
	for(int i = 0; i < tiles.size(); ++i) {
		qint8 b;
		if(tiles[i][TileInfo::FIELD_BEHAVIOR] == TileInfo::BEHAVE_MASKED) {
			b = -2;
		} else if(tiles[i][TileInfo::FIELD_BEHAVIOR] == TileInfo::BEHAVE_FOREGROUND) {
			b = -1;
		} else {
			b = tiles[i][TileInfo::FIELD_BEHAVIOR];
		}
		stream << b;
	}
	for(int i = 0; i < tiles.size(); ++i) {
		quint8 b = 0;
		if(tiles[i][TileInfo::FIELD_TOP]) {
			b = (tiles[i][TileInfo::FIELD_SURFACE_TYPE] + 1);
			if(tiles[i][TileInfo::FIELD_SLOPE_ENABLED] && tiles[i][TileInfo::FIELD_SLOPE_SIDE] == TileInfo::SLOPED_TOP) {
				b |= 0x80 | (quint8(log(abs(tiles[i][TileInfo::FIELD_SLOPE])) / log(2)) << 4);
				if(tiles[i][TileInfo::FIELD_SLOPE] < 0) {
					b |= 0x40 | (tiles[i][TileInfo::FIELD_SLOPE_OFFSET] - 4);
				} else {
					b |= tiles[i][TileInfo::FIELD_SLOPE_OFFSET];
				}
			}
		}
		stream << b;
	}
	for(int i = 0; i < tiles.size(); ++i) {
		quint8 b = 0;
		if(tiles[i][TileInfo::FIELD_BOTTOM] &&
		tiles[i][TileInfo::FIELD_SLOPE_ENABLED] &&
		tiles[i][TileInfo::FIELD_SLOPE_SIDE] == TileInfo::SLOPED_BOTTOM &&
		tiles[i][TileInfo::FIELD_SLOPE] > 0) {
			b = 1 | 0x80 | (quint8(log(abs(tiles[i][TileInfo::FIELD_SLOPE])) / log(2)) << 4) | tiles[i][TileInfo::FIELD_SLOPE_OFFSET];
		} else if(tiles[i][TileInfo::FIELD_LEFT]) {
			b = 1;
		}
		stream << b;
	}
	for(int i = 0; i < tiles.size(); ++i) {
		quint8 b = 0;
		if(tiles[i][TileInfo::FIELD_BOTTOM]) {
			b = (tiles[i][TileInfo::FIELD_BOTTOM_BEHAVIOR] + 1);
			if(tiles[i][TileInfo::FIELD_SLOPE_ENABLED] && tiles[i][TileInfo::FIELD_SLOPE_SIDE] == TileInfo::SLOPED_BOTTOM) {
				b |= 0x80 | (quint8(log(abs(tiles[i][TileInfo::FIELD_SLOPE])) / log(2)) << 4);
				if(tiles[i][TileInfo::FIELD_SLOPE] < 0) {
					b |= 0x40 | (tiles[i][TileInfo::FIELD_SLOPE_OFFSET] - 4);
				} else {
					b |= tiles[i][TileInfo::FIELD_SLOPE_OFFSET];
				}
			}
		}
		stream << b;
	}
	for(int i = 0; i < tiles.size(); ++i) {
		quint8 b = 0;
		if(tiles[i][TileInfo::FIELD_BOTTOM] &&
		tiles[i][TileInfo::FIELD_SLOPE_ENABLED] &&
		tiles[i][TileInfo::FIELD_SLOPE_SIDE] == TileInfo::SLOPED_BOTTOM &&
		tiles[i][TileInfo::FIELD_SLOPE] < 0) {
			b = 1 | 0x80 | (quint8(log(abs(tiles[i][TileInfo::FIELD_SLOPE])) / log(2)) << 4) | 0x40 | (tiles[i][TileInfo::FIELD_SLOPE_OFFSET] - 4);
		} else if(tiles[i][TileInfo::FIELD_RIGHT]) {
			b = 1;
		}
		stream << b;
	}
	for(int j = 0; j < TileInfo::MAX_FRAMES; ++j) {
		for(int i = 0; i < tiles.size(); ++i) {
			stream << qint16((tiles[i][(j % frames)+TileInfo::FIELD_FRAME1]) << 5);
		}
	}
	stream.setByteOrder(QDataStream::BigEndian);
	for(int p = 0; p < 4; ++p) {
		for(int y = 0; y < gfxLength >> 1; ++y) {
			quint16 w = 0;
			for(int x = 0; x != 16; ++x) {
				w |= ( ( (_graphics.pixelIndex(x, y) & (1 << p) ) != 0) << (15 - x) );
			}
			stream << w;
		}
	}
	stream.setByteOrder(QDataStream::LittleEndian);
	for(int i = 0; i < 256; ++i) {
		unsigned int color = _graphics.color(i);
		stream << quint8( (color & 0x00FF0000) >> 18);
		stream << quint8( (color & 0x0000FF00) >> 10);
		stream << quint8( (color & 0x000000FF) >> 2);
	}
	return true;
}
bool TileModel::importImage(const QString& filename) {
	QImage newGfx;
	if(newGfx.load(filename)) {
			if(newGfx.width() % 16) return false;
			if(newGfx.height() % 16) return false;
			if(newGfx.numColors() == 0) { //32/24/16bit color, must convert
				//QVector<QRgb> pal;
				//pal	= _defaultPal;
				newGfx = newGfx.convertToFormat(QImage::Format_Indexed8);
			} else if(newGfx.numColors() > 16) { //256 color, reduce to 16 colors to be safe
				QVector<QRgb> pal = newGfx.colorTable();
				pal.resize(16); //this can go horribly wrong if the image is not just 16 colors with a padded-out palette. Too bad there's no way proper to reduce to 16 colors in Qt.
				newGfx = newGfx.convertToFormat(QImage::Format_Indexed8, pal);
			}
			int length = (newGfx.width() / 16) * (newGfx.height() / 16);
			_graphics = QImage(16, length * 16, QImage::Format_RGB32);
			_graphics.fill(0);
			QPainter painter(&_graphics);
			int x = 0, y = 0;
			for(int i = 0; i < length; ++i) {
				painter.drawImage(QRect(0, i * 16, 16, 16), newGfx, QRect(x, y, 16, 16));
				x += 16;
				if(x >= newGfx.width()) {
					x = 0;
					y += 16;
				}
			}
			painter.end();
			_graphics = _graphics.convertToFormat(QImage::Format_Indexed8, newGfx.colorTable());
	} else {
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

		QImage sheetImage(16, tileCount * 16, QImage::Format_Indexed8);
		QVector<QRgb> pal;
		pal	= _defaultPal;
		sheetImage.setColorTable(pal);
		sheetImage.fill(0);

		QString extension = filename.right(3);
		QString path = QFileInfo(filename).path();
		QDir dir(path);
		QStringList files = dir.entryList( QStringList( QString("EGALATCH.%1").arg(extension) ) );
		if(files.count() == 0) return false;
		QString latchFilename = path;
		latchFilename.append(QString("/%1").arg(files[0]) );
		//qDebug() << latchFilename;

		QFile latchFile(latchFilename);
		if(!latchFile.open(QIODevice::ReadOnly)) {
			return false;
		}

		QBuffer decompressedData;

		QIODevice* latchData;

		if(compression) {
			return false; // code unfinished
			QDataStream outStream(&decompressedData);
			QDataStream inStream(&latchFile);



			latchData = &decompressedData;
		} else {
			latchData = &latchFile;
		}

		QDataStream latchStream(latchData);
		latchStream.setByteOrder(QDataStream::BigEndian);

		for(unsigned int p = 0; p != 4; ++p) {
			latchData->seek((p * latchPlaneSize) + tileLocation);
			for(unsigned int y = 0; y != tileCount * 16; ++y) {
				quint16 w;
				latchStream >> w;
				for(unsigned int x = 0; x != 16; ++x) {
					sheetImage.setPixel(x, y, sheetImage.pixelIndex(x, y) | ( (w & (1 << (15 - x) ) ) >> (15 - x) ) << p);
				}
			}
		}

		_graphics = sheetImage;
	}
	_extendPalette();
	_tileGfx->markAllNew();
	_tilePalette->markAllNew();
	_tileAnim->markAllNew();
	return true;
}
bool TileModel::loadPalette(const QString& filename) {
	QVector<QRgb> newpal;
	if(filename.endsWith(".bmp", Qt::CaseInsensitive) || filename.endsWith(".png", Qt::CaseInsensitive) || filename.endsWith(".tiff", Qt::CaseInsensitive)) {
		QImage donor(filename);
		if(!_extractPalette(donor, newpal)) return false;
	} else {
		QFile file(filename);
		if(!file.open(QIODevice::ReadOnly)) return false;
		if(file.size() < 48) return false;
		uchar * buffer = file.map(0, 48);
		for(int i = 0; i < 48; i += 3) {
			newpal.push_back(0xFF000000 | ( (buffer[i] & 0x3f) << 18 ) | ( (buffer[i+1] & 0x3f) << 10) | ( (buffer[i+2] & 0x3f) << 2) );
		}
	}
	_graphics.setColorTable(newpal);
	_extendPalette();
	//emit graphicsChanged(_graphics);
	_tileGfx->markAllNew();
	_tilePalette->markAllNew();
	_tileAnim->markAllNew();
	return true;
}
bool TileModel::savePalette(const QString& filename) {
	QFile file(filename);
	if(!file.open(QIODevice::WriteOnly)) {
		return false;
	}
	QDataStream out(&file);
	for(int i = 0; i < _graphics.colorCount(); ++i) {
		out << quint8( (_graphics.color(i) >> 18) & 0x3F);
		out << quint8( (_graphics.color(i) >> 10) & 0x3F);
		out << quint8( (_graphics.color(i) >> 2) & 0x3F);
	}
	return true;
}
bool TileModel::fixPalette(const QString& filename) {
	QVector<QRgb> newpal;
	if(filename.isEmpty()) {
		newpal	= _defaultPal;
	} else if(filename.endsWith(".bmp", Qt::CaseInsensitive) || filename.endsWith(".png", Qt::CaseInsensitive) || filename.endsWith(".tiff", Qt::CaseInsensitive)) {
		QImage donor(filename);
		if(!_extractPalette(donor, newpal)) return false;
	} else {
		QFile file(filename);
		if(!file.open(QIODevice::ReadOnly)) return false;
		if(file.size() < 48) return false;
		uchar * buffer = file.map(0, 48);
		for(int i = 0; i < 48; i += 3) {
			newpal.push_back(0xFF000000 | ( (buffer[i] & 0x3f) << 18 ) | ( (buffer[i+1] & 0x3f) << 10) | ( (buffer[i+2] & 0x3f) << 2) );
		}
		return false;
	}
	_graphics = _graphics.convertToFormat(QImage::Format_RGB32).convertToFormat(QImage::Format_Indexed8, newpal.mid(0, 16));
	_graphics.setColorTable(newpal);
	_extendPalette();
	//emit graphicsChanged(_graphics);
	_tileGfx->markAllNew();
	_tilePalette->markAllNew();
	_tileAnim->markAllNew();
	return true;
}
bool TileModel::_extractPalette(QImage& donor, QVector<unsigned int>& newpal) {
	if(donor.isNull()) return false;
	if(donor.colorCount() != 0) {
		newpal = donor.colorTable();
	} else if(newpal.isEmpty()) {
		if(donor.width() < 16) return false;
		int lines = donor.height();
		if(lines > 16) lines = 16;
		for(int y = 0; y != lines; ++y) {
			for(int x = 0; x != 16; x++){
				newpal.push_back(donor.pixel(x,y));
			}
		}
	}
	return true;
}
void TileModel::_extendPalette() {
	QVector<QRgb> basePal = _graphics.colorTable();
	QVector<QRgb> newPal = basePal;
	while(newPal.size() < 256) {
		newPal += (basePal.mid(0,16));
	}
	if(newPal.size() > 256) {
		newPal.resize(256);
	}
	_graphics.setColorCount(256);
	_graphics.setColorTable(newPal);
}
