#include "delivery.h"
#include "ui_delivery.h"
#include "widget.h"
#include "editor.h"
#include <QFile>
#include <QMap>

Delivery::Delivery(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Delivery)
{
    ui->setupUi(this);
    setCentralWidget (new Widget);


    Deserialize ();

    MultiGraph<City,Transport> gr2(m_graph);
}

Delivery::~Delivery()
{
    delete ui;
}

void Delivery::LoadDefault()
{
    m_graph.AddNode (City("Moscow"));
    m_graph.AddNode (City("Petersburg"));
    m_graph.AddNode (City("Rostov"));

    m_graph.AddArc (QString("Moscow"), QString("Petersburg"), Road(54));
    m_graph.AddArc (QString("Moscow"), QString("Petersburg"), Air(14));
    m_graph.AddArc (QString("Moscow"), QString("Rostov"), Air(12));
}

void Delivery::Serialize()
{
    QFile file("/Users/HimsDLee/Desktop/aFile.txt");

    if(!file.open (QIODevice::WriteOnly))
    {
        qDebug() << "Couldn't open file";
        return;
    }

    QDataStream out(&file);
    out.setVersion (QDataStream::Qt_5_5);

//    MultiGraphIterator<City, Transport> itr = m_graph.Iterator ();

    out << m_graph;

    file.flush ();
    file.close ();
}

void Delivery::Deserialize()
{
    QFile file("/Users/HimsDLee/Desktop/aFile.txt");

    if(!file.open (QIODevice::ReadOnly))
    {
        qDebug() << "Couldn't open file";
        LoadDefault ();
        return;
    }

    QDataStream in(&file);
    in.setVersion (QDataStream::Qt_5_5);

    in >> m_graph;
    qDebug() << m_graph.m_nodes.size ();
    file.close ();

    qDebug() << "done";
}

void Delivery::on_actionEdit_Cities_triggered()
{
    Editor e(m_graph);
    e.exec ();

    Serialize ();
    Deserialize ();
}
