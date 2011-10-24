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

#include "PreferencesDialog.hpp"
#include <QtGui>
PreferencesDialog::PreferencesDialog(QSettings* settings, QWidget * parent, Qt::WindowFlags f): QDialog(parent, f), _settings(settings) {
	setWindowTitle(tr("Preferences"));
	QVBoxLayout* layout = new QVBoxLayout;
		setLayout(layout);
		QFormLayout* viewOptionsLayout = new QFormLayout;
			layout->addLayout(viewOptionsLayout);
			_settings->beginGroup("Tileset_View");
			_tilesetScale = new QSpinBox;
				_tilesetScale->setRange(1,8);
				_tilesetScale->setValue(_settings->value("Scale", 2).toInt());
				viewOptionsLayout->addRow(tr("Tileset View Zoom:"),_tilesetScale);
			_tilesetWidth = new QSpinBox;
				_tilesetWidth->setRange(1,50);
				_tilesetWidth->setValue(_settings->value("Width", 13).toInt());
				viewOptionsLayout->addRow(tr("Tileset Width:"),_tilesetWidth);
			_settings->endGroup();
			_settings->beginGroup("Animation_Editor");
				_animationZoom = new QSpinBox;
					_animationZoom->setRange(1,8);
					_animationZoom->setValue(_settings->value("Zoom", 4).toInt());
					viewOptionsLayout->addRow(tr("Animation Editor Zoom:"),_animationZoom);
				_settings->endGroup();
		QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Discard);
			layout->addWidget(buttonBox);
			connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
			connect(buttonBox->button(QDialogButtonBox::Discard), SIGNAL(clicked()), this, SLOT(reject()));

}
void PreferencesDialog::accept() {
	_settings->beginGroup("Tileset_View");
		_settings->setValue("Scale", _tilesetScale->value());
		_settings->setValue("Width", _tilesetWidth->value());
		_settings->endGroup();
		_settings->beginGroup("Animation_Editor");
			_settings->setValue("Zoom", _animationZoom->value());
			_settings->endGroup();
	QDialog::accept();
}
