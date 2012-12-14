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

#include "TileSetView.hpp"
#include <QtGui>
#include "TileInfo.h"

void TileSetView::setModel(QAbstractItemModel * model) {
	QAbstractItemView::setModel(model);
	gridHeight = model->rowCount() / gridWidth;
	redrawBuffer();
}
void TileSetView::setItemDelegate(QAbstractItemDelegate * delegate) {
	QAbstractItemView::setItemDelegate(delegate);
	redrawBuffer();
}
void TileSetView::setWrap(int width) {
	gridWidth = width;
	if(model()) {
		gridHeight = model()->rowCount() / gridWidth;
	} else {
		gridHeight = 1;
	}
	redrawBuffer();
}
void TileSetView::setZoom(int scale) {
	_scale = scale;
	redrawBuffer();
}
void TileSetView::redrawBuffer() {
	if(!model()) return;
	if(gridWidth * tileWidth * _scale == 0 || gridHeight * tileHeight * _scale == 0) return;
	buffer = QPixmap(gridWidth * tileWidth * _scale, gridHeight * tileHeight * _scale);
	buffer.fill(QColor(0,0,0,0));
	QItemSelectionModel *selections = selectionModel();
	QStyleOptionViewItem option = viewOptions();
	QStyle::State state = option.state;
	QPainter painter(&buffer);
	for(int i = 0; i < model()->rowCount(); i++) {
		QModelIndex ix = model()->index(i, 0, rootIndex());
		option.rect = bufferRect(ix);
		itemDelegate()->paint(&painter, option, ix);
	}
	updateGeometries();
}
TileSetView::TileSetView(QWidget* parent): QAbstractItemView(parent) {
	horizontalScrollBar()->setRange(0,0);
	verticalScrollBar()->setRange(0,0);
	tileWidth = tileHeight = 16;
	gridWidth = 13;
	gridHeight = 1;
	_scale = 2;
	setSelectionMode(NoSelection);
	//setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum));
	//setMaximumSize(maximumViewportSize());
}
void TileSetView::scrollTo(const QModelIndex &index, ScrollHint hint) {
	QRect r = visualRect(index);
	int dx, dy;
	if(r.left() < 0) {
		dx = r.left();
	} else {
		dx = r.right() - viewport()->width();
		if(dx < 0) dx = 0;
	}
	if(r.top() < 0) {
		dy = r.top();
	} else {
		dy = r.bottom() - viewport()->height();
		if(dy < 0) dy = 0;
	}

	horizontalScrollBar()->setValue(horizontalOffset() + dx);
	verticalScrollBar()->setValue(verticalOffset() + dy);
}
QModelIndex TileSetView::indexAt(const QPoint &point) const {
	if(!model()) {
		return QModelIndex();
	}
	int tilex = point.x() + horizontalScrollBar()->value();
	int tiley = point.y() + verticalScrollBar()->value();
	tilex /= (tileWidth * _scale);
	tiley /= (tileHeight * _scale);
	int i = (tiley * gridWidth) + tilex;
	return model()->index(i, 0);
}
void TileSetView::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight) {
	QAbstractItemView::dataChanged(topLeft, bottomRight);
	if(!buffer.isNull()) {
		QPainter painter(&buffer);
		QStyleOptionViewItem option = viewOptions();
		//QItemSelectionModel *selections = selectionModel();
		//QStyle::State state = option.state;
		for(int i = topLeft.row(); i <= bottomRight.row(); i++) {
			QModelIndex ix = model()->index(i, 0, rootIndex());
			option.rect = bufferRect(ix);
			itemDelegate()->paint(&painter, option, ix);
		}
		viewport()->update();
	}
}
void TileSetView::rowsInserted(const QModelIndex &parent, int start, int end) {
	gridHeight = model()->rowCount() / gridWidth;
	redrawBuffer();
	//qDebug() << "rowsInserted!";
}
void TileSetView::rowsAboutToBeRemoved(const QModelIndex &parent, int start, int end) {
	gridHeight -= ((end - start) / gridWidth);
	redrawBuffer();
	//qDebug() << "rowsRemoved!";
}

bool TileSetView::edit(const QModelIndex &index, EditTrigger trigger, QEvent *event) {
	return QAbstractItemView::edit(index, trigger, event);
}
void TileSetView::setCurrentIndex ( const QModelIndex & index ) {
	QAbstractItemView::setCurrentIndex(index);
}
QModelIndex TileSetView::moveCursor(QAbstractItemView::CursorAction cursorAction, Qt::KeyboardModifiers modifiers) {
	if(!model()) {
		return QModelIndex();
	}
	int section = currentIndex().row();
	switch(cursorAction) {
		case QAbstractItemView::MoveUp:
			section -= gridWidth;
			break;
		case QAbstractItemView::MoveDown:
			section += gridWidth;
			break;
		case QAbstractItemView::MoveLeft:
			section--;
			break;
		case QAbstractItemView::MoveRight:
			section++;
			break;
		case QAbstractItemView::MoveHome:
			section -= section % gridWidth;
			break;
		case QAbstractItemView::MoveEnd:
			section += gridWidth - (section % gridWidth) - 1;
			break;
		default:
			break;
	}
	if(section < 0) section = 0;
	if(section > model()->rowCount()) section = model()->rowCount();
	viewport()->update();
	return model()->index(section, 0);
}
int TileSetView::horizontalOffset() const {
	return horizontalScrollBar()->value();
}
int TileSetView::verticalOffset() const {
	return verticalScrollBar()->value();
}
bool TileSetView::isIndexHidden(const QModelIndex &index) const {
	return false;
}
void TileSetView::paintEvent(QPaintEvent *event) {
	QPainter painter(viewport());
	QRect r(horizontalScrollBar()->value() + event->rect().left(), verticalScrollBar()->value() + event->rect().top(), event->rect().width(), event->rect().height());

	painter.fillRect(event->rect(), QApplication::palette().dark());
	painter.drawPixmap(event->rect(), buffer, r);

	QStyleOptionViewItem option = viewOptions();
	option.rect = visualRect(currentIndex());
	option.state |= QStyle::State_HasFocus;
	itemDelegate()->paint(&painter, option, currentIndex());

//	QItemSelectionModel *selections = selectionModel();
//	QStyleOptionViewItem option = viewOptions();
//	QStyle::State state = option.state;
//	int j;
//	for(int i = 0; i < model()->rowCount(); i++) {
//		QModelIndex labelIndex = model()->index(i, 0, rootIndex());
//		QStyleOptionViewItem option = viewOptions();
//		option.rect = visualRect(labelIndex);
//		if(option.rect.intersects(event->rect())) {
//			itemDelegate()->paint(&painter, option, model()->index(i, 0, rootIndex()));
//		}
//	}
}
void TileSetView::resizeEvent(QResizeEvent *event) {
	updateGeometries();
}
void TileSetView::scrollContentsBy(int dx, int dy) {
	viewport()->scroll(dx, dy);
	viewport()->update();
}
QRegion TileSetView::visualRegionForSelection(const QItemSelection &selection) const {
	return QRegion();
}

void TileSetView::updateGeometries() {
	horizontalScrollBar()->setPageStep(viewport()->width());
	horizontalScrollBar()->setRange(0, qMax(0, buffer.width() - viewport()->width() ) );
	verticalScrollBar()->setPageStep(viewport()->height());
	verticalScrollBar()->setRange(0, qMax(0, buffer.height() - viewport()->height() ) );
	horizontalScrollBar()->setSingleStep(tileWidth * _scale);
	verticalScrollBar()->setSingleStep(tileHeight * _scale);
	viewport()->update();
}
void TileSetView::mouseDoubleClickEvent (QMouseEvent * event) {
	emit activated(indexAt(QPoint(event->x(), event->y())));
}

QRect TileSetView::visualRect(const QModelIndex &index) const {
	if(!index.isValid()) return QRect();
	int x, y;
	x = index.row() % gridWidth;
	y = index.row() / gridWidth;
	x *= tileWidth * _scale;
	y *= tileHeight * _scale;
	x -= horizontalScrollBar()->value();
	y -= verticalScrollBar()->value();
	return QRect(x, y, tileWidth * _scale, tileHeight * _scale);
}
QRect TileSetView::bufferRect(const QModelIndex &index) const {
	if(!index.isValid()) return QRect();
	int x, y;
	x = index.row() % gridWidth;
	y = index.row() / gridWidth;
	x *= tileWidth * _scale;
	y *= tileHeight * _scale;
	return QRect(x, y, tileWidth * _scale, tileHeight * _scale);
}
void TileSetView::setSelection (const QRect & rect, QItemSelectionModel::SelectionFlags flags) {
	//QAbstractItemView::setSelection(rect, flags);
	/*QPainter painter(&buffer);
	QStyleOptionViewItem option = viewOptions();
	option.rect = bufferRect(currentIndex());
	option.state |= QStyle::State_HasFocus;
	itemDelegate()->paint(&painter, option, currentIndex());*/
	viewport()->update();

	emit tileSelected(currentIndex().row());

}
QSize TileSetView::maximumViewportSize() {
	return QSize(gridWidth * tileWidth * _scale, gridHeight * tileHeight * _scale);
}
QSize TileSetView::sizeHint() {
	return maximumViewportSize();
}
QPixmap TileSetView::dumpView() {
    int scale = _scale;
    _scale = 1;
    redrawBuffer();
    QPixmap dump = buffer.copy();
    _scale = scale;
    redrawBuffer();
    return dump;
}
