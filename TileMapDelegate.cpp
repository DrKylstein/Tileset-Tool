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

#include "TileMapDelegate.hpp"
#include <QtGui>
TileMapDelegate::TileMapDelegate(QWidget* parent): QAbstractItemDelegate(parent) {
}
void TileMapDelegate::paint (QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
	if(!index.model() || !_gfxModel) return;
	if(_gfxModel->index(index.model()->data(index).toInt(), 0 ).data().value<QPixmap>().isNull()) {
		painter->fillRect(option.rect, QApplication::palette().dark());
	} else {
		painter->drawPixmap(option.rect, _gfxModel->index(index.model()->data(index).toInt(), 0 ).data().value<QPixmap>());
	}
	if (option.state & QStyle::State_HasFocus) {
		QColor tint = QApplication::palette().highlight().color();
		tint.setAlphaF(0.25);
		painter->fillRect(option.rect, tint);
		QPen highlighter; highlighter.setColor(QColor(255, 255, 255)); highlighter.setStyle(Qt::DashLine); highlighter.setWidth(1);
		QRect r = option.rect;
		r.setWidth(r.width() - 1);
		r.setHeight(r.height() - 1);
		painter->save();
		painter->setPen(highlighter);
		painter->drawRect(r);
		painter->restore();
	}
}
QSize TileMapDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const {
	return QSize();
}
void TileMapDelegate::setGfxModel(QAbstractItemModel * model) {
	_gfxModel = model;
}

