#ifndef INTERFACELISTVIEW_H
#define INTERFACELISTVIEW_H

#include <memory>

#include <QDialog>
#include <QTableWidgetItem>
#include <QTableWidget>
#include <QHBoxLayout>

class InterfaceListView : public QWidget
{
    Q_OBJECT

public:
    explicit InterfaceListView(QWidget* parent = nullptr);
    virtual ~InterfaceListView() = default;

private:
    std::unique_ptr<QTableWidget> _table;
};

#endif // INTERFACELISTVIEW_H
