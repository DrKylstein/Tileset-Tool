#include "TilePaletteModel.hpp"
#include "TileModel.hpp"
#include <QtGui>

TilePaletteModel::TilePaletteModel(TileModel* parent) {
	_parent = parent;
}
int TilePaletteModel::rowCount(const QModelIndex & parent) const {
	return 16;
}
int TilePaletteModel::columnCount(const QModelIndex & parent) const {
	return 16;
}
/*QVariant TilePaletteModel::headerData ( int section, Qt::Orientation orientation, int role) const {
    if(orientation == Qt::Vertical && role == Qt::DisplayRole) {
        if(section < 8) {
            return tr("Light %1 (0x%2)").arg(section%8+1).arg(section, 2, 16, QLatin1Char( '0' ));
        } else {
            return tr("Dark %1 (0x%2)").arg(section%8+1).arg(section, 2, 16, QLatin1Char( '0' ));
        }
    }
    return QVariant();
}*/
QVariant TilePaletteModel::data (const QModelIndex & index, int role) const{
	if(!index.isValid()) return QVariant();
	if(role == Qt::BackgroundRole) {
		return QVariant(QBrush(QColor( _parent->getColor(index.column(), index.row()))));
	} else if(role == Qt::EditRole) {
		return QVariant( QColor( _parent->getColor(index.column(), index.row())));
	}
	return QVariant();
}
Qt::ItemFlags TilePaletteModel::flags (const QModelIndex & index) const {
	if(!index.isValid()) return Qt::ItemIsEnabled;
	return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}
bool TilePaletteModel::setData (const QModelIndex & index, const QVariant & value, int role) {
	if(!index.isValid()) return false;
	_parent->setColor(index.column(), index.row(), value.value<QColor>().rgba());
	emit dataChanged(index, index);
	return true;
}

void TilePaletteModel::markAllNew(void) {
	emit layoutChanged();
	emit dataChanged(createIndex(0, 0), createIndex(rowCount() - 1, columnCount() - 1));
}
