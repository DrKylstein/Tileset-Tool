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

#ifndef SLOPEEDITOR_HPP
#define SLOPEEDITOR_HPP
#include <QWidget>
#include <QModelIndex>
#include <QLine>
class QAbstractItemModel;
class SlopeEditor: public QWidget {
		Q_OBJECT
		Q_PROPERTY(int slope READ slope WRITE setSlope)
		Q_PROPERTY(int offset READ offset WRITE setOffset)
		Q_PROPERTY(int side READ side WRITE setSide)


	public:
		SlopeEditor(QWidget* parent = 0);
		QSize sizeHint() const;
		int slope() const;
		int offset() const;
		int side() const;
		void setModel(QAbstractItemModel*);
		void setRow(int);

	public slots:
		void setSlope(int);
		void setOffset(int);
		void setSide(int);

	protected:
		void paintEvent(QPaintEvent*);
		void mousePressEvent(QMouseEvent*);

	private slots:
		void dataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight);

	private:
		int _slope, _offset, _side;
		QAbstractItemModel* model;
		int row;
};
#endif
