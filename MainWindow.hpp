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
		void save();
		void saveAs();

		void toggleTop();
		void toggleLeft();
		void toggleBottom();
		void toggleRight();
		void setNormalType();
		void cycleKeyTypes();
		void cycleDoorTypes();
		void setDeadlyType();
		void setMasked();
		void setForeground();

		void setOneToOne();
		void importBitmap();
		void exportBitmap();
		void preferences();
		void fixPalette();
		void importPalette();
		void exportPalette();

		void about();
		void help();

		void tileSelected(int);

		//void dataChanged(const QModelIndex&,const QModelIndex&);
		void _setUnmodified(bool);
		void setPaintMode(bool);
		void _setPaintAnimations(bool);
		void _setPaintProperties(bool);

		void _tileInfoEdited(const QModelIndex&, const QVariant&);
		void _frameEdited(const QModelIndex&, const QVariant&);
		void _presetApplied(int, const QVector<int>&);
		void _frameCountSet(int);

	protected:
		void closeEvent(QCloseEvent* event);
		//void keyPressEvent(QKeyEvent* event);

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

		QMenu* fileMenu;
			QAction* newAction;
			QAction* openAction;
			QAction* saveAsAction;
			QAction* saveAction;
			QAction* propertiesAction;
			QAction* quitAction;

		QMenu* editMenu;
			QAction* undoAction;
			QAction* redoAction;
			QAction* togglePaintModeAction;
			QMenu* paintMenu;
				QAction* togglePaintPropertiesAction;
				QAction* togglePaintAnimationsAction;
			QAction* prefAction;

			QAction* toggleTopAction;
			QAction* toggleLeftAction;
			QAction* toggleBottomAction;
			QAction* toggleRightAction;

			QAction* setNormalTypeAction;
			QAction* cycleKeyTypesAction;
			QAction* cycleDoorTypesAction;
			QAction* setDeadlyTypeAction;
			QAction* setMaskedAction;
			QAction* setForegroundAction;
			QAction* setAnimationAction;



		QMenu* toolMenu;
			QAction* setOneToOneAction;
			QAction* importBitmapAction;
			QAction* exportBitmapAction;
			QAction* fixPaletteAction;
			QAction* importPaletteAction;
			QAction* exportPaletteAction;

		QMenu* helpMenu;
			QAction* aboutAction;
			QAction* helpAction;

		QToolBar* fileToolBar;
		QToolBar* editToolBar;
		QToolButton* paintToggleButton;

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
		bool _paintMode;
		int _brushRow;
		bool _paintAnimations;
		bool _paintProperties;
		QUndoStack* _undoStack;
		QTextBrowser* _helpViewer;
};
#endif
