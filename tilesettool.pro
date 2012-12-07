TEMPLATE = app
CONFIG += debug_and_release qt

DESTDIR = ./bin
OBJECTS_DIR = ./obj

RESOURCES = tilesettool.qrc

HEADERS +=	AnimationPreview.hpp \
			SlopeEditor.hpp \
			PreferencesDialog.hpp \
			TileEditor.hpp \
			AnimationEditor.hpp \
			TileSetView.hpp \
			TileModel.hpp \
			MainWindow.hpp \
			TileInfoModel.hpp \
			TileAnimModel.hpp \
			TileGfxModel.hpp \
			TileMapDelegate.hpp \
			TileSheetDelegate.hpp \
			FramePickerDialog.hpp \
			RowToListProxyModel.hpp \
			TilePaletteModel.hpp \
			PaletteEditor.hpp \
			V2TileInfo.hpp


SOURCES +=	AnimationPreview.cpp \
			SlopeEditor.cpp \
			PreferencesDialog.cpp \
			TileEditor.cpp \
			AnimationEditor.cpp \
			TileSetView.cpp \
			TileModel.cpp \
			main.cpp \
			MainWindow.cpp \
			TileInfoModel.cpp \
			TileAnimModel.cpp \
			TileGfxModel.cpp \
			TileMapDelegate.cpp \
			TileSheetDelegate.cpp \
			FramePickerDialog.cpp \
			RowToListProxyModel.cpp \
			TilePaletteModel.cpp \
			PaletteEditor.cpp \
			V2TileInfo.cpp
