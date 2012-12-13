#ifndef TILEPALETTEMODEL_HPP
#define TILEPALETTEMODEL_HPP

#include <QAbstractTableModel>
#include <QColor>
#include <QVector>
class TilePaletteModel: public QAbstractTableModel {
	public:
		void reset();
		void dump(QDataStream&) const;
		bool load(QDataStream&, bool sixteen=false);
		bool load(QImage&);
		void generatePages(void);
		QVector<QRgb> colorTable(int page = -1) const;

		int rowCount(const QModelIndex & parent = QModelIndex()) const;
		int columnCount(const QModelIndex & parent = QModelIndex()) const;
		QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
		QVariant data (const QModelIndex & index, int role = Qt::DisplayRole) const;
		Qt::ItemFlags flags (const QModelIndex & index) const;
		bool setData (const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

		TilePaletteModel(QObject* parent);

	private:
		QVector<QRgb> _colors;
		static const int PALETTE_SIZE = 256;
		static const int COLUMNS = 16;
		static const int ROWS = 16;
		static const int COLORBITS = 0x3F;

};

#endif // TILEPALETTEMODEL_HPP
