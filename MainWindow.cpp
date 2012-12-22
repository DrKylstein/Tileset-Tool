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
#include "PreferencesDialog.hpp"

void MainWindow::startNew() {
	if(!reallyClose()) {
		return;
	}
	_tileSet->blank();
	setCurrentFile("");
	setWindowModified(false);
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
	QString filename = QFileDialog::getOpenFileName(this, tr("Open File"), _currentDirectory,
		tr("V2 tileset file (*.tls *.TLS)"));
	setCurrentDirectory(filename);
	if(!filename.isEmpty()) {
		if(!reallyClose()) {
			return;
		}
		if(!_tileSet->open(filename)) {
			QMessageBox::critical(this, tr("File Error"), tr("The specified file could not be opened."));
		} else {
			setCurrentFile(filename);
			setWindowModified(false);
		}
	}
}
void MainWindow::_importInfo() {
	QString filename = QFileDialog::getOpenFileName(this, tr("Open File"), _currentDirectory,
		tr("TileInfo file (*.tli *.TLI);;UnLZ'ed Commander Keen Episode (*.exe *.EXE)")); // *.tli *.exe
	setCurrentDirectory(filename);
	if(!filename.isEmpty()) {
		if(!reallyClose()) {
			return;
		}
		if(!_tileSet->openClassic(filename)) {
			QMessageBox::critical(this, tr("File Error"), tr("The specified file could not be opened."));
		}
	}
}

bool MainWindow::saveAs() {
	QFileDialog saveDialog(this);
	saveDialog.setDefaultSuffix("tls");
	saveDialog.setWindowTitle(tr("Save File"));
	saveDialog.setDirectory(_currentDirectory);
	saveDialog.setNameFilter(tr("V2 tileset file (*.tls *.TLS)"));
	saveDialog.setFileMode(QFileDialog::AnyFile);
	saveDialog.setAcceptMode(QFileDialog::AcceptSave);
	QString filename;
	if(saveDialog.exec()) {
		filename = saveDialog.selectedFiles()[0];
	} else {
		return false;
	}
	//QString filename = QFileDialog::getSaveFileName(this, tr("Save File"), _currentDirectory, tr("V2 tileset file (*.tls *.TLS)"));
	setCurrentDirectory(filename);
	if(!filename.isEmpty()) {
		if(!_tileSet->save(filename)) {
			QMessageBox::critical(this, tr("File Error"), tr("The specified file could not be saved to."));
		} else {
			setCurrentFile(filename);
			setWindowModified(false);
			return true;
		}
	}
	return false;
}
bool MainWindow::save() {
	if(_currentFile.isEmpty() || _currentFile.endsWith(".exe", Qt::CaseInsensitive)) {
		return saveAs();
	}
	if(!_tileSet->save(_currentFile)) {
		QMessageBox::critical(this, tr("File Error"), tr("The file could not be saved."));
		return false;
	} else {
		setWindowModified(false);
		return true;
	}
	return false;
}
void MainWindow::importBitmap() {
	QString filename = QFileDialog::getOpenFileName(this, tr("Open Image"), _currentDirectory,
		tr("Image files (*.bmp *.BMP *.png *.PNG *.tiff *.TIFF)"));
	setCurrentDirectory(filename);
	if(!filename.isEmpty()) {
		if(!reallyClose()) {
			return;
		}
		if(!_tileSet->importImage(filename)) {
			QMessageBox::critical(this, tr("File Error"),
				tr("The specified image file could not be imported. Make sure the width and height are divisible by 16."));
		}
	}
}

void MainWindow::importEgaHead() {
	QString filename = QFileDialog::getOpenFileName(this, tr("Open EGAHEAD"), _currentDirectory, tr("EGAHEAD Files (EGAHEAD.CK? egahead.ck?)"));
	setCurrentDirectory(filename);
	if(!filename.isEmpty()) {
		if(!reallyClose()) {
			return;
		}
		if(!_tileSet->importEgaHead(filename)) {
			QMessageBox::critical(this, tr("File Error"),
				tr("The specified file could not be imported. Note that Keen1 compressed files are not supported at this time."));
		}
	}
}
void MainWindow::exportBitmap() {
	QString filename = QFileDialog::getSaveFileName(this, tr("Save Image"), _currentDirectory, tr("Image files (*.bmp *.png *.tiff)"));
	setCurrentDirectory(filename);
	if(!filename.isEmpty()) {
		if(!_tileSet->tileGfx()->image(_wrap).save(filename)) {
			QMessageBox::critical(this, tr("File Error"), tr("The specified image file could not be exported to."));
		}
	}
}
void MainWindow::exportForEditor() {
	QString filename = QFileDialog::getSaveFileName(this, tr("Save Image"), _currentDirectory, tr("Image files (*.bmp *.png *.tiff)"));
	setCurrentDirectory(filename);
	if(!filename.isEmpty()) {
        QImage dump = _mainView->dumpView().toImage().convertToFormat(QImage::Format_Indexed8, _tileSet->getPaletteRow(0));
		if(!dump.save(filename)) {
			QMessageBox::critical(this, tr("File Error"), tr("The specified image file could not be exported to."));
		}
	}
}
void MainWindow::fixPalette() {
	int button = QMessageBox::question(this, tr("Fix Palette"), tr("Use standard EGA palette?"), QMessageBox::Yes | QMessageBox::No);
	if(button == QMessageBox::Yes) {
		if(!reallyClose()) {
			return;
		}
		_tileSet->fixPalette();
	} else {
		QString filename = QFileDialog::getOpenFileName(this, tr("Open Palette"), _currentDirectory, tr("Image and palette files (*.bmp *.BMP *.png *.PNG *.tiff *.TIFF *.bin *.BIN *.dat *.DAT);;Image files (*.bmp *.BMP *.png *.PNG *.tiff *.TIFF);;Raw VGA palettes (*.bin *.BIN *.dat *.DAT)"));
		if(!filename.isEmpty()) {
			if(!reallyClose()) {
				return;
			}
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
		if(!reallyClose()) {
			return;
		}
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

void MainWindow::preferences() {
	_prefsDialog->wrapping = _wrap;
	if(_prefsDialog->exec() == QDialog::Accepted) {
		_setWrap(_prefsDialog->wrapping);
	}
}

void MainWindow::tileSelected(int i) {
	_currentTileIndicator->setText(tr("Tile: %1/910 Hex: 0x%2").arg(i+1).arg(i, 4, 16, QLatin1Char( '0' )));
	_propertyEditor->setCurrentModelIndex(_tileSet->tileInfo()->index(i, 0));
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

	importEgaHeadAction = new QAction(QIcon(":/images/import-ega.png"), tr("&Import EGAHEAD..."), this);
	connect(importEgaHeadAction, SIGNAL(triggered()), this, SLOT(importEgaHead()));

	importInfoAction = new QAction(QIcon(":/images/import-info.png"), tr("&Import TileInfo..."), this);
	connect(importInfoAction, SIGNAL(triggered()), this, SLOT(_importInfo()));

	exportBitmapAction = new QAction(QIcon(":/images/export-image.png"), tr("&Export image..."), this);
	connect(exportBitmapAction, SIGNAL(triggered()), this, SLOT(exportBitmap()));

	exportForEditorAction = new QAction(QIcon(":/images/leveleditor.png"), tr("&Export for level editor..."), this);
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


	prefAction = new QAction(QIcon(":/images/grid.png"), tr("Change tileset width..."), this);
	connect(prefAction, SIGNAL(triggered()), this, SLOT(preferences()));

	zoomInAction = new QAction(QIcon(":/images/zoomin.png"), tr("Zoom in"), this);
	connect(zoomInAction, SIGNAL(triggered()), this, SLOT(_zoomIn()));
	zoomOutAction = new QAction(QIcon(":/images/zoomout.png"), tr("Zoom out"), this);
	connect(zoomOutAction, SIGNAL(triggered()), this, SLOT(_zoomOut()));
}
void MainWindow::_setZoom(int i) {
	if(i < MIN_ZOOM) {
		i = MIN_ZOOM;
	}
	if(i > MAX_ZOOM) {
		i = MAX_ZOOM;
	}
	if(i == MIN_ZOOM) {
		zoomOutAction->setDisabled(true);
	} else if(i == MAX_ZOOM) {
		zoomInAction->setDisabled(true);
	} else {
		zoomOutAction->setDisabled(false);
		zoomInAction->setDisabled(false);
	}
	_zoom = i;
	_mainView->setZoom(_zoom);
	_animEditor->framePicker()->setZoom(_zoom);
	_animEditor->setZoom(_zoom+2);
}
void MainWindow::_setWrap(int i) {
	_wrap = i;
	_mainView->setWrap(_wrap);
	_animEditor->framePicker()->setWrap(_wrap);
}
void MainWindow::_zoomIn(void) {
	_setZoom(_zoom + 1);
}
void MainWindow::_zoomOut(void){
	_setZoom(_zoom - 1);
}


void MainWindow::createMenus() {
	QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(newAction);
	fileMenu->addAction(openAction);
	fileMenu->addAction(saveAction);
	fileMenu->addAction(saveAsAction);
	fileMenu->addSeparator();
	fileMenu->addAction(quitAction);

	QMenu* portMenu = menuBar()->addMenu(tr("Im&port/Export"));
	portMenu->addAction(importBitmapAction);
	portMenu->addAction(exportBitmapAction);
	portMenu->addSeparator();
	portMenu->addAction(importPaletteAction);
	portMenu->addAction(exportPaletteAction);
	portMenu->addSeparator();
	portMenu->addAction(importInfoAction);
	portMenu->addAction(importEgaHeadAction);


	QMenu* toolMenu = menuBar()->addMenu(tr("&Tools"));
	toolMenu->addAction(exportForEditorAction);
	toolMenu->addAction(fixPaletteAction);
	toolMenu->addAction(setOneToOneAction);

	QMenu* viewMenu = menuBar()->addMenu(tr("&View"));
	viewMenu->addAction(zoomInAction);
	viewMenu->addAction(zoomOutAction);
	viewMenu->addAction(prefAction);

	menuBar()->addSeparator();

	QMenu* helpMenu = menuBar()->addMenu(tr("&Help"));
	helpMenu->addAction(helpAction);
	helpMenu->addAction(aboutAction);
}
void MainWindow::createToolBars() {
	QToolBar* fileToolBar = addToolBar(tr("File"));
	fileToolBar->setObjectName("fileToolbar");
	fileToolBar->addAction(newAction);
	fileToolBar->addAction(openAction);
	fileToolBar->addAction(saveAction);

	QToolBar* portBar = addToolBar(tr("Import/Export"));
	portBar->setObjectName("portBar");
	portBar->addAction(importBitmapAction);
	portBar->addAction(exportBitmapAction);
	portBar->addAction(importPaletteAction);
	portBar->addAction(exportPaletteAction);
	portBar->addAction(importEgaHeadAction);
	portBar->addAction(importInfoAction);


	QToolBar* toolsBar = addToolBar(tr("&Tools"));
	toolsBar->setObjectName("toolsBar");
	toolsBar->addAction(exportForEditorAction);
	toolsBar->addAction(fixPaletteAction);
	toolsBar->addAction(setOneToOneAction);

	QToolBar* viewBar = addToolBar(tr("&View"));
	viewBar->setObjectName("viewBar");
	viewBar->addAction(zoomInAction);
	viewBar->addAction(zoomOutAction);
	viewBar->addAction(prefAction);

	QToolBar* helpBar = addToolBar(tr("&Help"));
	helpBar->setObjectName("helpBar");
	helpBar->addAction(helpAction);
	helpBar->addAction(aboutAction);

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
		switch(QMessageBox::question(this, tr("Save Changes"),
			tr("If you continue, your recent changes may be lost! Would you like to save first?"),
			QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Yes)) {
			case QMessageBox::Yes:
				return save();
			case QMessageBox::No:
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
	_settings->beginGroup("View");
		_setZoom(_settings->value("Scale", 2).toInt());
		_setWrap(_settings->value("Width", 13).toInt());
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
	_settings->beginGroup("View");
		_settings->setValue("Scale", _zoom);
		_settings->setValue("Width", _wrap);
		_settings->endGroup();
	_settings->setValue("Working_Directory", _currentDirectory);
}

MainWindow::MainWindow(QWidget * parent, Qt::WindowFlags flags): QMainWindow(parent, flags) {
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
	_prefsDialog = new PreferencesDialog(this);

	_currentTileIndicator = new QLabel(tr("Tile: %1/910 Hex: 0x%2").arg(1).arg(0, 4, 16, QLatin1Char( '0' )));
	connect(_mainView, SIGNAL(tileSelected(int)), this, SLOT(tileSelected(int)));
	statusBar()->addWidget(_currentTileIndicator);

	_mainView->setCurrentIndex(_tileSet->tileAnim()->index(0,0));

	connect(_mainView, SIGNAL(doubleClicked(const QModelIndex&)), _animEditor, SLOT(pickTile(const QModelIndex&)));
	connect(_tileSet, SIGNAL(modificationStateChanged(bool)), this, SLOT(setWindowModified(bool)));
}
MainWindow::~MainWindow() {
	_saveSettings();
	delete _helpViewer;
}
void MainWindow::setOneToOne() {
	if(reallyClose()) {
		QProgressDialog progress(tr("Setting frames..."), tr("Cancel"), 0, _tileSet->tileAnim()->rowCount()*_tileSet->tileAnim()->columnCount(), this);
		progress.setWindowTitle("Operation in Progress");
		progress.setWindowModality(Qt::WindowModal);
        progress.show();
		for(int tile=0;tile<_tileSet->tileAnim()->rowCount(); ++tile) {
			for(int frame=0;frame<_tileSet->tileAnim()->columnCount();++frame) {
                //if(tile < _tileSet->tileGfx()->rowCount()) {
                _tileSet->tileAnim()->setData(_tileSet->tileAnim()->index(tile, frame), tile);
                //}
				progress.setValue((tile * _tileSet->tileAnim()->columnCount())+frame);
				if(progress.wasCanceled()) {
					tile = _tileSet->tileAnim()->rowCount();
					break;
				}
			}
		}
		progress.setValue(_tileSet->tileGfx()->rowCount()*_tileSet->tileAnim()->columnCount());
	}
}
