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

#include "FilePropertiesDialog.hpp"
#include <QtGui>
#include "TileModel.hpp"
FilePropertiesDialog::FilePropertiesDialog(QWidget * parent, Qt::WindowFlags f): QDialog(parent, f) {
		setWindowTitle(tr("File Properties"));
		QVBoxLayout* layout= new QVBoxLayout;
			setLayout(layout);
			QFormLayout* proplayout = new QFormLayout;
				layout->addLayout(proplayout);
				proplayout->addRow(tr("Format"), new QLabel(tr("Keen: Vorticons Extended")));
				proplayout->addRow(tr("Number of Tiles"), new QLabel(tr("910")));
				frames = new QComboBox;
					frames->addItem(tr("4"));
					frames->addItem(tr("8"));
					proplayout->addRow(tr("Number of Animation Frames"),frames);
			/*QHBoxLayout* buttonsLayout = new QHBoxLayout;
				layout->addLayout(buttonsLayout);
				QPushButton* cancel = new QPushButton(tr("Discard"));
					buttonsLayout->addWidget(cancel);
					connect(cancel, SIGNAL(clicked()), this, SLOT(reject()));
				QPushButton* save = new QPushButton(tr("Save"));
					save->setDefault(true);
					buttonsLayout->addWidget(save);
					connect(save, SIGNAL(clicked()), this, SLOT(accept()));*/
			QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Discard);
				layout->addWidget(buttonBox);
				connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
				connect(buttonBox->button(QDialogButtonBox::Discard), SIGNAL(clicked()), this, SLOT(reject()));
}
void FilePropertiesDialog::updateFrameCount(int i) {
	if(i == 4) frames->setCurrentIndex(0);
	else frames->setCurrentIndex(1);
}
void FilePropertiesDialog::setModel(TileModel* m) {
	model = m;
	connect(model, SIGNAL(frameCountChanged(int)), this, SLOT(updateFrameCount(int)));
}
void FilePropertiesDialog::accept() {
	model->setFrameCount(frames->currentText().toInt());
	QDialog::accept();
}

