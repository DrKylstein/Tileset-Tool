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

#ifndef ROWTOLISTPROXYMODEL_HPP
#define ROWTOLISTPROXYMODEL_HPP
#include <QAbstractProxyModel>
class RowToListProxyModel: public QAbstractProxyModel {
		Q_OBJECT

	public:
		RowToListProxyModel(QObject* parent = 0);
		QModelIndex	mapFromSource(const QModelIndex & sourceIndex) const;
		QModelIndex	mapToSource(const QModelIndex & proxyIndex) const;
		int rowCount(const QModelIndex & parent = QModelIndex()) const;
		int columnCount(const QModelIndex & parent = QModelIndex()) const;
		void setRow(int i);
		QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
		QModelIndex parent(const QModelIndex &child) const;
		void setSourceModel(QAbstractItemModel* m);

	private slots:
		void sourceDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);
		void sourceColumnsAboutToBeRemoved(const QModelIndex &parent, int start, int end);
		void sourceColumnsRemoved(const QModelIndex &parent, int start, int end);
		void sourceColumnsAboutToBeInserted(const QModelIndex &parent, int start, int end);
		void sourceColumnsInserted(const QModelIndex &parent, int start, int end);

	private:
		int _row;
};
#endif
