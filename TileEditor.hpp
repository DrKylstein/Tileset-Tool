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

#ifndef TILEEDITOR_HPP
#define TILEEDITOR_HPP
#include <QWidget>
#include <QModelIndex>
#include <QVariant>
#include <QList>
class QDataWidgetMapper;
class QAbstractItemModel;
class QComboBox;
class QCheckBox;
class QShortcut;

class SlopeEditor;

class TileEditor: public QWidget {
	Q_OBJECT

	public:
		TileEditor(QWidget* parent = 0);
		void setModel(QAbstractItemModel * model);
		void resetCurrentIndex();

	public slots:
		void setCurrentModelIndex(const QModelIndex & index);

	private slots:
		void _focusChange(QWidget*, QWidget*);
		void _slopeEnableChanged(int);
		void _runChanged(int);
		void toggleTop();
		void toggleLeft();
		void toggleBottom();
		void toggleRight();
		void setNormalType();
		void cycleKeyTypes();
		void cycleDoorTypes();
		void setDeadlyType();
		void setMasked();
		void setForeground();

	private:
		QDataWidgetMapper* mapper;
		QComboBox* behavior;
		QComboBox* surfaceType;
		QComboBox* bottomType;
		QCheckBox* topBlocking;
		QCheckBox* bottomBlocking;
		QCheckBox* leftBlocking;
		QCheckBox* rightBlocking;
		QComboBox* slopeEnable;
		SlopeEditor* topSlope;

		QShortcut* toggleTopAction;
		QShortcut* toggleLeftAction;
		QShortcut* toggleBottomAction;
		QShortcut* toggleRightAction;
		QShortcut* setNormalTypeAction;
		QShortcut* cycleKeyTypesAction;
		QShortcut* cycleDoorTypesAction;
		QShortcut* setDeadlyTypeAction;
		QShortcut* setMaskedAction;
		QShortcut* setForegroundAction;

		QAbstractItemModel* _model;
		QModelIndex _index;

		void _updateSlopeBlocking();

};
#endif
