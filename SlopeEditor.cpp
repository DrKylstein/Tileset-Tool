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
	QLine line = QLine(start, end);
	QPolygon p;
	painter.setRenderHint(QPainter::Antialiasing);
	painter.fillRect(0, 0, width(), height(), pal.color(QPalette::Base));
	QBrush brush = pal.highlight();
	painter.setBrush(brush);
	painter.setPen(Qt::NoPen);
	if(!_slopeOnBottom) {
		p.setPoints(4, line.x1(), line.y1(), line.x2() + 1, line.y2(), event->rect().right() + 1, event->rect().bottom() + 1, event->rect().left(), event->rect().bottom() + 1);
		painter.drawPolygon(p);
	} else {
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

	switch(_dragMode) {
		case DRAG_LEFT:
			_left = result;
			break;
		case DRAG_RIGHT:
			_right = result;
			break;
	}
	_translateAndSubmit();
}

void SlopeEditor::mouseReleaseEvent(QMouseEvent* event) {
	if(_dragMode == NOT_DRAGGING) {
		if(event->y() < height()/4) {
			_slopeOnBottom = false;
		} else if(event->y() > height() - (height()/4)) {
			_slopeOnBottom = true;
		}
		_translateAndSubmit();
	}
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
	update();
}
void SlopeEditor::dataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight) {
	if(topLeft.row() <= _row && bottomRight.row() >= _row) {
		_translateAndLoad();
	}
	update();
}

void SlopeEditor::_translateAndLoad() {
	_slopeOnBottom = model->data(model->index(_row, TileInfoModel::FIELD_SLOPED+TileInfoModel::BOTTOM)).toBool();
	TileInfoModel* infoModel = (TileInfoModel*)(model);
	int run, y0;
	if(_slopeOnBottom) {
		run = infoModel->tile(_row).run[TileInfoModel::BOTTOM];
		y0 = infoModel->tile(_row).y0[TileInfoModel::BOTTOM];
	} else {
		run = infoModel->tile(_row).run[TileInfoModel::TOP];
		y0 = infoModel->tile(_row).y0[TileInfoModel::TOP];
	}
	_right =_left = y0;
	if(run != 0) {
		_right += 16 / run;
	}
}

void SlopeEditor::_translateAndSubmit() {
	int run, y0;
	y0 = _left;
	run = 0;
	if(_right-_left != 0) {
		run = 16 / (_right-_left);
	}
	if(run == 0) {
		return;
	}
	TileInfoModel* infoModel = (TileInfoModel*)model;
	TileInfoModel::TileProperties* tile = &infoModel->tile(_row);
	if(_slopeOnBottom) {
		//unset top
		tile->sloped[TileInfoModel::TOP] = false;
		tile->run[TileInfoModel::TOP] = 0;
		tile->y0[TileInfoModel::TOP] = 0;
		//set bottom
		tile->sloped[TileInfoModel::BOTTOM] = true;
		tile->run[TileInfoModel::BOTTOM] = run;
		tile->y0[TileInfoModel::BOTTOM] = y0;
		//set paired side, clear opposite side
		if(run > 0) { //positive bottom slope -> down to the left, match on right
			tile->sloped[TileInfoModel::LEFT] = false;
			tile->run[TileInfoModel::LEFT] = 0;
			tile->y0[TileInfoModel::LEFT] = 0;

			tile->sloped[TileInfoModel::RIGHT] = true;
			tile->run[TileInfoModel::RIGHT] = run;
			tile->y0[TileInfoModel::RIGHT] = y0;
		} else { //negative -> down to the right, match on left
			tile->sloped[TileInfoModel::LEFT] = true;
			tile->run[TileInfoModel::LEFT] = run;
			tile->y0[TileInfoModel::LEFT] = y0;

			tile->sloped[TileInfoModel::RIGHT] = false;
			tile->run[TileInfoModel::RIGHT] = 0;
			tile->y0[TileInfoModel::RIGHT] = 0;
		}
	} else {
		//set top
		tile->sloped[TileInfoModel::TOP] = true;
		tile->run[TileInfoModel::TOP] = run;
		tile->y0[TileInfoModel::TOP] = y0;
		//unset bottom
		tile->sloped[TileInfoModel::BOTTOM] = false;
		tile->run[TileInfoModel::BOTTOM] = 0;
		tile->y0[TileInfoModel::BOTTOM] = 0;
		//clear sides
		tile->sloped[TileInfoModel::LEFT] = false;
		tile->run[TileInfoModel::LEFT] = 0;
		tile->y0[TileInfoModel::LEFT] = 0;
		tile->sloped[TileInfoModel::RIGHT] = false;
		tile->run[TileInfoModel::RIGHT] = 0;
		tile->y0[TileInfoModel::RIGHT] = 0;
	}
	infoModel->markTileUpdated(_row);
}
