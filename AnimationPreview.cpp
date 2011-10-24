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

#include "AnimationPreview.hpp"
#include <QtGui>
#include <QTimer>
//include "TileInfo.h"
AnimationPreview::AnimationPreview(QWidget* parent): QWidget(parent) {
	setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed, QSizePolicy::Frame));
	_timer = new QTimer(this);
	connect(_timer, SIGNAL(timeout()), this, SLOT(advanceFrame()));
	_size = QSize(16, 16);
}
QSize AnimationPreview::sizeHint() const {
	return _size;
}
void AnimationPreview::setModel(QAbstractItemModel* model) {
	_model = model;
	connect(model, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(dataChanged(const QModelIndex&, const QModelIndex&)));
}
void AnimationPreview::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight) {
	_pic = _gfxModel->data(_gfxModel->index(_model->data(_model->index(_row, _frame) ).toInt(), 0) ).value<QPixmap>();
	update();
}
void AnimationPreview::setZoom(int i) {
	_size = QSize(i * 16, i * 16);
	resize(_size);
	setMinimumSize(_size);
}
void AnimationPreview::setGfxModel(QAbstractItemModel* m) {
	_gfxModel = m;
}
void AnimationPreview::setRow(int i) {
	if(!_gfxModel || !_model) return;
	_row = i;
	_frame = 0;
	_pic = _gfxModel->data(_gfxModel->index(_model->data(_model->index(_row, _frame) ).toInt(), 0) ).value<QPixmap>();
	update();
}
void AnimationPreview::stop() {
	_timer->stop();
}
void AnimationPreview::start() {
	_timer->start(1000 / (_model->columnCount()));
}
void AnimationPreview::advanceFrame() {
	_frame += 1;
	if(_frame >= _model->columnCount()) {
		_frame = 0;
	}
	_pic = _gfxModel->data(_gfxModel->index(_model->data(_model->index(_row, _frame) ).toInt(), 0) ).value<QPixmap>();
	update();
	emit frameChanged(_frame);
}
void AnimationPreview::previousFrame() {
	_frame -= 1;
	if(_frame < 0) {
		_frame = _model->columnCount() - 1;
	}
	_pic = _gfxModel->data(_gfxModel->index(_model->data(_model->index(_row, _frame) ).toInt(), 0) ).value<QPixmap>();
	update();
	emit frameChanged(_frame);
}
void AnimationPreview::goToStart(){
	_frame = 0;
	_pic = _gfxModel->data(_gfxModel->index(_model->data(_model->index(_row, _frame) ).toInt(), 0) ).value<QPixmap>();
	update();
	emit frameChanged(_frame);
}
void AnimationPreview::goToEnd(){
	_frame = _model->columnCount() - 1;
	_pic = _gfxModel->data(_gfxModel->index(_model->data(_model->index(_row, _frame) ).toInt(), 0) ).value<QPixmap>();
	update();
	emit frameChanged(_frame);
}
void AnimationPreview::setFrame(int i){
	_frame = i;
	if(_frame >= _model->columnCount()) {
		_frame = _model->columnCount() - 1;
	}
	if(_frame < 0) {
		_frame = 0;
	}
	_pic = _gfxModel->data(_gfxModel->index(_model->data(_model->index(_row, _frame) ).toInt(), 0) ).value<QPixmap>();
	update();
	// do not emit change signal. The caller is responsible for sync.
}

void AnimationPreview::paintEvent(QPaintEvent* event) {
	QPainter painter(this);
	painter.drawPixmap(event->rect(), _pic, _pic.rect());
}
