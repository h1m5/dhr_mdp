#include "visualizer.h"
#include "nodeitem.h"
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QVectorIterator>

Visualizer::Visualizer() : QObject (nullptr)
{
    _scene = new QGraphicsScene;

    QBrush redBrush(Qt::red);
    QBrush blueBrush(Qt::blue);
    QPen blackPen (Qt::black);
}

QGraphicsScene *Visualizer::scene() const
{
    return _scene;
}

void Visualizer::removeDeletedItems(MultiGraph<City, Transport> &graph)
{

    for(int i=0; i<_items.size (); i++){
        if(graph.GetNode (_items[i]->text ()) == NULL){
            _scene->removeItem (_items[i]);
            _items.remove (i);
        }
    }
    _scene->update ();
}

void Visualizer::highlightPath()
{
    if(_path.isEmpty ()){
        return;
    }

    foreach(ArcItem* a, _arcs)
    {
        a->resetPen ();
    }

    for(int i=_path.size ()-1; i>=0; i--)
    {
        NodeItem *sourceItem = getNodeItem (_path.at (i)->m_data.name ());
        NodeItem *destinationItem = NULL;
        if(i>0){
            destinationItem = getNodeItem (_path.at (i-1)->m_data.name ());
            ArcItem *arc = nullptr;
            arc = getArcItem (sourceItem, destinationItem);

            arc->setPenColour (Qt::red);
        }
    }

    _scene->update ();
}

ArcItem *Visualizer::getArcItem(NodeItem *source, NodeItem *destination)
{
    for(int i=0; i<_arcs.size (); i++)
    {
        if((source->text () == _arcs[i]->_source->text ()) &&
                destination->text () == _arcs[i]->_destination->text ())
            return _arcs[i];
    }
    return NULL;
}

NodeItem *Visualizer::getNodeItem(QString name)
{
    for(int i=0; i<_items.size (); i++)
    {
        if(name == _items[i]->text ())
            return _items[i];
    }
    return NULL;
}

void Visualizer::setPath(const QVector<HGraphNode<City, Transport> *> &path)
{
    _path = path;
}

void Visualizer::refresh(MultiGraph<City, Transport> &graph)
{
    int startIndex = _items.size ();
    if(graph.m_nodes.size () <= startIndex) {
//        _scene->clear ();
//        _items.clear ();
//        startIndex = 0;
        removeDeletedItems (graph);
    }
    for(int i=_items.size (); i<graph.m_nodes.size (); i++)
    {
#define cur graph.m_nodes[i]
        QString name = cur->m_data.name();
        NodeItem *anItem = new NodeItem(name);
        int dim = 200;
        anItem->setPos (randReal (-dim, dim), randReal(-dim, dim));
        _items << anItem;
    }

    //for each node item, connect to its children
    for(int i=0; i<_arcs.size (); i++){
        _scene->removeItem (_arcs[i]);
    }
    _arcs.clear ();

    for(int i=0; i<graph.m_count; i++)
    {
        NodeItem *source = _items[i];
        if(!_scene->items ().contains (source))
            _scene->addItem (source);
        for(int j=0; j<graph.m_nodes[i]->m_arcList.size (); j++)
        {
            QString destName = graph.m_nodes[i]->m_arcList[j].m_node->m_data.name ();
            QString arcLabel = graph.m_nodes[i]->m_arcList[j].m_weight.tpMeansString () + ": " + QString::number (graph.m_nodes[i]->m_arcList[j].m_weight.cost ());
            NodeItem *destination = NULL;
            for(int k=0; k<_items.size (); k++)
            {
                if(destName == _items[k]->text()){
                    destination = _items[k];
                }
            }
            ArcItem *anArcItem = new ArcItem(arcLabel, source, destination);
            anArcItem->setZValue (-1);
            source->subscribe (anArcItem);
            destination->subscribe(anArcItem);
            _arcs << anArcItem;
            _scene->addItem (anArcItem);
        }
    }

    highlightPath ();
    _scene->update ();
}

qreal Visualizer::randReal(int low, int high)
{
    // Random number between low and high
    return qrand() % ((high + 1) - low) + low;
}
