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

#include "TileEditor.hpp"
#include <QtGui>
#include "SlopeEditor.hpp"
#include "AnimationPreview.hpp"
#include "TileInfoModel.hpp"
void TileEditor::resetCurrentIndex() {
	mapper->toFirst();
}
TileEditor::TileEditor(QWidget* parent): QWidget(parent) {
	mapper = new QDataWidgetMapper(this);
	QVBoxLayout* layout = new QVBoxLayout;
		setLayout(layout);
		QGroupBox* collision = new QGroupBox(tr("Collision"));
			layout->addWidget(collision);
			QGridLayout* currentTileGrid = new QGridLayout;
				collision->setLayout(currentTileGrid);
				QVBoxLayout* centerLayout = new QVBoxLayout;
					currentTileGrid->addLayout(centerLayout, 1, 1);
					QFrame* slopeFrame = new QFrame;
						slopeFrame->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
						slopeFrame->setMaximumWidth(64);
						centerLayout->addWidget(slopeFrame, 0, Qt::AlignHCenter);
						QHBoxLayout* slopeFrameLayout = new QHBoxLayout;
						slopeFrame->setLayout(slopeFrameLayout);
						slopeFrameLayout->setContentsMargins(0, 0, 0, 0);
						topSlope = new SlopeEditor;
							slopeFrameLayout->addWidget(topSlope);
							topSlope->setEnabled(false);
							connect(topSlope, SIGNAL(runChanged(int)), this, SLOT(_runChanged(int)));
					slopeEnable = new QComboBox();
						slopeEnable->addItem(tr("No Slope"));
						slopeEnable->addItem(tr("Sloped Top"));
						slopeEnable->addItem(tr("Sloped Bottom"));
						centerLayout->addWidget(slopeEnable, 0, Qt::AlignHCenter);
							connect(slopeEnable, SIGNAL(currentIndexChanged(int)), this, SLOT(_slopeEnableChanged(int)));
				topBlocking = new QCheckBox(tr("Top"));
					currentTileGrid->addWidget(topBlocking, 0, 1, Qt::AlignHCenter | Qt::AlignVCenter);
				leftBlocking = new QCheckBox(tr("Left"));
					currentTileGrid->addWidget(leftBlocking, 1, 0, Qt::AlignHCenter | Qt::AlignVCenter);
				rightBlocking = new QCheckBox(tr("Right"));
					currentTileGrid->addWidget(rightBlocking, 1, 2, Qt::AlignHCenter | Qt::AlignVCenter);
				bottomBlocking = new QCheckBox(tr("Bottom"));
					currentTileGrid->addWidget(bottomBlocking, 2, 1, Qt::AlignHCenter | Qt::AlignVCenter);
		QGroupBox* gamerules = new QGroupBox("Game Rules");
			layout->addWidget(gamerules);
		QFormLayout* behaviorForm = new QFormLayout;
			gamerules->setLayout(behaviorForm);
					behavior = new QComboBox;
						behaviorForm->addRow(tr("When player touches center:"), behavior);
						behavior->addItem(tr("Do Nothing"));
						behavior->addItem(tr("Hurt"));
						behavior->addItem(tr("Door 1"));
						behavior->addItem(tr("Door 2"));
						behavior->addItem(tr("Door 3"));
						behavior->addItem(tr("Door 4"));
						behavior->addItem(tr("500 points"));
						behavior->addItem(tr("100 points"));
						behavior->addItem(tr("200 points"));
						behavior->addItem(tr("1000 points"));
						behavior->addItem(tr("5000 points"));
						behavior->addItem(tr("Joystick"));
						behavior->addItem(tr("Battery"));
						behavior->addItem(tr("Vacuum"));
						behavior->addItem(tr("Whiskey"));
						behavior->addItem(tr("Raygun"));
						behavior->addItem(tr("Pogo"));
						behavior->addItem(tr("Exit"));
						behavior->addItem(tr("Key 1"));
						behavior->addItem(tr("Key 2"));
						behavior->addItem(tr("Key 3"));
						behavior->addItem(tr("Key 4"));
						behavior->addItem(tr("Message box pops up"));
						behavior->addItem(tr("Lightswitch"));
						behavior->addItem(tr("Teleporter"));
						behavior->addItem(tr("Switch On"));
						behavior->addItem(tr("Switch Off"));
						behavior->addItem(tr("Ankh"));
						behavior->addItem(tr("Ammo"));
						behavior->addItem(tr("Foreground"));
						behavior->addItem(tr("Masked"));
					surfaceType = new QComboBox;
						surfaceType->addItem(tr("Do Nothing"));
						surfaceType->addItem(tr("Player slips"));
						surfaceType->addItem(tr("Player slides"));
						behaviorForm->addRow(tr("When player touches top:"), surfaceType);
								connect(topBlocking, SIGNAL(toggled(bool)), surfaceType, SLOT(setEnabled(bool)));
					bottomType = new QComboBox;
						bottomType->addItem(tr("Do Nothing"));
						bottomType->addItem(tr("(CKCM) Player grabs on"));
						bottomType->addItem(tr("Patch Action"));
						behaviorForm->addRow(tr("When player touches bottom:"), bottomType);
								connect(bottomBlocking, SIGNAL(toggled(bool)), bottomType, SLOT(setEnabled(bool)));

	connect(QApplication::instance(), SIGNAL(focusChanged(QWidget*, QWidget*)), this, SLOT(_focusChange(QWidget*, QWidget*)));


	toggleTopAction = new QShortcut(QKeySequence(tr("W")), this);
	connect(toggleTopAction, SIGNAL(activated()), this, SLOT(toggleTop()));
	toggleTopAction->setContext(Qt::ApplicationShortcut);

	toggleLeftAction = new QShortcut(QKeySequence(tr("A")), this);
	connect(toggleLeftAction, SIGNAL(activated()), this, SLOT(toggleLeft()));
	toggleLeftAction->setContext(Qt::ApplicationShortcut);

	toggleBottomAction = new QShortcut(QKeySequence(tr("S")), this);
	connect(toggleBottomAction, SIGNAL(activated()), this, SLOT(toggleBottom()));
	toggleBottomAction->setContext(Qt::ApplicationShortcut);

	toggleRightAction = new QShortcut(QKeySequence(tr("D")), this);
	connect(toggleRightAction, SIGNAL(activated()), this, SLOT(toggleRight()));
	toggleRightAction->setContext(Qt::ApplicationShortcut);

	setNormalTypeAction = new QShortcut(QKeySequence(tr("N")), this);
	connect(setNormalTypeAction, SIGNAL(activated()), this, SLOT(setNormalType()));
	setNormalTypeAction->setContext(Qt::ApplicationShortcut);

	cycleKeyTypesAction = new QShortcut(QKeySequence(tr("K")), this);
	connect(cycleKeyTypesAction, SIGNAL(activated()), this, SLOT(cycleKeyTypes()));
	cycleKeyTypesAction->setContext(Qt::ApplicationShortcut);

	cycleDoorTypesAction = new QShortcut(QKeySequence(tr("L")), this);
	connect(cycleDoorTypesAction, SIGNAL(activated()), this, SLOT(cycleDoorTypes()));
	cycleDoorTypesAction->setContext(Qt::ApplicationShortcut);

	setDeadlyTypeAction = new QShortcut(QKeySequence(tr("H")), this);
	connect(setDeadlyTypeAction, SIGNAL(activated()), this, SLOT(setDeadlyType()));
	setDeadlyTypeAction->setContext(Qt::ApplicationShortcut);

	setForegroundAction = new QShortcut(QKeySequence(tr("F")), this);
	connect(setForegroundAction, SIGNAL(activated()), this, SLOT(setForeground()));
	setForegroundAction->setContext(Qt::ApplicationShortcut);

	setMaskedAction = new QShortcut(QKeySequence(tr("M")), this);
	connect(setMaskedAction, SIGNAL(activated()), this, SLOT(setMasked()));
	setMaskedAction->setContext(Qt::ApplicationShortcut);

	topSlope->setStatusTip(tr("Drag handles to edit slope."));
	slopeEnable->setStatusTip(tr("Enable slope or select side to slope."));
	topBlocking->setStatusTip(tr("Shortcut: W"));
	leftBlocking->setStatusTip(tr("Shortcut: A"));
	bottomBlocking->setStatusTip(tr("Shortcut: S"));
	rightBlocking->setStatusTip(tr("Shortcut: D"));
}
void TileEditor::_slopeEnableChanged(int i) {
	topSlope->setEnabled(i > 0);
	topBlocking->setEnabled(i != 1);
	bottomBlocking->setEnabled(i != 2);
	_updateSlopeBlocking();
}
void TileEditor::setModel(QAbstractItemModel * model) {
	_model = model;
	mapper->setModel(model);
	mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
	mapper->addMapping(behavior, TileInfoModel::FIELD_BEHAVIOR, "currentIndex");
	mapper->addMapping(surfaceType, TileInfoModel::FIELD_TOP_STYLE, "currentIndex");
	mapper->addMapping(bottomType, TileInfoModel::FIELD_BOTTOM_STYLE, "currentIndex");
	mapper->addMapping(topBlocking, TileInfoModel::FIELD_TOP_BLOCKING);
	mapper->addMapping(rightBlocking, TileInfoModel::FIELD_RIGHT_BLOCKING);
	mapper->addMapping(bottomBlocking, TileInfoModel::FIELD_BOTTOM_BLOCKING);
	mapper->addMapping(leftBlocking, TileInfoModel::FIELD_LEFT_BLOCKING);
	mapper->addMapping(slopeEnable, TileInfoModel::FIELD_SLOPED_SIDE, "currentIndex");
	mapper->toFirst();
	topSlope->setModel(model);
	topSlope->setRow(0);
}

void TileEditor::_updateSlopeBlocking() {
	if(_model->index(_index.row(), TileInfoModel::FIELD_SLOPED_SIDE).data().toInt() == 2) {
		if(_model->index(_index.row(), TileInfoModel::FIELD_RUN).data().toInt() < 0) {
			rightBlocking->setEnabled(false);
			leftBlocking->setEnabled(true);
		} else {
			rightBlocking->setEnabled(true);
			leftBlocking->setEnabled(false);
		}
	} else {
		rightBlocking->setEnabled(true);
		leftBlocking->setEnabled(true);
	}
}

void TileEditor::setCurrentModelIndex(const QModelIndex & index) {
	_index = index;
	mapper->setCurrentModelIndex(index);
	topSlope->setRow(index.row());
	_updateSlopeBlocking();
}

void TileEditor::_focusChange(QWidget* old, QWidget* new_) {
	mapper->submit();
}

void TileEditor::_runChanged(int i) {
	_updateSlopeBlocking();
}

void TileEditor::toggleTop() {
	topBlocking->toggle();
	_focusChange(topBlocking, topBlocking);
}
void TileEditor::toggleLeft() {
	leftBlocking->toggle();
	_focusChange(leftBlocking, leftBlocking);
}
void TileEditor::toggleBottom() {
	bottomBlocking->toggle();
	_focusChange(bottomBlocking, bottomBlocking);
}
void TileEditor::toggleRight() {
	rightBlocking->toggle();
	_focusChange(rightBlocking, rightBlocking);
}
void TileEditor::setNormalType() {
	behavior->setCurrentIndex(TileInfoModel::BEHAVE_NOTHING);
	_focusChange(behavior, behavior);
}
void TileEditor::cycleKeyTypes() {
	if(behavior->currentIndex() >= TileInfoModel::BEHAVE_KEY_ONE && behavior->currentIndex() < TileInfoModel::BEHAVE_KEY_FOUR) {
		behavior->setCurrentIndex(behavior->currentIndex() + 1);
	}
	else {
		behavior->setCurrentIndex(TileInfoModel::BEHAVE_KEY_ONE);
	}
	_focusChange(behavior, behavior);
}
void TileEditor::cycleDoorTypes() {
	if(behavior->currentIndex() >= TileInfoModel::BEHAVE_DOOR_ONE && behavior->currentIndex() < TileInfoModel::BEHAVE_DOOR_FOUR) {
		behavior->setCurrentIndex(behavior->currentIndex() + 1);
	}
	else {
		behavior->setCurrentIndex(TileInfoModel::BEHAVE_DOOR_ONE);
	}
	_focusChange(behavior, behavior);
}
void TileEditor::setDeadlyType() {
	behavior->setCurrentIndex(TileInfoModel::BEHAVE_KILLS);
	_focusChange(behavior, behavior);
}
void TileEditor::setMasked() {
	behavior->setCurrentIndex(TileInfoModel::BEHAVE_MASKED);
	_focusChange(behavior, behavior);
}
void TileEditor::setForeground() {
	behavior->setCurrentIndex(TileInfoModel::BEHAVE_FOREGROUND);
	_focusChange(behavior, behavior);
}
