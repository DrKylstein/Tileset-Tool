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

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP
#include <QMainWindow>
#include <QModelIndex>
#include <QVariant>
#include <QVector>
class QMenu;
class QAction;
class QToolBar;
class QSettings;
class TileModel;
class TileMapDelegate;
class TileEditor;
class AnimationEditor;
class TileSetView;
class PreferencesDialog;
class QLabel;
class QToolButton;
class QTableView;
class QUndoStack;
class QTextBrowser;
class PaletteEditor;
class MainWindow: public QMainWindow {
	Q_OBJECT

	public:
		MainWindow(QWidget * parent = 0, Qt::WindowFlags flags = 0);
		~MainWindow();

	private slots:
		void startNew();
		void open();
		bool save();
		bool saveAs();

		void setOneToOne();
		void _importInfo();
		void importBitmap();
		void importEgaHead();
		void exportBitmap();
		void exportForEditor();
		void preferences();
		void fixPalette();
		void importPalette();
		void exportPalette();

		void about();
		void help();

		void tileSelected(int);

		void _zoomIn(void);
		void _zoomOut(void);

	protected:
		void closeEvent(QCloseEvent* event);

	private:
		void createActions();
		void createMenus();
		void createToolBars();
		//void setFileOpened(bool);
		void setCurrentFile(QString);
		void setCurrentDirectory(QString);
		bool reallyClose();
		void _readSettings();
		void _saveSettings();

		QAction* newAction;
		QAction* openAction;
		QAction* saveAsAction;
		QAction* saveAction;
		QAction* propertiesAction;
		QAction* quitAction;
		QAction* prefAction;
		QAction* zoomInAction;
		QAction* zoomOutAction;
		QAction* setOneToOneAction;
		QAction* importInfoAction;
		QAction* importBitmapAction;
		QAction* importEgaHeadAction;
		QAction* exportBitmapAction;
		QAction* exportForEditorAction;
		QAction* fixPaletteAction;
		QAction* importPaletteAction;
		QAction* exportPaletteAction;
		QAction* aboutAction;
		QAction* helpAction;

		QString _currentFile;
		QString _currentDirectory;
		TileModel* _tileSet;
		TileSetView* _mainView;
		TileMapDelegate* _graphicDelegate;
		TileEditor* _propertyEditor;
		AnimationEditor* _animEditor;
		PaletteEditor*	_paletteEditor;
		PreferencesDialog* _prefsDialog;
		QSettings* _settings;
		QLabel* _currentTileIndicator;
		QTextBrowser* _helpViewer;

		int _zoom;
		int _wrap;
		void _setZoom(int);
		void _setWrap(int);

		static const int MAX_ZOOM = 8;
		static const int MIN_ZOOM = 1;
};
#endif
