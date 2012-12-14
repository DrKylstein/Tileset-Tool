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

#include "AnimationEditor.hpp"
#include <QtGui>
#include "TileInfo.h"
#include "AnimationPreview.hpp"
#include "FramePickerDialog.hpp"
#include "TileSetView.hpp"
#include "RowToListProxyModel.hpp"
#include "TileMapDelegate.hpp"
void AnimationEditor::resetCurrentIndex() {
	setCurrentModelIndex(_model->index(0,0) );
}
AnimationEditor::AnimationEditor(QWidget* parent): QWidget(parent) {
	_framesModel = new RowToListProxyModel(this);
	_framesView = new TileSetView();
	_framesDelegate = new TileMapDelegate(this);

	_framePicker = new FramePickerDialog(this);
	_framePicker->setWrap(13);

	QVBoxLayout* layout = new QVBoxLayout;
	setLayout(layout);
		QHBoxLayout* framesLayout = new QHBoxLayout;
		layout->addLayout(framesLayout);
			QFrame* animFrame = new QFrame;
			animFrame->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
			framesLayout->addWidget(animFrame, 0, Qt::AlignHCenter);
				QHBoxLayout* animFrameLayout = new QHBoxLayout;
				animFrame->setLayout(animFrameLayout);
				animFrameLayout->setContentsMargins(0, 0, 0, 0);
				anim = new AnimationPreview;
				animFrameLayout->addWidget(anim);
				//animFrame->setMaximumSize(QSize(64, 64));
			framesLayout->addWidget(_framesView);
		QHBoxLayout* controlsLayout = new QHBoxLayout;
		layout->addLayout(controlsLayout);
			_start = new QToolButton();
				controlsLayout->addWidget(_start);
				_start->setIcon(QIcon(":/images/start.png"));
				connect(_start, SIGNAL(clicked()), anim, SLOT(goToStart()));
			_previous = new QToolButton();
				controlsLayout->addWidget(_previous);
				_previous->setIcon(QIcon(":/images/previous.png"));
				connect(_previous, SIGNAL(clicked()), anim, SLOT(previousFrame()));
			_play = new QToolButton();
				controlsLayout->addWidget(_play);
				_play->setIcon(QIcon(":/images/play.png"));
				connect(_play, SIGNAL(clicked()), this, SLOT(togglePlayback()));
			_next = new QToolButton();
				controlsLayout->addWidget(_next);
				_next->setIcon(QIcon(":/images/next.png"));
				connect(_next, SIGNAL(clicked()), anim, SLOT(advanceFrame()));
			_end = new QToolButton();
				controlsLayout->addWidget(_end);
				_end->setIcon(QIcon(":/images/end.png"));
				connect(_end, SIGNAL(clicked()), anim, SLOT(goToEnd()));
			animPreset = new QComboBox;
				controlsLayout->addWidget(animPreset);
				animPreset->addItem(tr("No Animation"));
				animPreset->addItem(tr("Two-Frame Cycle"));
				animPreset->addItem(tr("Four-Frame Cycle"));
				animPreset->addItem(tr("Eight-Frame Cycle"));
				animPreset->addItem(tr("Three-Frame Oscilation"));
				animPreset->addItem(tr("Five-Frame Oscilation"));
			_applyPreset = new QPushButton(tr("Apply"));
				controlsLayout->addWidget(_applyPreset);
				connect(_applyPreset, SIGNAL(released(void)), this, SLOT(applyPreset(void)));

			connect(_framesView, SIGNAL(doubleClicked(const QModelIndex&) ), this, SLOT(pickFrame(const QModelIndex&) ) );
			connect(anim, SIGNAL(frameChanged(int)), this, SLOT(frameChange(int)));
			connect(_framesView, SIGNAL(tileSelected(int)), this, SLOT(frameSelected(int)));

	_presetShortcut = new QShortcut(QKeySequence(tr("C")), this);
	_presetShortcut->setContext(Qt::ApplicationShortcut);
	connect(_presetShortcut, SIGNAL(activated()), this, SLOT(applyPreset()));
}
void AnimationEditor::applyPreset() {
	QModelIndex baseFrame = _model->index(_currentRow, 0);
	QVector<int> newFrames;
	switch(animPreset->currentIndex()) {
		case 0:
			for(int i = 0; i != 8; ++i) {
				newFrames << baseFrame.data().toInt();
			}
			break;
		case 1:
			for(int i = 0; i != 8; ++i) {
				newFrames << baseFrame.data().toInt() + (i & 1);
			}
			break;
		case 2:
			for(int i = 0; i != 8; ++i) {
				newFrames << baseFrame.data().toInt() + (i & 3);
			}
			break;
		case 3:
			for(int i = 0; i != 8; ++i) {
				newFrames << baseFrame.data().toInt() + i;
			}
			break;
		case 4:
			newFrames << baseFrame.data().toInt();
			newFrames << baseFrame.data().toInt()+1;
			newFrames << baseFrame.data().toInt()+2;
			newFrames << baseFrame.data().toInt()+1;
			newFrames << baseFrame.data().toInt();
			newFrames << baseFrame.data().toInt()+1;
			newFrames << baseFrame.data().toInt()+2;
			newFrames << baseFrame.data().toInt()+1;
			break;
		case 5:
			for(int i = 0; i != 8; ++i) {
				if(i < 5) {
					newFrames << baseFrame.data().toInt() + i;
				} else {
					newFrames << baseFrame.data().toInt() + 8 - i;
				}
			}
			break;
	}
	emit presetApplied(_currentRow, newFrames);
}

void AnimationEditor::setModel(QAbstractItemModel * model) {
	_model = model;
	anim->setModel(_model);
	_framesModel->setSourceModel(_model);
	_framesModel->setRow(0);
	_framesView->setModel(_framesModel);
	_framesView->setWrap(_framesModel->rowCount());
	_framesView->setCurrentIndex(_framesModel->index(0,0));
}
void AnimationEditor::setGfxModel(QAbstractItemModel * model) {
	_gfxModel = model;
	anim->setGfxModel(_gfxModel);
	_framePicker->setModel(_gfxModel);
	_framesDelegate->setGfxModel(_gfxModel);
	_framesView->setItemDelegate(_framesDelegate);
}
void AnimationEditor::setCurrentModelIndex(const QModelIndex & index) {
	_currentRow = index.row();
	_framesModel->setRow(_currentRow);
	anim->setRow(_currentRow);
}
void AnimationEditor::setZoom(int i){
	_framesView->setZoom(i);
	anim->setZoom(i);
}
void AnimationEditor::frameSelected(int i) {
	anim->setFrame(i);
}
void AnimationEditor::pickFrame(const QModelIndex& index) {
	_framePicker->setSelectedFrame(index.data().toInt());
	_framePicker->exec();
	emit frameEdited(_framesModel->mapToSource(index), _framePicker->selectedFrame());//_framesModel->setData(index, _framePicker->selectedFrame());
}
void AnimationEditor::pickTile(const QModelIndex& index) {
	_framePicker->setSelectedFrame(index.data().toInt());
	_framePicker->exec();
	for(int i = 0; i < 8; ++i) {
		emit frameEdited(_model->index(_framesModel->mapToSource(index).row(), i), _framePicker->selectedFrame());
	}
}
void AnimationEditor::togglePlayback() {
	if(!_playing) {
		_playing = true;
		_play->setIcon(QIcon(":/images/pause.png"));
		anim->start();
	} else {
		_playing = false;
		_play->setIcon(QIcon(":/images/play.png"));
		anim->stop();
	}
}
void AnimationEditor::frameChange(int i) {
	_framesView->setCurrentIndex(_framesModel->index(i, 0));
}
FramePickerDialog* AnimationEditor::framePicker() {
	return _framePicker;
}
