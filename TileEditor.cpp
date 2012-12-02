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
		//QGroupBox* collisionGroup = new QGroupBox("&Collision:");
			//layout->addWidget(collisionGroup);
			QGridLayout* currentTileGrid = new QGridLayout;
				layout->addLayout(currentTileGrid); //collisionGroup->setLayout(currentTileGrid);
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
					slopeEnable = new QCheckBox(tr("Sloped"));
						centerLayout->addWidget(slopeEnable, 0, Qt::AlignHCenter);
							connect(slopeEnable, SIGNAL(toggled(bool)), topSlope, SLOT(setEnabled(bool)));
				topBlocking = new QCheckBox(tr("Top"));
					currentTileGrid->addWidget(topBlocking, 0, 1, Qt::AlignHCenter | Qt::AlignVCenter);
				leftBlocking = new QCheckBox(tr("Left"));
					currentTileGrid->addWidget(leftBlocking, 1, 0, Qt::AlignHCenter | Qt::AlignVCenter);
				rightBlocking = new QCheckBox(tr("Right"));
					currentTileGrid->addWidget(rightBlocking, 1, 2, Qt::AlignHCenter | Qt::AlignVCenter);
				bottomBlocking = new QCheckBox(tr("Bottom"));
					currentTileGrid->addWidget(bottomBlocking, 2, 1, Qt::AlignHCenter | Qt::AlignVCenter);
		QFormLayout* behaviorForm = new QFormLayout;
			layout->addLayout(behaviorForm);
					behavior = new QComboBox;
						behaviorForm->addRow(tr("Interaction:"), behavior);
						behavior->addItem(tr("Does Nothing"));
						behavior->addItem(tr("Kills"));
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
						behavior->addItem(tr("Message box popup"));
						behavior->addItem(tr("Lightswitch"));
						behavior->addItem(tr("Teleporter"));
						behavior->addItem(tr("Switch On"));
						behavior->addItem(tr("Switch Off"));
						behavior->addItem(tr("Ankh"));
						behavior->addItem(tr("Ammo"));
						behavior->addItem(tr("Foreground"));
						behavior->addItem(tr("Masked"));
					surfaceType = new QComboBox;
						surfaceType->addItem(tr("Normal"));
						surfaceType->addItem(tr("Slippery"));
						surfaceType->addItem(tr("Frictionless"));
						behaviorForm->addRow(tr("Friction:"), surfaceType);
								connect(topBlocking, SIGNAL(toggled(bool)), surfaceType, SLOT(setEnabled(bool)));
					bottomType = new QComboBox;
						bottomType->addItem(tr("Normal"));
						bottomType->addItem(tr("Type 2 (CKCM Hand-hang)"));
						bottomType->addItem(tr("Type 3"));
						behaviorForm->addRow(tr("Bottom Behavior:"), bottomType);
								connect(bottomBlocking, SIGNAL(toggled(bool)), bottomType, SLOT(setEnabled(bool)));

	connect(QApplication::instance(), SIGNAL(focusChanged(QWidget*, QWidget*)), this, SLOT(_focusChange(QWidget*, QWidget*)));
}
void TileEditor::setModel(QAbstractItemModel * model) {
	_model = model;
	mapper->setModel(model);
	mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
	mapper->addMapping(behavior, TileInfoModel::FIELD_BEHAVIOR, "currentIndex");
	mapper->addMapping(surfaceType, TileInfoModel::FIELD_SURFACE_TYPE + TileInfoModel::TOP, "currentIndex");
	mapper->addMapping(bottomType, TileInfoModel::FIELD_SURFACE_TYPE + TileInfoModel::TOP, "currentIndex");
	mapper->addMapping(topBlocking, TileInfoModel::FIELD_SOLID + TileInfoModel::TOP);
	mapper->addMapping(rightBlocking, TileInfoModel::FIELD_SOLID + TileInfoModel::RIGHT);
	mapper->addMapping(bottomBlocking, TileInfoModel::FIELD_SOLID + TileInfoModel::BOTTOM);
	mapper->addMapping(leftBlocking, TileInfoModel::FIELD_SOLID + TileInfoModel::LEFT);
	mapper->addMapping(slopeEnable, TileInfoModel::FIELD_SLOPED + TileInfoModel::TOP);
	mapper->toFirst();
	topSlope->setModel(model);
	topSlope->setRow(0);
}

void TileEditor::setCurrentModelIndex(const QModelIndex & index) {
	_index = index;
	mapper->setCurrentModelIndex(index);
	topSlope->setRow(index.row());
}

void TileEditor::_focusChange(QWidget* old, QWidget* new_) {
	int section;
	section = mapper->mappedSection(old);
	if(section != -1) {
		emit propertyChanged(_model->index(mapper->currentIndex(), section), old->property(mapper->mappedPropertyName(old)));
	}
}
