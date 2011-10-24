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

#ifndef TILESETVIEW_HPP
#define TILESETVIEW_HPP
#include <QAbstractItemView>
class QPixmap;
class TileSetView: public QAbstractItemView {
	Q_OBJECT

	public:
		TileSetView(QWidget* parent = 0);
		QRect visualRect(const QModelIndex &index) const;
		void scrollTo(const QModelIndex &index, ScrollHint hint = EnsureVisible);
		QModelIndex indexAt(const QPoint &point) const;
		void setModel(QAbstractItemModel * model);
		void setItemDelegate(QAbstractItemDelegate * delegate);
		QSize maximumViewportSize();
		QSize sizeHint();

	public slots:
		void setZoom(int scale);
		void setWrap(int width);
		void setCurrentIndex ( const QModelIndex & index );

	signals:
		void tileSelected(int);

	protected slots:
		void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);
		void rowsInserted(const QModelIndex &parent, int start, int end);
		void rowsAboutToBeRemoved(const QModelIndex &parent, int start, int end);

	protected:
		bool edit(const QModelIndex &index, EditTrigger trigger, QEvent *event);
		QModelIndex moveCursor(QAbstractItemView::CursorAction cursorAction, Qt::KeyboardModifiers modifiers);
		int horizontalOffset() const;
		int verticalOffset() const;
		bool isIndexHidden(const QModelIndex &index) const;
		void setSelection(const QRect&, QItemSelectionModel::SelectionFlags command);
		void paintEvent(QPaintEvent *event);
		void resizeEvent(QResizeEvent *event);
		void scrollContentsBy(int dx, int dy);
		QRegion visualRegionForSelection(const QItemSelection &selection) const;

	private:
		void updateGeometries();
		void redrawBuffer();
		QRect bufferRect(const QModelIndex &) const;
		QPixmap buffer;
		int tileWidth, tileHeight;
		int gridWidth, gridHeight;
		int _scale;
		//bool _tilePaintMode;
};

#endif
