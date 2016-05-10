#include "editor.h"
#include "ui_editor.h"
#include <QDebug>
#include "ui_newcity.h"
#include <QList>
typedef MultiGraphIterator<City,Transport> iterator;

Editor::Editor(MultiGraph<City,Transport> &mgraph, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Editor),
    _graph(mgraph)
{
    ui->setupUi(this);
    ui->treeWidget->setColumnCount (2);

    iterator itr = _graph.Iterator ();
    for(itr.begin (); itr.isValid (); ++itr){
        ui->listWidget->addItem ((*itr)->m_data.name());
    }
}

Editor::~Editor()
{
    delete ui;
}

void Editor::on_pushButton_clicked()
{
    _newCityDialog.exec ();
    qDebug() << _newCityDialog.cityName ();

    //Insert new City object into graph
    if(_graph.GetNode (_newCityDialog.cityName ()) == NULL){
        _graph.AddNode (QString(_newCityDialog.cityName ()));
        ui->listWidget->addItem (_newCityDialog.cityName ());
    }
}

void Editor::AddRoot(const HGraphArc<City,Transport> &arc)
{
    QString rootName = arc.m_node->m_data.name ();
    QString childName = arc.m_weight.tpMeansString ();

    //reuse item if it already exists
    QTreeWidgetItem *itm = NULL;
    if(!ui->treeWidget->findItems(rootName, Qt::MatchExactly).isEmpty ())
        itm = ui->treeWidget->findItems(rootName, Qt::MatchExactly).at (0);
    if(itm == NULL)
        itm = new QTreeWidgetItem(ui->treeWidget);
    itm->setText (0, rootName);
    ui->treeWidget->addTopLevelItem (itm);

    AddChild (itm, childName, QString::number (arc.m_weight.cost ()));
}

void Editor::AddChild(QTreeWidgetItem *parent, QString name, QString cost)
{
    QTreeWidgetItem *itm = new QTreeWidgetItem();
    itm->setText (0, name);
    itm->setText (1, cost);
    parent->addChild (itm);
}

void Editor::on_listWidget_activated(const QModelIndex &index)
{
    ui->treeWidget->clear ();
    HGraphNode<City,Transport> *currentNode = _graph.GetNode (index.data ().toString ());
    if(currentNode != NULL)
    {
        for(int i=0; i<currentNode->m_arcList.size (); ++i)
        {
//            AddRoot (currentNode->m_arcList.at (i).m_node->m_data.name (), currentNode->m_arcList.at (i).m_weight.tpMeansString ());
            AddRoot (currentNode->m_arcList.at (i));
        }
    }
}
