#ifndef EDITOR_H
#define EDITOR_H

#include <QDialog>
#include "newcity.h"
#include "multigraph.h"
#include "transport.h"
#include <QTreeWidgetItem>
#include <QListWidget>
#include "publisher.h"

namespace Ui {
    class Editor;
}

class Editor : public QDialog, public Publisher
{
    Q_OBJECT

public:
    explicit Editor(MultiGraph<City, Transport> &mgraph, QWidget *parent = 0);
    ~Editor();
    void notifySubscribers();

public slots:
    void on_pushButton_clicked();

private slots:

    void on_editButton_toggled(bool checked);

    void on_arcEditOkButton_clicked();

    void on_listWidget_currentRowChanged(int currentRow);

    void on_removeButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Editor *ui;
    NewCity _newCityDialog;

    MultiGraph<City,Transport> &_graph;

    void AddRoot(const HGraphArc<City, Transport> &arc);
    void AddChild(QTreeWidgetItem* parent, QString name, QString cost);
    void AddToListWidget(QListWidget *listWidget);
};

#endif // EDITOR_H
