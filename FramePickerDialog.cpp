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

#include "FramePickerDialog.hpp"
#include "TileSetView.hpp"
#include "TileSheetDelegate.hpp"
#include "TileGfxModel.hpp"
#include <QtGui>
void FramePickerDialog::setModel(QAbstractItemModel* model) {
	_model = model;
	_view->setModel(_model);
	_view->setItemDelegate(_delegate);
}
void FramePickerDialog::setWrap(int i) {
	_view->setWrap(i);
}
void FramePickerDialog::setZoom(int i) {
	_view->setZoom(i);
}

FramePickerDialog::FramePickerDialog(QWidget * parent, Qt::WindowFlags f): QDialog(parent, f) {
	QVBoxLayout* layout = new QVBoxLayout;
	setLayout(layout);
	_view = new TileSetView();
	_delegate = new TileSheetDelegate(this);
	layout->addWidget(_view);
	connect(_view, SIGNAL(tileSelected(int)), this, SLOT(frameSelected(int)));
}
void FramePickerDialog::frameSelected(int i) {
	_selectedFrame = i;
	accept();
}
void FramePickerDialog::showEvent(QShowEvent * event) {
	QDialog::showEvent(event);
}
int FramePickerDialog::selectedFrame(void) {
	return _selectedFrame;
}
void FramePickerDialog::setSelectedFrame(int i) {
	_selectedFrame = i;
	_view->setCurrentIndex(_model->index(i, 0));
	_view->scrollTo(_model->index(i, 0));
}
