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
PreferencesDialog::PreferencesDialog(QWidget * parent, Qt::WindowFlags f): QDialog(parent, f) {
	setWindowTitle(tr("Tileset Width"));
	QVBoxLayout* layout = new QVBoxLayout;
		setLayout(layout);
		_tilesetWidth = new QSpinBox;
		_tilesetWidth->setRange(1,50);
		_tilesetWidth->setValue(wrapping);
		layout->addWidget(_tilesetWidth);
		QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
			layout->addWidget(buttonBox);
			connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
			connect(buttonBox->button(QDialogButtonBox::Cancel), SIGNAL(clicked()), this, SLOT(reject()));

}
void PreferencesDialog::accept() {
	wrapping = _tilesetWidth->value();
	QDialog::accept();
}
int PreferencesDialog::exec() {
	_tilesetWidth->setValue(wrapping);
	return QDialog::exec();
}
