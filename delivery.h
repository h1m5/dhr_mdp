#ifndef DELIVERY_H
#define DELIVERY_H

#include "multigraph.h"
#include "transport.h"
#include "widget.h"
#include "editor.h"
#include <QMainWindow>

namespace Ui {
    class Delivery;
}

class Delivery : public QMainWindow, public Subscriber
{
    Q_OBJECT

public:
    explicit Delivery(QWidget *parent = 0);
    ~Delivery();

    void Serialize();
    void Deserialize();
    void Update();

private slots:
    void on_actionEdit_Cities_triggered();

private:
    Ui::Delivery *ui;
    MultiGraph<City, Transport> m_graph;
    Widget *_centralWidget;
    Editor *_editor;
    void LoadDefault();
};

#endif // DELIVERY_H
