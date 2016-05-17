#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "visualizer.h"
#include "multigraph.h"
#include "transport.h"
#include "subscriber.h"
#include "publisher.h"

namespace Ui {
    class Widget;
}

class Widget : public QWidget, public Subscriber
{
    Q_OBJECT
public:
    explicit Widget(MultiGraph<City,Transport> &gr, Publisher *publisher, QWidget *parent = 0);
    void Update();
signals:

public slots:

private slots:
    void on_pushButton_clicked();

private:
    Ui::Widget *ui;
    Visualizer *_visualizer;
    MultiGraph<City,Transport> &m_graph;

    void loadCities();
};

#endif // WIDGET_H
