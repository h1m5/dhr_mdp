#ifndef EDITOR_H
#define EDITOR_H

#include <QDialog>
#include "newcity.h"
#include "multigraph.h"
#include "transport.h"
#include <QTreeWidgetItem>

namespace Ui {
    class Editor;
}

class Editor : public QDialog
{
    Q_OBJECT

public:
    explicit Editor(MultiGraph<City, Transport> &mgraph, QWidget *parent = 0);
    ~Editor();

public slots:
    void on_pushButton_clicked();

private slots:
    void on_listWidget_activated(const QModelIndex &index);

private:
    Ui::Editor *ui;
    NewCity _newCityDialog;

    MultiGraph<City,Transport> &_graph;

    void AddRoot(const HGraphArc<City, Transport> &arc);
    void AddChild(QTreeWidgetItem* parent, QString name, QString cost);
};

#endif // EDITOR_H
