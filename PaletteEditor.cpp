#include "PaletteEditor.hpp"
#include <QAbstractItemModel>
#include <QTableView>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QSlider>
#include <QDataWidgetMapper>
#include <QColorDialog>
PaletteEditor::PaletteEditor(QWidget* parent): QWidget(parent) {
	QVBoxLayout* layout = new QVBoxLayout;
	setLayout(layout);
		_table = new QTableView;
		_table->horizontalHeader()->hide();
		layout->addWidget(_table);
	_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
	connect(_table, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(chooseColor(const QModelIndex&)));
}
void PaletteEditor::setModel(QAbstractItemModel* model) {
	_model = model;
	_table->setModel(_model);
	for(int i = 0; i < 16; ++i) {
		_table->setColumnWidth(i, 20);
		_table->setRowHeight(i, 20);
	}
}
void PaletteEditor::chooseColor(const QModelIndex& index) {
	QColor color = QColorDialog::getColor(index.data(Qt::EditRole).value<QColor>());
	if(color.isValid()) {
		_model->setData(index, color);
	}

}
