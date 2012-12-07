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
#include "TileInfoModel.hpp"
SlopeEditor::SlopeEditor(QWidget* parent): QWidget(parent), _left(0), _right(0), _row(0), _slopeOnBottom(false), _dragMode(SlopeEditor::NOT_DRAGGING) {
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
	QPoint start(0,0), end(event->rect().right(),0);
	start.setY(_left * scale);
	end.setY(_right * scale);
	//qDebug() << (_slope != 0 ? event->rect().height()/_slope : 0);
	QPolygon p;
	painter.setRenderHint(QPainter::Antialiasing);
	painter.fillRect(0, 0, width(), height(), pal.color(QPalette::Base));
	QPen slopePen;
	slopePen.setColor(pal.color(QPalette::Text));
	slopePen.setStyle(Qt::SolidLine);
	slopePen.setWidth(1);
	QBrush brush = pal.highlight();
	painter.setBrush(brush);
	painter.setPen(slopePen);
	if(!_slopeOnBottom) {
		p.setPoints(4, start.x(), start.y(), end.x() + 1, end.y(), event->rect().right() + 1, event->rect().bottom() + 1, event->rect().left(), event->rect().bottom() + 1);
		painter.drawPolygon(p);
	} else {
		p.setPoints(4, start.x(), start.y(), end.x() + 1, end.y(), event->rect().right() + 1, event->rect().top(), event->rect().left(), event->rect().top());
		painter.drawPolygon(p);
	}
	//handles
	painter.setBrush(Qt::NoBrush);
	start.rx() += width()/16;
	end.rx() -= width()/16;
	if(start.y() == height()) {
		start.ry() -= width()/16;
	}
	if(start.y() == 0) {
		start.ry() += width()/16;
	}
	if(end.y() == height()) {
		end.ry() -= width()/16;
	}
	if(end.y() == 0) {
		end.ry() += width()/16;
	}
	painter.drawEllipse(start, width()/16, width()/16);
	painter.drawEllipse(end, width()/16, width()/16);
}

void SlopeEditor::mouseMoveEvent(QMouseEvent* event) {
	int scale = width() / 16;
	int scaled_y = (event->y() / scale);
	if(scaled_y > 16) {
		scaled_y = 16;
	}
	if(scaled_y < 0) {
		scaled_y = 0;
	}
	int result = scaled_y & 0x1C;

	int run = 0;

	switch(_dragMode) {
		case DRAG_LEFT:
			if(_right-result != 0) {
				_left = result;
				model->setData(model->index(_row, TileInfoModel::FIELD_Y0), _left);
				emit y0Changed(_left);
				run = 16 / (_right-_left);
				model->setData(model->index(_row, TileInfoModel::FIELD_RUN), run);
				emit runChanged(run);
			}
			break;
		case DRAG_RIGHT:
			if(result-_left != 0) {
				_right = result;
				run = 16 / (_right-_left);
				model->setData(model->index(_row, TileInfoModel::FIELD_RUN), run);
				emit runChanged(run);
			}
			break;
	}
	update();
}

void SlopeEditor::mouseReleaseEvent(QMouseEvent* event) {
	_dragMode = NOT_DRAGGING;
	setMouseTracking(false);
}

void SlopeEditor::mousePressEvent(QMouseEvent* event) {
	if(event->x() < width()/4) {
		_dragMode = DRAG_LEFT;
		setMouseTracking(true);
	} else if(event->x() > width() - (width()/4)) {
		_dragMode = DRAG_RIGHT;
		setMouseTracking(true);
	}
}
void SlopeEditor::setModel(QAbstractItemModel* m) {
	model = m;
	connect(model, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(dataChanged(const QModelIndex&, const QModelIndex&)));
}
void SlopeEditor::setRow(int i) {
	_row = i;
	_translateAndLoad();
}
void SlopeEditor::dataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight) {
	if(topLeft.row() <= _row && bottomRight.row() >= _row) {
		_translateAndLoad();
	}
}

void SlopeEditor::_translateAndLoad() {
	_slopeOnBottom = model->data(model->index(_row, TileInfoModel::FIELD_SLOPED_SIDE)).toInt() == 2;
	_right =_left = model->data(model->index(_row, TileInfoModel::FIELD_Y0)).toInt();
	_right += 16 / model->data(model->index(_row, TileInfoModel::FIELD_RUN)).toInt();
	update();
}
