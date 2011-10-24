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

#include "RowToListProxyModel.hpp"
QModelIndex	RowToListProxyModel::mapFromSource(const QModelIndex & sourceIndex) const {
	if(sourceIndex.row() == _row) {
		return index(sourceIndex.column(), 0);
	}
	return QModelIndex();
}
QModelIndex	RowToListProxyModel::mapToSource(const QModelIndex & proxyIndex) const {
	return sourceModel()->index(_row, proxyIndex.row());
}
int RowToListProxyModel::rowCount(const QModelIndex & parent) const {
	return sourceModel()->columnCount();
}
int RowToListProxyModel::columnCount(const QModelIndex & parent) const {
	return 1;
}
void RowToListProxyModel::setRow(int i) {
	_row = i;
	emit dataChanged(index(0,0), index(rowCount() -1, 0));
}
QModelIndex RowToListProxyModel::index(int row, int column, const QModelIndex &parent) const {
	if(hasIndex(row, column, parent)) {
		return createIndex(row, column, 0);
	}
	return QModelIndex();
}
QModelIndex RowToListProxyModel::parent(const QModelIndex &child) const {
	return QModelIndex();
}
RowToListProxyModel::RowToListProxyModel(QObject* parent): QAbstractProxyModel(parent) {}
void RowToListProxyModel::setSourceModel(QAbstractItemModel* m) {
	if(sourceModel()) {
		disconnect(sourceModel(), SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &) ), this, SLOT(sourceDataChanged(const QModelIndex &, const QModelIndex &) ) );
	}
	QAbstractProxyModel::setSourceModel(m);
	connect(sourceModel(), SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &) ), this, SLOT(sourceDataChanged(const QModelIndex &, const QModelIndex &) ) );
	connect(sourceModel(), SIGNAL(columnsAboutToBeRemoved ( const QModelIndex & , int, int )), this, SLOT(sourceColumnsAboutToBeRemoved(const QModelIndex &, int, int) ) );
	connect(sourceModel(), SIGNAL(columnsRemoved(const QModelIndex &, int, int) ), this, SLOT(sourceColumnsRemoved(const QModelIndex &, int, int) ) );
	connect(sourceModel(), SIGNAL(columnsAboutToBeInserted(const QModelIndex &, int, int) ), this, SLOT(sourceColumnsAboutToBeInserted(const QModelIndex &, int, int) ) );
	connect(sourceModel(), SIGNAL(columnsInserted(const QModelIndex &, int, int) ), this, SLOT(sourceColumnsInserted(const QModelIndex &, int, int) ) );

}
void RowToListProxyModel::sourceDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight) {
	//since we have usually only a small subset of the model, it's easier to just update everything than work out the intersection.
	emit dataChanged(index(0,0), index(rowCount() -1, 0));
	//emit dataChanged(mapFromSource(topLeft), mapFromSource(bottomRight));
}

void RowToListProxyModel::sourceColumnsAboutToBeRemoved(const QModelIndex &parent, int start, int end) {
	beginRemoveRows(parent, start, end);
}
void RowToListProxyModel::sourceColumnsRemoved(const QModelIndex &parent, int start, int end) {
	endRemoveRows();
}
void RowToListProxyModel::sourceColumnsAboutToBeInserted(const QModelIndex &parent, int start, int end) {
	beginInsertRows(parent, start, end);
}
void RowToListProxyModel::sourceColumnsInserted(const QModelIndex &parent, int start, int end) {
	endInsertRows();
}

