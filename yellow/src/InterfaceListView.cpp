#include "InterfaceListView.h"

InterfaceListView::InterfaceListView(QWidget* parent)
: QWidget(parent), _table(new QTableWidget) {
    _table->setRowCount(10);
    _table->setColumnCount(1);

    //QTableWidgetItem puts data into each cell. Just an example
    for (int i = 0; i < 10; i++){
            QTableWidgetItem *item = new QTableWidgetItem(tr("%1").arg(i));
            _table->setItem(i, i, item);
    }

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(_table.get());

    setLayout(layout);
}


// model->setHeaderData(0, Qt::Horizontal, QObject::tr("Name"));
// model->setHeaderData(1, Qt::Horizontal, QObject::tr("Addr"));
// model->setHeaderData(2, Qt::Horizontal, QObject::tr("Netmask"));
// model->setHeaderData(3, Qt::Horizontal, QObject::tr("Broadcast"));
//
// QTableView *ifa_view = new QTableView;
// ifa_view->setModel(model);
// ifa_view->setEditTriggers(QAbstractItemView::NoEditTriggers);
// ifa_view->show();
