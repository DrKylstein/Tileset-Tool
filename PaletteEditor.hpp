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

#ifndef PALETTEEDITOR_HPP
#include <QWidget>
#include <QModelIndex>
#include <QColor>
class QSlider;
class QAbstractItemModel;
class QTableView;
class QDataWidgetMapper;
class PaletteEditor: public QWidget {
	Q_OBJECT

	public:
		PaletteEditor(QWidget* parent = 0);
		void setModel(QAbstractItemModel*);

	//signals:
		//void colorChanged(int column, int row, QColor color);

	private slots:
		void chooseColor(const QModelIndex&);

	private:
		QAbstractItemModel* _model;
		QDataWidgetMapper* _mapper;
		QTableView* _table;
};

#define PALETTEEDITOR_HPP
#endif
