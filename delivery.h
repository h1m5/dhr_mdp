#ifndef DELIVERY_H
#define DELIVERY_H

#include "multigraph.h"
#include "transport.h"
#include <QMainWindow>

namespace Ui {
    class Delivery;
}

class Delivery : public QMainWindow
{
    Q_OBJECT

public:
    explicit Delivery(QWidget *parent = 0);
    ~Delivery();

    void Serialize();
    void Deserialize();

private slots:
    void on_actionEdit_Cities_triggered();

private:
    Ui::Delivery *ui;
    MultiGraph<City, Transport> m_graph;
};

#endif // DELIVERY_H
