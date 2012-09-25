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

#include "MainWindow.hpp"
#include <QtGui>
#include "TileModel.hpp"
#include "TileSetView.hpp"
#include "TileMapDelegate.hpp"
#include "TileEditor.hpp"
#include "AnimationEditor.hpp"
#include "TileInfo.h"
#include "TileModel.hpp"
#include "TileInfoModel.hpp"
#include "TileAnimModel.hpp"
#include "TileGfxModel.hpp"
#include "TilePaletteModel.hpp"
#include "FramePickerDialog.hpp"
#include "PaletteEditor.hpp"
class RowEdit: public QUndoCommand{
	public:
		RowEdit(QAbstractItemModel* model, int row, const QVector<int>& columns, const QVector<QVariant>& data) {
			_model = model;
			_data = data;
			_row = row;
			_columns = columns;
			for(int i = 0; i < _data.size(); ++i) {
				_oldData.append(_model->index(_row, _columns[i]).data());
			}
		}
		void redo(){
			for(int i = 0; i < _data.size(); ++i) {
				_model->setData(_model->index(_row, _columns[i]), _data[i]);
			}
		}
		void undo(){
			for(int i = 0; i < _oldData.size(); ++i) {
				_model->setData(_model->index(_row, _columns[i]), _oldData[i]);
			}
		}
		int id() {
			return 10;
		}
		bool mergeWith(const QUndoCommand* command){
			qDebug() << "attempting undo merge.";
			if(command->id() != id()) {
				qDebug() << "merge rejected: wrong id.";
				return false;
			}
			const RowEdit* rowEdit = static_cast<const RowEdit*>(command);
			if(rowEdit->_model != _model || rowEdit->_row != _row){
				qDebug() << "merge rejected: wrong model and/or row.";
				return false;
			}
			for(int i = 0; i < rowEdit->_columns.size(); ++i) {
				if(_columns.contains(rowEdit->_columns[i])) {
					return false;
					qDebug() << "merge rejected: overlapping changes.";
				}
			}
			_columns += rowEdit->_columns;
			_data += rowEdit->_data;
			_oldData += rowEdit->_oldData;
			qDebug() << "a command was merged.";
			return true;
		}
	private:
		QAbstractItemModel* _model;
		int _row;
		QVector<int> _columns;
		QVector<QVariant> _data;
		QVector<QVariant> _oldData;

};
void MainWindow::startNew() {
	_tileSet->blank();
	setCurrentFile("");
	//setWindowModified(false);
	_undoStack->setClean();
}
void MainWindow::about() {
	QMessageBox::about(this, tr("About Tileset Tool"), tr(
		"<p><b>Tileset Tool</b>, an editor for Commander Keen: Invasion of the Vorticons Tilesets.<br />"
		"Copyright &copy; 2010-2012 Kyle Delaney.</p>"

		"<p>Tileset Tool is free software: you can redistribute it and/or modify "
		"it under the terms of the GNU General Public License as published by "
		"the Free Software Foundation, either version 3 of the License, or "
		"(at your option) any later version.</p>"

		"<p>Tileset Tool is distributed in the hope that it will be useful, "
		"but WITHOUT ANY WARRANTY; without even the implied warranty of "
		"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the "
		"GNU General Public License for more details.</p>"

		"<p>You should have received a copy of the GNU General Public License "
		"along with this program.  If not, see <a href='http://www.gnu.org/licenses/'>www.gnu.org/licenses/</a>.</p>"
	));
}
void MainWindow::help() {
	_helpViewer->show();
}
void MainWindow::open() {
	QString filename = QFileDialog::getOpenFileName(this, tr("Open File"), _currentDirectory, tr("Tileset files (*.tls *.TLS *.tli *.TLI *.exe *.EXE);;Lemm's tileset patch file (*.tls *.TLS);;TileInfo file (*.tli *.TLI);;Commander Keen Episode (*.exe *.EXE)")); // *.tli *.exe
	setCurrentDirectory(filename);
	if(!filename.isEmpty()) {
		if(!_tileSet->open(filename)) {
			QMessageBox::critical(this, tr("File Error"), tr("The specified file could not be opened."));
		} else {
			//_propertyEditor->resetCurrentIndex();
			setCurrentFile(filename);
			//setWindowModified(false);
			_undoStack->setClean();
		}
	}
}
void MainWindow::saveAs() {
	QString filename = QFileDialog::getSaveFileName(this, tr("Save File"), _currentDirectory, tr("Lemm's tileset patch file (*.tls *.TLS)"));
	setCurrentDirectory(filename);
	if(!filename.isEmpty()) {
		if(!_tileSet->save(filename)) {
			QMessageBox::critical(this, tr("File Error"), tr("The specified file could not be saved to."));
		} else {
			setCurrentFile(filename);
			_undoStack->setClean();
		}
	}
}
void MainWindow::save() {
	if(_currentFile.isEmpty() || _currentFile.endsWith(".exe", Qt::CaseInsensitive)) {
		saveAs();
		return;
	}
	if(!_tileSet->save(_currentFile)) {
		QMessageBox::critical(this, tr("File Error"), tr("The file could not be saved."));
	}
	_undoStack->setClean();
}
void MainWindow::importBitmap() {
	QString filename = QFileDialog::getOpenFileName(this, tr("Open Image"), _currentDirectory, tr("Image files (*.bmp *.BMP *.png *.PNG *.tiff *.TIFF);; EGAHEAD Files (EGAHEAD.CK? egahead.ck?)"));
	setCurrentDirectory(filename);
	if(!filename.isEmpty()) {
		if(!_tileSet->importImage(filename)) {
			QMessageBox::critical(this, tr("File Error"), tr("The specified image file could not be imported. Make sure the width and height are divisible by 16."));
		} else {
			//setWindowModified(true);
		}
	}
}
void MainWindow::exportBitmap() {
	QString filename = QFileDialog::getSaveFileName(this, tr("Save Image"), _currentDirectory, tr("Image files (*.bmp *.png *.tiff)"));
	setCurrentDirectory(filename);
	if(!filename.isEmpty()) {
		if(!_tileSet->graphics().save(filename)) {
			QMessageBox::critical(this, tr("File Error"), tr("The specified image file could not be exported to."));
		}
	}
}
void MainWindow::exportForEditor() {
	QString filename = QFileDialog::getSaveFileName(this, tr("Save Image"), _currentDirectory, tr("Image files (*.bmp *.png *.tiff)"));
	setCurrentDirectory(filename);
	if(!filename.isEmpty()) {
        QImage dump = _mainView->dumpView().toImage().convertToFormat(QImage::Format_Indexed8, _tileSet->tilePalette()->dumpPage(0));
		if(!dump.save(filename)) {
			QMessageBox::critical(this, tr("File Error"), tr("The specified image file could not be exported to."));
		}
	}
}
void MainWindow::fixPalette() {
	int button = QMessageBox::question(this, tr("Fix Palette"), tr("Use standard EGA palette?"), QMessageBox::Yes | QMessageBox::No);
	if(button == QMessageBox::Yes) {
		_tileSet->fixPalette();
	} else {
		QString filename = QFileDialog::getOpenFileName(this, tr("Open Palette"), _currentDirectory, tr("Image and palette files (*.bmp *.BMP *.png *.PNG *.tiff *.TIFF *.bin *.BIN *.dat *.DAT);;Image files (*.bmp *.BMP *.png *.PNG *.tiff *.TIFF);;Raw VGA palettes (*.bin *.BIN *.dat *.DAT)"));
		if(!filename.isEmpty()) {
			setCurrentDirectory(filename);
			if(!_tileSet->fixPalette(filename)) {
				QMessageBox::critical(this, tr("File Error"), tr("The specified file could not be loaded as a palette."));
			}
		}
	}
}
void MainWindow::importPalette() {
	QString filename = QFileDialog::getOpenFileName(this, tr("Open Palette"), _currentDirectory, tr("Image and palette files (*.bmp *.BMP *.png *.PNG *.tiff *.TIFF *.bin *.BIN *.dat *.DAT);;Image files (*.bmp *.BMP *.png *.PNG *.tiff *.TIFF);;Raw VGA palettes (*.bin *.BIN *.dat *.DAT)"));
	setCurrentDirectory(filename);
	if(!filename.isEmpty()) {
		if(!_tileSet->loadPalette(filename)) {
			QMessageBox::critical(this, tr("File Error"), tr("The specified file could not be loaded as a palette."));
		}
	}
}
void MainWindow::exportPalette() {
	QString filename = QFileDialog::getSaveFileName(this, tr("Save Palette"), _currentDirectory, tr("Raw VGA palettes (*.bin *.BIN *.dat *.DAT)"));
	setCurrentDirectory(filename);
	if(!filename.isEmpty()) {
		if(!_tileSet->savePalette(filename)) {
			QMessageBox::critical(this, tr("File Error"), tr("The palette could not be exported."));
		}
	}
}

#include "PreferencesDialog.hpp"
void MainWindow::preferences() {
	if(_prefsDialog->exec() == QDialog::Accepted) {
		_saveSettings();
		_readSettings();
	}
}
void MainWindow::setPaintMode(bool b) {
	if(_paintMode == true && b == false) {
		_propertyEditor->setCurrentModelIndex(_mainView->currentIndex());
		_brushRow = _mainView->currentIndex().row();
	}
	_paintMode = b;
}
void MainWindow::_setPaintAnimations(bool b) {
	_paintAnimations = b;
}
void MainWindow::_setPaintProperties(bool b) {
	_paintProperties = b;
}

void MainWindow::tileSelected(int i) {
	_currentTileIndicator->setText(tr("Tile: %1/910 Hex: 0x%2").arg(i+1).arg(i, 4, 16, QLatin1Char( '0' )));
	if(!_paintMode) {
		_propertyEditor->setCurrentModelIndex(_tileSet->tileInfo()->index(i, 0));
		_brushRow = i;
	} else {
		_undoStack->beginMacro(tr("paint tiles"));
		if(_paintProperties) {
			for(int j = 0; j < _tileSet->tileInfo()->columnCount(); ++j) {
				_tileInfoEdited(_tileSet->tileInfo()->index(i, j), _tileSet->tileInfo()->index(_brushRow, j).data());//_tileSet->tileInfo()->setData(_tileSet->tileInfo()->index(i, j), _tileSet->tileInfo()->index(_brushRow, j).data());
			}
		}
		if (_paintAnimations) {
			for(int j = 0; j < _tileSet->tileAnim()->columnCount(); ++j) {
				int frame = _tileSet->tileAnim()->index(_brushRow, j).data().toInt() + i - _brushRow;
				_tileInfoEdited(_tileSet->tileAnim()->index(i, j), frame);//_tileSet->tileAnim()->setData(_tileSet->tileAnim()->index(i, j), frame);
			}
		}
		_undoStack->endMacro();
	}
	_animEditor->setCurrentModelIndex(_tileSet->tileAnim()->index(i, 0));
}
void MainWindow::createActions() {
	newAction = new QAction(QIcon(":/images/new.png"), tr("&New"), this);
	newAction->setShortcuts(QKeySequence::New);
	connect(newAction, SIGNAL(triggered()), this, SLOT(startNew()));

	openAction = new QAction(QIcon(":/images/open.png"), tr("&Open..."), this);
	openAction->setShortcuts(QKeySequence::Open);
	connect(openAction, SIGNAL(triggered()), this, SLOT(open()));

	saveAction = new QAction(QIcon(":/images/save.png"), tr("&Save"), this);
	saveAction->setShortcuts(QKeySequence::Save);
	connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));

	saveAsAction = new QAction(QIcon(":/images/save-as.png"), tr("Save &As..."), this);
	saveAsAction->setShortcuts(QKeySequence::SaveAs);
	connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()));

	importBitmapAction = new QAction(QIcon(":/images/import-image.png"), tr("&Import image..."), this);
	connect(importBitmapAction, SIGNAL(triggered()), this, SLOT(importBitmap()));

	exportBitmapAction = new QAction(QIcon(":/images/export-image.png"), tr("&Export image..."), this);
	connect(exportBitmapAction, SIGNAL(triggered()), this, SLOT(exportBitmap()));
    
	exportForEditorAction = new QAction(QIcon(":/images/export-image.png"), tr("&Export to external editor..."), this);
	connect(exportForEditorAction, SIGNAL(triggered()), this, SLOT(exportForEditor()));

	fixPaletteAction = new QAction(QIcon(":/images/convert-palette.png"), tr("&Fix palette..."), this);
	connect(fixPaletteAction, SIGNAL(triggered()), this, SLOT(fixPalette()));

	importPaletteAction = new QAction(QIcon(":/images/import-palette.png"), tr("Import &palette..."), this);
	connect(importPaletteAction, SIGNAL(triggered()), this, SLOT(importPalette()));

	exportPaletteAction = new QAction(QIcon(":/images/export-palette.png"), tr("E&xport palette..."), this);
	connect(exportPaletteAction, SIGNAL(triggered()), this, SLOT(exportPalette()));

	setOneToOneAction = new QAction(QIcon(":/images/map-tiles.png"), tr("Auto-map tiles"), this);
	connect(setOneToOneAction, SIGNAL(triggered()), this, SLOT(setOneToOne()));

	quitAction = new QAction(QIcon(":/images/exit.png"), tr("&Quit"), this);
	quitAction->setShortcut(QKeySequence(tr("Ctrl+Q")));
	connect(quitAction, SIGNAL(triggered()), this, SLOT(close()));

	aboutAction = new QAction(QIcon(":/images/about.png"), tr("&About..."), this);
	connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

	helpAction = new QAction(QIcon(":/images/help.png"), tr("&Contents"), this);
	helpAction->setShortcuts(QKeySequence::HelpContents);
	connect(helpAction, SIGNAL(triggered()), this, SLOT(help()));


	prefAction = new QAction(QIcon(":/images/preferences.png"), tr("P&references..."), this);
	connect(prefAction, SIGNAL(triggered()), this, SLOT(preferences()));

	propertiesAction = new QAction(QIcon(":/images/properties.png"), tr("&Properties..."), this);

	undoAction = _undoStack->createUndoAction(this, tr("&Undo"));
	undoAction->setShortcuts(QKeySequence::Undo);

	redoAction = _undoStack->createRedoAction(this, tr("&Redo"));
	redoAction->setShortcuts(QKeySequence::Redo);

	togglePaintModeAction = new QAction(QIcon(":/images/paint.png"), tr("&Paint mode"), this);
	togglePaintModeAction->setCheckable(true);
	connect(togglePaintModeAction, SIGNAL(toggled(bool)), this, SLOT(setPaintMode(bool)));
	//togglePaintModeAction->setIconVisibleInMenu(false);

	paintMenu = new QMenu(this);
	togglePaintModeAction->setMenu(paintMenu);

	togglePaintAnimationsAction = paintMenu->addAction(tr("Paint animations"));
	togglePaintAnimationsAction->setCheckable(true);
	connect(togglePaintAnimationsAction, SIGNAL(toggled(bool)), this, SLOT(_setPaintAnimations(bool)));

	togglePaintPropertiesAction = paintMenu->addAction(tr("Paint properties"));
	togglePaintPropertiesAction->setCheckable(true);
	connect(togglePaintPropertiesAction, SIGNAL(toggled(bool)), this, SLOT(_setPaintProperties(bool)));

	toggleTopAction = new QAction(tr("Toggle Top Collison"), this);
	connect(toggleTopAction, SIGNAL(triggered()), this, SLOT(toggleTop()));
	toggleTopAction->setShortcut(QKeySequence(tr("W")));

	toggleLeftAction = new QAction(tr("Toggle Left Collison"), this);
	connect(toggleLeftAction, SIGNAL(triggered()), this, SLOT(toggleLeft()));
	toggleLeftAction->setShortcut(QKeySequence(tr("A")));

	toggleBottomAction = new QAction(tr("Toggle Bottom Collison"), this);
	connect(toggleBottomAction, SIGNAL(triggered()), this, SLOT(toggleBottom()));
	toggleBottomAction->setShortcut(QKeySequence(tr("S")));

	toggleRightAction = new QAction(tr("Toggle Right Collison"), this);
	connect(toggleRightAction, SIGNAL(triggered()), this, SLOT(toggleRight()));
	toggleRightAction->setShortcut(QKeySequence(tr("D")));

	setNormalTypeAction = new QAction(tr("Set &Normal Behavior"), this);
	connect(setNormalTypeAction, SIGNAL(triggered()), this, SLOT(setNormalType()));
	setNormalTypeAction->setShortcut(QKeySequence(tr("N")));

	cycleKeyTypesAction = new QAction(tr("Set &Key Behavior"), this);
	connect(cycleKeyTypesAction, SIGNAL(triggered()), this, SLOT(cycleKeyTypes()));
	cycleKeyTypesAction->setShortcut(QKeySequence(tr("K")));

	cycleDoorTypesAction = new QAction(tr("Set (&Locked) Door Behavior"), this);
	connect(cycleDoorTypesAction, SIGNAL(triggered()), this, SLOT(cycleDoorTypes()));
	cycleDoorTypesAction->setShortcut(QKeySequence(tr("L")));

	setDeadlyTypeAction = new QAction(tr("Set &Hazard Behavior"), this);
	connect(setDeadlyTypeAction, SIGNAL(triggered()), this, SLOT(setDeadlyType()));
	setDeadlyTypeAction->setShortcut(QKeySequence(tr("H")));

	setForegroundAction = new QAction(tr("Set &Foreground Behavior"), this);
	connect(setForegroundAction, SIGNAL(triggered()), this, SLOT(setForeground()));
	setForegroundAction->setShortcut(QKeySequence(tr("F")));

	setMaskedAction = new QAction(tr("Set &Masked Behavior"), this);
	connect(setMaskedAction, SIGNAL(triggered()), this, SLOT(setMasked()));
	setMaskedAction->setShortcut(QKeySequence(tr("M")));

	setAnimationAction = new QAction(tr("Apply Animation &Cycle"), this);
	connect(setAnimationAction, SIGNAL(triggered()), _animEditor, SLOT(applyPreset()));
	setAnimationAction->setShortcut(QKeySequence(tr("C")));


}
void MainWindow::createMenus() {
	fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(newAction);
	fileMenu->addAction(openAction);
	fileMenu->addAction(saveAction);
	fileMenu->addAction(saveAsAction);
	fileMenu->addSeparator();
	//fileMenu->addAction(propertiesAction);
	//fileMenu->addSeparator();
	fileMenu->addAction(quitAction);

	editMenu = menuBar()->addMenu(tr("&Edit"));
	editMenu->addAction(undoAction);
	editMenu->addAction(redoAction);
	editMenu->addSeparator();
	editMenu->addAction(togglePaintModeAction);
	editMenu->addSeparator();
	editMenu->addAction(prefAction);
//	editMenu->addAction(toggleTopAction);
//	editMenu->addAction(toggleLeftAction);
//	editMenu->addAction(toggleBottomAction);
//	editMenu->addAction(toggleRightAction);
//	editMenu->addAction(setNormalTypeAction);
//	editMenu->addAction(cycleKeyTypesAction);
//	editMenu->addAction(cycleDoorTypesAction);
//	editMenu->addAction(setDeadlyTypeAction);
//	editMenu->addAction(setMaskedAction);
//	editMenu->addAction(setForegroundAction);
//	editMenu->addAction(setAnimationAction);

	toolMenu = menuBar()->addMenu(tr("&Tools"));
	toolMenu->addAction(importBitmapAction);
	toolMenu->addAction(fixPaletteAction);
	toolMenu->addAction(importPaletteAction);
	toolMenu->addSeparator();
	toolMenu->addAction(setOneToOneAction);
	toolMenu->addAction(exportForEditorAction);
	toolMenu->addAction(exportBitmapAction);
	toolMenu->addAction(exportPaletteAction);

	menuBar()->addSeparator();

	helpMenu = menuBar()->addMenu(tr("&Help"));
	helpMenu->addAction(helpAction);
	helpMenu->addAction(aboutAction);
}
void MainWindow::createToolBars() {
	fileToolBar = addToolBar(tr("File"));
	fileToolBar->setObjectName("fileToolbar");
	fileToolBar->addAction(newAction);
	fileToolBar->addAction(openAction);
	fileToolBar->addAction(saveAction);
	editToolBar = addToolBar(tr("Edit"));
	editToolBar->setObjectName("editToolbar");
	editToolBar->addAction(importBitmapAction);
	editToolBar->addAction(fixPaletteAction);
	editToolBar->addAction(importPaletteAction);
	editToolBar->addAction(togglePaintModeAction);
//	editToolBar->addAction(exportBitmapAction);
//	editToolBar->addAction(setOneToOneAction);
//	editToolBar->addAction(exportPaletteAction);

}
void MainWindow::setCurrentFile(QString str) {
	_currentFile = str;
	if(_currentFile.isEmpty()) {
		setWindowTitle(tr("%1[*] - Tileset Tool").arg(tr("untitled.tls")));
	} else {
		setWindowTitle(tr("%1[*] - Tileset Tool").arg(QFileInfo(_currentFile).fileName()));
	}
}
void MainWindow::setCurrentDirectory(QString str) {
	if(str.isEmpty()) {
		if(_currentDirectory.isEmpty()) {
			_currentDirectory = QDir::homePath();
		}
	} else {
		_currentDirectory = QFileInfo(str).path();
	}
}
bool MainWindow::reallyClose() {
	if(isWindowModified()) {
		switch(QMessageBox::question(this, tr("Save Changes"), tr("The tileset has been modified. Would you like to save your changes?"), QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel)) {
			case QMessageBox::Save:
				save();
			case QMessageBox::Discard:
				return true;
			case QMessageBox::Cancel:
				return false;
			default:
				return false;
		}
	}
	return true;
}
void MainWindow::closeEvent(QCloseEvent* event) {
	if(reallyClose()) {
		event->accept();
	} else {
		event->ignore();
	}
}
//void MainWindow::dataChanged(const QModelIndex& topLeft,const QModelIndex& bottomRight) {
	//setWindowModified(true);
//}
void MainWindow::_tileInfoEdited(const QModelIndex& index, const QVariant& data) {
	if(data == _tileSet->tileInfo()->data(index)) return;
	RowEdit* command = new RowEdit(_tileSet->tileInfo(), index.row(), QVector<int>(1, index.column()), QVector<QVariant>(1, data));
	command->setText(tr("set tile properties"));
	_undoStack->push(command);
	//_tileSet->tileInfo()->setData(index, data);
}
void MainWindow::_frameEdited(const QModelIndex& index, const QVariant& data) {
	if(data == _tileSet->tileAnim()->data(index)) return;
	RowEdit* command = new RowEdit(_tileSet->tileAnim(), index.row(), QVector<int>(1, index.column()), QVector<QVariant>(1, data));
	command->setText(tr("set tile animation"));
	_undoStack->push(command);
	//_tileSet->tileAnim()->setData(index, data);
}
void MainWindow::_presetApplied(int section, const QVector<int>& newFrames) {
	QVector<int> columns;
	QVector<QVariant> data;
	for(int i = 0; i < newFrames.size(); ++i) {
		columns.append(i);
		data.append(newFrames[i]);
	}
	RowEdit* rowEdit = new RowEdit(_tileSet->tileAnim(), section, columns, data);
	rowEdit->setText("set tile animation");
	_undoStack->push(rowEdit);
//	for(int i = 0; i < newFrames.size(); ++i) {
//		_tileSet->tileAnim()->setData(_tileSet->tileAnim()->index(section, i), newFrames[i]);
//	}
}
void MainWindow::_frameCountSet(int i) {
	_tileSet->setFrameCount(i);
}
void MainWindow::_setUnmodified(bool b) {
	setWindowModified(!b);
}
void MainWindow::_readSettings() {
	_settings->beginGroup("Window");
		resize(_settings->value("Size", QSize(976, 506)).toSize());
		move(_settings->value("Position", QPoint(200, 200)).toPoint());
		restoreState(_settings->value("State").toByteArray());
		_settings->endGroup();
	_settings->beginGroup("Help_Window");
		_helpViewer->resize(_settings->value("Size", QSize(500, 600)).toSize());
		_helpViewer->move(_settings->value("Position", QPoint(200, 200)).toPoint());
		_settings->endGroup();
	_settings->beginGroup("Frame_Picker_Window");
		_animEditor->framePicker()->resize(_settings->value("Size", QSize(500, 500)).toSize());
		_animEditor->framePicker()->move(_settings->value("Position", QPoint(200, 200)).toPoint());
		_settings->endGroup();
	_settings->beginGroup("Tileset_View");
		_mainView->setZoom(_settings->value("Scale", 2).toInt());
		_mainView->setWrap(_settings->value("Width", 13).toInt());
		_animEditor->framePicker()->setZoom(_settings->value("Scale", 2).toInt());
		_animEditor->framePicker()->setWrap(_settings->value("Width", 13).toInt());
		_settings->endGroup();
	_settings->beginGroup("Animation_Editor");
		_animEditor->setZoom(_settings->value("Zoom", 3).toInt());
		_settings->endGroup();
	_settings->beginGroup("Tools");
		_paintProperties = _settings->value("paint_properties", true).toBool();
		togglePaintPropertiesAction->setChecked(_paintProperties);
		_paintAnimations = _settings->value("paint_animations", false).toBool();
		togglePaintAnimationsAction->setChecked(_paintAnimations);
		_settings->endGroup();

	_currentDirectory = _settings->value("Working_Directory", QDir::homePath()).toString();
}
void MainWindow::_saveSettings() {
	_settings->beginGroup("Window");
		_settings->setValue("Size", size());
		_settings->setValue("Position", pos());
		_settings->setValue("State", saveState());
		_settings->endGroup();
	_settings->beginGroup("Help_Window");
		_settings->setValue("Size", _helpViewer->size());
		_settings->setValue("Position", _helpViewer->pos());
		_settings->endGroup();
	_settings->beginGroup("Frame_Picker_Window");
		_settings->setValue("Size", _animEditor->framePicker()->size());
		_settings->setValue("Position", _animEditor->framePicker()->pos());
		_settings->endGroup();
	_settings->beginGroup("Tools");
		_settings->setValue("paint_properties", _paintProperties);
		_settings->setValue("paint_animations", _paintAnimations);
		_settings->endGroup();
	//Tileset View settings are only editable by the preferences dialog, which saves them immeadiately.
	_settings->setValue("Working_Directory", _currentDirectory);
}

MainWindow::MainWindow(QWidget * parent, Qt::WindowFlags flags): QMainWindow(parent, flags), _paintMode(false) {
	setWindowIcon(QIcon(":/images/icon.svg"));
	setCurrentFile("");
	_tileSet = new TileModel(this);
	_mainView = new TileSetView;
	_mainView->setModel(_tileSet->tileAnim());
	_graphicDelegate = new TileMapDelegate(this);
	_graphicDelegate->setGfxModel(_tileSet->tileGfx());
	_mainView->setItemDelegate(_graphicDelegate);
	_propertyEditor = new TileEditor;
		_propertyEditor->setModel(_tileSet->tileInfo());
	_animEditor = new AnimationEditor;
		_animEditor->setModel(_tileSet->tileAnim());
		_animEditor->setGfxModel(_tileSet->tileGfx());
	_paletteEditor = new PaletteEditor;
	_paletteEditor->setModel(_tileSet->tilePalette());

	_undoStack = new QUndoStack(this);
	connect(_undoStack, SIGNAL(cleanChanged(bool)), this, SLOT(_setUnmodified(bool)));

	_helpViewer = new QTextBrowser();
	_helpViewer->setSource(QUrl("qrc:/docs/index.html"));
	_helpViewer->setWindowTitle("Tileset Tool Help");

	createActions();
	createMenus();
	createToolBars();

	QDockWidget* editorDock = new QDockWidget(tr("Tile Properties"), this);
	editorDock->setObjectName("editorDock");
	editorDock->setWidget(_propertyEditor);
	editorDock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
	addDockWidget(Qt::RightDockWidgetArea, editorDock);

	QDockWidget* animDock = new QDockWidget(tr("Animation"), this);
	animDock->setObjectName("animDock");
	animDock->setWidget(_animEditor);
	animDock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
	addDockWidget(Qt::RightDockWidgetArea, animDock);

	QDockWidget* paletteDock = new QDockWidget(tr("Palette"), this);
	paletteDock->setObjectName("paletteDock");
	paletteDock->setWidget(_paletteEditor);
	paletteDock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
	addDockWidget(Qt::RightDockWidgetArea, paletteDock);

	setCentralWidget(_mainView);
	startNew();
	_settings = new QSettings(tr("Tileset Tool"), QString(), this);
	_readSettings();
	_prefsDialog = new PreferencesDialog(_settings, this);

	_currentTileIndicator = new QLabel(tr("Tile: %1/910 Hex: 0x%2").arg(1).arg(0, 4, 16, QLatin1Char( '0' )));
	connect(_mainView, SIGNAL(tileSelected(int)), this, SLOT(tileSelected(int)));
	statusBar()->addWidget(_currentTileIndicator);

	_mainView->setCurrentIndex(_tileSet->tileAnim()->index(0,0));


	connect(_propertyEditor, SIGNAL(propertyChanged(const QModelIndex&, const QVariant&)), this, SLOT(_tileInfoEdited(const QModelIndex&, const QVariant&)));
	connect(_animEditor, SIGNAL(frameEdited(const QModelIndex&, const QVariant&)), this, SLOT(_frameEdited(const QModelIndex&, const QVariant&)));
	connect(_animEditor, SIGNAL(presetApplied(int, const QVector<int>&)), this, SLOT(_presetApplied(int, const QVector<int>&)));
	connect(_animEditor, SIGNAL(frameCountChange(int)), this, SLOT(_frameCountSet(int)));
	connect(_tileSet, SIGNAL(frameCountChanged(int)), _animEditor, SLOT(programSetFrameCount(int)));
}
MainWindow::~MainWindow() {
	_saveSettings();
	delete _helpViewer;
}
void MainWindow::toggleTop() {
	QModelIndex index = _tileSet->tileInfo()->index(_mainView->currentIndex().row(), TileInfo::FIELD_TOP);
	if(_tileSet->tileInfo()->data(index).toBool()) {
		_tileSet->tileInfo()->setData(index, 0);
	}
	else {
		_tileSet->tileInfo()->setData(index, 1);
	}
}
void MainWindow::toggleLeft() {
	QModelIndex index = _tileSet->tileInfo()->index(_mainView->currentIndex().row(), TileInfo::FIELD_LEFT);
	if(_tileSet->tileInfo()->data(index).toBool()) {
		_tileSet->tileInfo()->setData(index, 0);
	}
	else {
		_tileSet->tileInfo()->setData(index, 1);
	}
}
void MainWindow::toggleBottom() {
	QModelIndex index = _tileSet->tileInfo()->index(_mainView->currentIndex().row(), TileInfo::FIELD_BOTTOM);
	if(_tileSet->tileInfo()->data(index).toBool()) {
		_tileSet->tileInfo()->setData(index, 0);
	}
	else {
		_tileSet->tileInfo()->setData(index, 1);
	}

}
void MainWindow::toggleRight() {
	QModelIndex index = _tileSet->tileInfo()->index(_mainView->currentIndex().row(), TileInfo::FIELD_RIGHT);
	if(_tileSet->tileInfo()->data(index).toBool()) {
		_tileSet->tileInfo()->setData(index, 0);
	}
	else {
		_tileSet->tileInfo()->setData(index, 1);
	}
}
void MainWindow::setNormalType() {
	QModelIndex index = _tileSet->tileInfo()->index(_mainView->currentIndex().row(), TileInfo::FIELD_BEHAVIOR);
	_tileSet->tileInfo()->setData(index, TileInfo::BEHAVE_NOTHING);

}
void MainWindow::cycleKeyTypes() {
	QModelIndex index = _tileSet->tileInfo()->index(_mainView->currentIndex().row(), TileInfo::FIELD_BEHAVIOR);
	if(_tileSet->tileInfo()->data(index).toInt() >= TileInfo::BEHAVE_KEY_ONE && _tileSet->tileInfo()->data(index).toInt() < TileInfo::BEHAVE_KEY_FOUR) {
		_tileSet->tileInfo()->setData(index, _tileSet->tileInfo()->data(index).toInt() + 1);
	}
	else {
		_tileSet->tileInfo()->setData(index, TileInfo::BEHAVE_KEY_ONE);
	}
}
void MainWindow::cycleDoorTypes() {
	QModelIndex index = _tileSet->tileInfo()->index(_mainView->currentIndex().row(), TileInfo::FIELD_BEHAVIOR);
	if(_tileSet->tileInfo()->data(index).toInt() >= TileInfo::BEHAVE_DOOR_ONE && _tileSet->tileInfo()->data(index).toInt() < TileInfo::BEHAVE_DOOR_FOUR) {
		_tileSet->tileInfo()->setData(index, _tileSet->tileInfo()->data(index).toInt() + 1);
	}
	else {
		_tileSet->tileInfo()->setData(index, TileInfo::BEHAVE_DOOR_ONE);
	}
}
void MainWindow::setDeadlyType() {
	QModelIndex index = _tileSet->tileInfo()->index(_mainView->currentIndex().row(), TileInfo::FIELD_BEHAVIOR);
	_tileSet->tileInfo()->setData(index, TileInfo::BEHAVE_KILLS);
}
void MainWindow::setMasked() {
	QModelIndex index = _tileSet->tileInfo()->index(_mainView->currentIndex().row(), TileInfo::FIELD_BEHAVIOR);
	_tileSet->tileInfo()->setData(index, TileInfo::BEHAVE_MASKED);
}
void MainWindow::setForeground() {
	QModelIndex index = _tileSet->tileInfo()->index(_mainView->currentIndex().row(), TileInfo::FIELD_BEHAVIOR);
	_tileSet->tileInfo()->setData(index, TileInfo::BEHAVE_FOREGROUND);
}
void MainWindow::setOneToOne() {
	if(QMessageBox::question(this, tr("Proceed?"), tr("All animation data will be overwritten! Are you sure you want to continue?"), QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok) {
		QProgressDialog progress(tr("Setting frames..."), tr("Cancel"), 0, _tileSet->tileAnim()->rowCount()*_tileSet->tileAnim()->columnCount(), this);
		progress.setWindowTitle("Operation in Progress");
		progress.setWindowModality(Qt::WindowModal);
		for(int i = 0; i < _tileSet->tileAnim()->rowCount(); ++i) {
			for(int j = 0; j < _tileSet->tileAnim()->columnCount(); ++j) {
				_tileSet->tileAnim()->setData(_tileSet->tileAnim()->index(i, j), i);
				progress.setValue( (i * _tileSet->tileAnim()->columnCount() ) + j);
				if(progress.wasCanceled()) {
					i = _tileSet->tileAnim()->rowCount();
					break;
				}
			}
		}
		progress.setValue(_tileSet->tileAnim()->rowCount()*_tileSet->tileAnim()->columnCount());
	}
}
