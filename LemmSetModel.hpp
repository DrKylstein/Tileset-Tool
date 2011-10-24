#ifndef LEMMSETMODEL_HPP
#define LEMMSETMODEL_HPP
#include <QAbstractItemModel>
class LemmSetModel: public QAbstractItemModel {
	Q_OBJECT

	public:
		int rowCount(const QModelIndex & parent = QModelIndex()) const;
		int columnCount(const QModelIndex & parent = QModelIndex()) const;
		QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
		QVariant data (const QModelIndex & index, int role = Qt::DisplayRole) const;
		Qt::ItemFlags flags (const QModelIndex & index) const;
		bool setData (const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

	private:
		quint16 tileCount, frameCount, gfxLength;
		qint8 behavior[910];
		qint8 top[910];
		qint8 left[910];
		qint8 bottom[910];
		qint8 right[910];

};
#endif

