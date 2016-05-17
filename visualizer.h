#ifndef VISUALIZER_H
#define VISUALIZER_H
#include <QGraphicsScene>
#include <QVector>
#include "nodeitem.h"
#include "multigraph.h"
#include "arcitem.h"
#include "transport.h"

class Visualizer : public QObject
{
    Q_OBJECT
public:
    Visualizer();

    QGraphicsScene *scene() const;
    void refresh(MultiGraph<City,Transport> &graph);
    static qreal randReal(int low, int high);
    void buildGraph(MultiGraph<City,Transport> &graph);
    void removeDeletedItems(MultiGraph<City, Transport> &graph);
    void highlightPath();


    void setPath(const QVector<HGraphNode<City, Transport> *> &path);

private:
    QGraphicsScene *_scene;
    QVector<NodeItem*> _items;
    QVector<ArcItem*> _arcs;
    ArcItem* getArcItem(NodeItem *source, NodeItem *destination);
    NodeItem *getNodeItem(QString name);
    QVector<HGraphNode<City, Transport> *> _path;
};

#endif // VISUALIZER_H
