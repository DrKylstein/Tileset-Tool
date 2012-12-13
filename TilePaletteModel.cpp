#include "TilePaletteModel.hpp"
#include "TileModel.hpp"
#include <QtGui>

static const QRgb CGA_COLORS[256] = {
	0xFF000000, 0xFF0000AA, 0xFF00AA00, 0xFF00AAAA, 0xFFAA0000, 0xFFAA00AA, 0xFFAA5500, 0xFFAAAAAA,
	0xFF555555, 0xFF5555FF, 0xFF55FF55, 0xFF55FFFF, 0xFFFF5555, 0xFFFF55FF, 0xFFFFFF55, 0xFFFFFFFF,

	0xFF000000, 0xFF0000AA, 0xFF00AA00, 0xFF00AAAA, 0xFFAA0000, 0xFFAA00AA, 0xFFAA5500, 0xFFAAAAAA,
	0xFF555555, 0xFF5555FF, 0xFF55FF55, 0xFF55FFFF, 0xFFFF5555, 0xFFFF55FF, 0xFFFFFF55, 0xFFFFFFFF,

	0xFF000000, 0xFF0000AA, 0xFF00AA00, 0xFF00AAAA, 0xFFAA0000, 0xFFAA00AA, 0xFFAA5500, 0xFFAAAAAA,
	0xFF555555, 0xFF5555FF, 0xFF55FF55, 0xFF55FFFF, 0xFFFF5555, 0xFFFF55FF, 0xFFFFFF55, 0xFFFFFFFF,

	0xFF000000, 0xFF0000AA, 0xFF00AA00, 0xFF00AAAA, 0xFFAA0000, 0xFFAA00AA, 0xFFAA5500, 0xFFAAAAAA,
	0xFF555555, 0xFF5555FF, 0xFF55FF55, 0xFF55FFFF, 0xFFFF5555, 0xFFFF55FF, 0xFFFFFF55, 0xFFFFFFFF,

	0xFF000000, 0xFF0000AA, 0xFF00AA00, 0xFF00AAAA, 0xFFAA0000, 0xFFAA00AA, 0xFFAA5500, 0xFFAAAAAA,
	0xFF555555, 0xFF5555FF, 0xFF55FF55, 0xFF55FFFF, 0xFFFF5555, 0xFFFF55FF, 0xFFFFFF55, 0xFFFFFFFF,

	0xFF000000, 0xFF0000AA, 0xFF00AA00, 0xFF00AAAA, 0xFFAA0000, 0xFFAA00AA, 0xFFAA5500, 0xFFAAAAAA,
	0xFF555555, 0xFF5555FF, 0xFF55FF55, 0xFF55FFFF, 0xFFFF5555, 0xFFFF55FF, 0xFFFFFF55, 0xFFFFFFFF,

	0xFF000000, 0xFF0000AA, 0xFF00AA00, 0xFF00AAAA, 0xFFAA0000, 0xFFAA00AA, 0xFFAA5500, 0xFFAAAAAA,
	0xFF555555, 0xFF5555FF, 0xFF55FF55, 0xFF55FFFF, 0xFFFF5555, 0xFFFF55FF, 0xFFFFFF55, 0xFFFFFFFF,

	0xFF000000, 0xFF0000AA, 0xFF00AA00, 0xFF00AAAA, 0xFFAA0000, 0xFFAA00AA, 0xFFAA5500, 0xFFAAAAAA,
	0xFF555555, 0xFF5555FF, 0xFF55FF55, 0xFF55FFFF, 0xFFFF5555, 0xFFFF55FF, 0xFFFFFF55, 0xFFFFFFFF,
//dark
	0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000,
	0xFF000000, 0xFF0000AA, 0xFF00AA00, 0xFF00AAAA, 0xFFAA0000, 0xFFAA00AA, 0xFFAA5500, 0xFFAAAAAA,

	0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000,
	0xFF000000, 0xFF0000AA, 0xFF00AA00, 0xFF00AAAA, 0xFFAA0000, 0xFFAA00AA, 0xFFAA5500, 0xFFAAAAAA,

	0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000,
	0xFF000000, 0xFF0000AA, 0xFF00AA00, 0xFF00AAAA, 0xFFAA0000, 0xFFAA00AA, 0xFFAA5500, 0xFFAAAAAA,

	0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000,
	0xFF000000, 0xFF0000AA, 0xFF00AA00, 0xFF00AAAA, 0xFFAA0000, 0xFFAA00AA, 0xFFAA5500, 0xFFAAAAAA,

	0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000,
	0xFF000000, 0xFF0000AA, 0xFF00AA00, 0xFF00AAAA, 0xFFAA0000, 0xFFAA00AA, 0xFFAA5500, 0xFFAAAAAA,

	0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000,
	0xFF000000, 0xFF0000AA, 0xFF00AA00, 0xFF00AAAA, 0xFFAA0000, 0xFFAA00AA, 0xFFAA5500, 0xFFAAAAAA,

	0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000,
	0xFF000000, 0xFF0000AA, 0xFF00AA00, 0xFF00AAAA, 0xFFAA0000, 0xFFAA00AA, 0xFFAA5500, 0xFFAAAAAA,

	0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000,
	0xFF000000, 0xFF0000AA, 0xFF00AA00, 0xFF00AAAA, 0xFFAA0000, 0xFFAA00AA, 0xFFAA5500, 0xFFAAAAAA
};

void TilePaletteModel::reset() {
	for(int i = 0; i < PALETTE_SIZE; ++i) {
		_colors[i] = CGA_COLORS[i];
	}
}
QVector<QRgb> TilePaletteModel::colorTable(int page) const {
	if(page > -1 && page < 16) {
		return _colors.mid(page*16, 16);
	}
	return _colors;
}

TilePaletteModel::TilePaletteModel(QObject* parent): QAbstractTableModel(parent) {
	_colors.fill(0, PALETTE_SIZE);
	reset();
}
int TilePaletteModel::rowCount(const QModelIndex & parent) const {
	return ROWS;
}
int TilePaletteModel::columnCount(const QModelIndex & parent) const {
	return COLUMNS;
}
bool TilePaletteModel::load(QImage& donor) {
	if(donor.isNull()) return false;
	if(donor.colorCount() != 0) {
		for(int i = 0; i < donor.colorCount(); ++i) {
			_colors[i] = donor.color(i);
		}
		if(donor.colorCount() < PALETTE_SIZE) {
			generatePages();
		}
	} else if(donor.width() == COLUMNS && donor.height() == ROWS) {
		for(int y = 0; y < donor.height(); ++y) {
			for(int x = 0; x < donor.width(); ++x){
				_colors[(y * ROWS) + x] = donor.pixel(x,y);
			}
		}
	} else {
		donor = donor.convertToFormat(QImage::Format_Indexed8);
		for(int i = 0; i < donor.colorCount(); ++i) {
			_colors[i] = donor.color(i);
		}
		if(donor.colorCount() < PALETTE_SIZE) {
			generatePages();
		}
	}
	return true;
}
bool TilePaletteModel::load(QDataStream& stream, bool sixteen) {
	quint8 r, g, b;
	int size = PALETTE_SIZE;
	if(sixteen) {
		size = 16;
	}
	for(int i = 0; i < size; ++i) {
		stream >> r;
		stream >> g;
		stream >> b;
		_colors[i] = 0xFF000000 | (r << 18) | (g << 10) | (b << 2);
	}
	if(sixteen) {
		generatePages();
	}
	emit dataChanged(createIndex(0, 0), createIndex(rowCount() - 1, columnCount() - 1));
	return true;
}
void TilePaletteModel::dump(QDataStream& stream) const {
	for(int i = 0; i < PALETTE_SIZE; ++i) {
		stream << quint8( (_colors[i] >> 18) & COLORBITS);
		stream << quint8( (_colors[i] >> 10) & COLORBITS);
		stream << quint8( (_colors[i] >> 2) & COLORBITS);
	}
}

void TilePaletteModel::generatePages(void) {
	for(int row = 0; row < ROWS/2; ++row) {
		for(int column = 0; column < COLUMNS; ++column) {
			_colors[(row*COLUMNS) + column] = _colors[column];
		}
	}
	for(int row = ROWS/2; row < ROWS; ++row) {
		for(int column = 0; column < COLUMNS; ++column) {
			_colors[(row*COLUMNS) + column] = QColor(_colors[column]).darker().rgb();
		}
	}
}

QVariant TilePaletteModel::headerData ( int section, Qt::Orientation orientation, int role) const {
    if(orientation == Qt::Vertical && role == Qt::DisplayRole) {
        if(section < 8) {
            return tr("A%1 0x%2").arg(section%8+1).arg(section, 2, 16, QLatin1Char( '0' ));
        } else {
            return tr("B%1 0x%2").arg(section%8+1).arg(section, 2, 16, QLatin1Char( '0' ));
        }
    }
    return QVariant();
}
QVariant TilePaletteModel::data (const QModelIndex & index, int role) const{
	if(!index.isValid()) return QVariant();
	if(role == Qt::BackgroundRole) {
		return QVariant(QBrush(QColor(_colors[(index.row() * COLUMNS) + index.column()])));
	} else if(role == Qt::EditRole) {
		return QVariant(QColor(_colors[(index.row() * COLUMNS) + index.column()]));
	}
	return QVariant();
}
Qt::ItemFlags TilePaletteModel::flags (const QModelIndex & index) const {
	if(!index.isValid()) return Qt::ItemIsEnabled;
	return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}
bool TilePaletteModel::setData (const QModelIndex & index, const QVariant & value, int role) {
	if(!index.isValid()) return false;
	_colors[(index.row() * COLUMNS) + index.column()] = value.value<QColor>().rgba();
	emit dataChanged(index, index);
	return true;
}
