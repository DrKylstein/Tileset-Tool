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

#ifndef FRAMEPICKERDIALOG_HPP
#define FRAMEPICKERDIALOG_HPP
#include <QDialog>
class TileSetView;
class QAbstractItemModel;
class QAbstractItemDelegate;;
class FramePickerDialog: public QDialog {
		Q_OBJECT
	public:
		FramePickerDialog(QWidget * parent = 0, Qt::WindowFlags f = 0);
		void setModel(QAbstractItemModel* model);
		int selectedFrame(void);
		void setSelectedFrame(int);

	public slots:
		void setZoom(int);
		void setWrap(int);

	protected:
		void showEvent(QShowEvent * event);

	private slots:
		void frameSelected(int);

	private:
		TileSetView* _view;
		QAbstractItemModel* _model;
		QAbstractItemDelegate* _delegate;
		int _selectedFrame;
};
#endif

