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

#ifndef ANIMATIONPREVIEW_HPP
#define ANIMATIONPREVIEW_HPP
#include <QWidget>
#include <QPixmap>
#include <QModelIndex>
class AnimationPreview: public QWidget {
	Q_OBJECT

	public:
		AnimationPreview(QWidget* parent = 0);
		void setModel(QAbstractItemModel*);
		void setGfxModel(QAbstractItemModel*);
		QSize sizeHint() const;

	public slots:
		void stop();
		void start();
		void advanceFrame();
		void previousFrame();
		void goToStart();
		void goToEnd();
		void setFrame(int);
		void setRow(int);
		void setZoom(int);

	signals:
		void frameChanged(int);

	protected:
		void paintEvent(QPaintEvent*);

	private slots:
		void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);

	private:
		QTimer* _timer;
		QAbstractItemModel* _model;
		QAbstractItemModel* _gfxModel;
		int _frame;
		QPixmap _pic;
		int _row;
		QSize _size;
};
#endif
