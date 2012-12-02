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

#ifndef ANIMEDITOR_HPP
#define ANIMEDITOR_HPP
#include <QWidget>
#include <QModelIndex>
#include <QVariant>
#include <QVector>
class QAbstractItemModel;
class QComboBox;
class QPushButton;
class AnimationPreview;
class FramePickerDialog;
class RowToListProxyModel;
class TileSetView;
class TileMapDelegate;
class QToolButton;
class AnimationEditor: public QWidget {
	Q_OBJECT

	public:
		AnimationEditor(QWidget* parent = 0);
		void setModel(QAbstractItemModel * model);
		void setGfxModel(QAbstractItemModel * model);
		void resetCurrentIndex();
		FramePickerDialog* framePicker();

	public slots:
		void setCurrentModelIndex(const QModelIndex & index);
		void applyPreset();
		void setZoom(int);

	signals:
		void frameEdited(const QModelIndex&, const QVariant&);
		void presetApplied(int section, const QVector<int>& frames);
		void frameCountChange(int frames);

	private slots:
		void pickFrame(const QModelIndex&);
		void frameSelected(int);
		void togglePlayback();
		void frameChange(int);

	private:
		QComboBox* animPreset;
		QPushButton* _applyPreset;
		QToolButton* _play;
		QToolButton* _start;
		QToolButton* _previous;
		QToolButton* _next;
		QToolButton* _end;
		AnimationPreview* anim;

		TileSetView* _framesView;
		RowToListProxyModel* _framesModel;
		TileMapDelegate* _framesDelegate;

		QAbstractItemModel* _model;
		QAbstractItemModel* _gfxModel;
		int _currentRow;
		int _frames;
		bool _playing;

		FramePickerDialog* _framePicker;
};
#endif
