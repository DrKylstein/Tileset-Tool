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
/*QVariant TilePaletteModel::headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const {

}*/
QVariant TilePaletteModel::data (const QModelIndex & index, int role) const{
	if(!index.isValid()) return QVariant();
	if(role == Qt::BackgroundRole) {
		return QVariant( QBrush( QColor( _parent->_graphics.color( (index.row() * 16) + index.column() ) ) ) );
	} else if(role == Qt::EditRole) {
		return QVariant( QColor( _parent->_graphics.color( (index.row() * 16) + index.column() ) ) );
	}
	return QVariant();
}
Qt::ItemFlags TilePaletteModel::flags (const QModelIndex & index) const {
	if(!index.isValid()) return Qt::ItemIsEnabled;
	return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}
bool TilePaletteModel::setData (const QModelIndex & index, const QVariant & value, int role) {
	if(!index.isValid()) return false;
	_parent->_graphics.setColor( (index.row() * 16) + index.column(), value.value<QColor>().rgba() );
	emit dataChanged(index, index);
	return true;
}

void TilePaletteModel::markAllNew(void) {
	emit layoutChanged();
	emit dataChanged(createIndex(0, 0), createIndex(rowCount() - 1, columnCount() - 1));
}
QVector<QRgb> TilePaletteModel::dumpPage(int page) {
    QVector<QRgb> dump;
    for(int i = page*16; i < (page+1)*16; ++i) {
        dump <<  _parent->_graphics.color(i);
    }
    return dump;
}