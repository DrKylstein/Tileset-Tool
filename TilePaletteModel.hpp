#ifndef TILEPALETTEMODEL_HPP
#define TILEPALETTEMODEL_HPP

#include <QAbstractTableModel>
#include <QColor>
class TileModel;
class TilePaletteModel: public QAbstractTableModel {
	public:
		int rowCount(const QModelIndex & parent = QModelIndex()) const;
		int columnCount(const QModelIndex & parent = QModelIndex()) const;
		//QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
		QVariant data (const QModelIndex & index, int role = Qt::DisplayRole) const;
		Qt::ItemFlags flags (const QModelIndex & index) const;
		bool setData (const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
		TilePaletteModel(TileModel* parent);
		void markAllNew(void);

	private:
		TileModel* _parent;
};

#endif // TILEPALETTEMODEL_HPP
