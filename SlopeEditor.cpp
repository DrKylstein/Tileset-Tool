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

#include <QtGui>
#include "SlopeEditor.hpp"
#include "TileInfo.h"
SlopeEditor::SlopeEditor(QWidget* parent): QWidget(parent), _slope(0), _offset(0), _side(0), row(0) {
	setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed, QSizePolicy::Frame));
	setFocusPolicy(Qt::ClickFocus);
}
QSize SlopeEditor::sizeHint() const {
	return QSize(64, 64);
}
void SlopeEditor::paintEvent(QPaintEvent* event) {
	QPainter painter(this);
	QPalette pal = QApplication::palette();
	if(isEnabled()) {
		pal.setCurrentColorGroup(QPalette::Normal);
	} else {
		pal.setCurrentColorGroup(QPalette::Disabled);
	}
	float scale = event->rect().height() / 16;
	QLine line = QLine(event->rect().left(), _offset * scale, event->rect().right(), (_offset * scale) + (_slope != 0 ? (event->rect().right() / _slope) : 0));
	QPolygon p;
	painter.setRenderHint(QPainter::Antialiasing);
	painter.fillRect(0, 0, width(), height(), pal.color(QPalette::Base));
	QBrush brush = pal.highlight();
	painter.setBrush(brush);
	painter.setPen(Qt::NoPen);
	if(_side == TileInfo::SLOPED_TOP) {
		p.setPoints(4, line.x1(), line.y1(), line.x2() + 1, line.y2(), event->rect().right() + 1, event->rect().bottom() + 1, event->rect().left(), event->rect().bottom() + 1);
		painter.drawPolygon(p);
	} else if(_side == TileInfo::SLOPED_BOTTOM) {
		p.setPoints(4, line.x1(), line.y1(), line.x2() + 1, line.y2(), event->rect().right() + 1, event->rect().top(), event->rect().left(), event->rect().top());
		painter.drawPolygon(p);
	}
	QPen gridPen; gridPen.setColor(pal.color(QPalette::Text)); gridPen.setStyle(Qt::SolidLine); gridPen.setWidth(2);
	QPen slopePen; slopePen.setColor(pal.color(QPalette::Text)); slopePen.setStyle(Qt::SolidLine); slopePen.setWidth(3);
	painter.setPen(gridPen);
	for(int i = 1; i < 16; i += 4) {
		painter.drawLine(event->rect().left(), i * scale, event->rect().left() + 16, i * scale);
		painter.drawLine(event->rect().right(), i * scale, event->rect().right() - 16, i * scale);
	}
	painter.setPen(slopePen);
	painter.drawLine(line);
}
void SlopeEditor::mousePressEvent(QMouseEvent* event) {
	int scale = width() / 16;
	if(event->x() < 4 * scale) {
		setOffset( ( (event->y() + 8) / 16) * 4);
	} else if(event->x() > width() - (4 * scale) ) {
		int i =  (_offset >> 2) - ((event->y() + 8) / 16);
		if( i != 0) {
			i = -4 / i;
			if(i <= 8 && i >= -8)
			setSlope(i);
		} else {
			setSlope(0);
		}
	} else if((_offset * scale) + (_slope != 0 ? (event->x() / _slope) : 0) > event->y()) {
		setSide(TileInfo::SLOPED_BOTTOM);
	} else {
		setSide(TileInfo::SLOPED_TOP);
	}
}
int SlopeEditor::slope() const {
	return _slope;
}
int SlopeEditor::offset() const {
	return _offset;
}
int SlopeEditor::side() const {
	return _side;
}
void  SlopeEditor::setSlope(int i) {
	_slope = i;
	model->setData(model->index(row, TileInfo::FIELD_SLOPE), _slope);
	update();
}
void  SlopeEditor::setOffset(int i) {
	_offset = i;
	model->setData(model->index(row, TileInfo::FIELD_SLOPE_OFFSET), _offset);
	update();
}
void  SlopeEditor::setSide(int i) {
	_side = i;
	model->setData(model->index(row, TileInfo::FIELD_SLOPE_SIDE), _side);
	update();
}
void SlopeEditor::setModel(QAbstractItemModel* m) {
	model = m;
	connect(model, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(dataChanged(const QModelIndex&, const QModelIndex&)));
}
void SlopeEditor::setRow(int i) {
	row = i;
	setSlope(model->data(model->index(row, TileInfo::FIELD_SLOPE)).toInt());
	setOffset(model->data(model->index(row, TileInfo::FIELD_SLOPE_OFFSET)).toInt());
	setSide(model->data(model->index(row, TileInfo::FIELD_SLOPE_SIDE)).toInt());
}
void SlopeEditor::dataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight) {
	if(topLeft.row() <= row && bottomRight.row() >= row) {
		_slope = model->data(model->index(row, TileInfo::FIELD_SLOPE)).toInt();
		_offset = model->data(model->index(row, TileInfo::FIELD_SLOPE_OFFSET)).toInt();
		_side = model->data(model->index(row, TileInfo::FIELD_SLOPE_SIDE)).toInt();
	}
	update();
}
