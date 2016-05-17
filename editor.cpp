#include "editor.h"
#include "ui_editor.h"
#include <QDebug>
#include "ui_newcity.h"
#include "subscriber.h"
#include <QList>
typedef MultiGraphIterator<City,Transport> iterator;

Editor::Editor(MultiGraph<City,Transport> &mgraph, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Editor),
    _graph(mgraph)
{
    ui->setupUi(this);
    ui->treeWidget->setColumnCount (2);
    ui->cityList->hide ();
    ui->modeOfTransport->hide ();
    ui->arcEditOkButton->hide ();
    ui->costEdit->hide ();
    ui->costLabel->hide ();
    ui->removeButton->setDisabled (true);

    QPalette pal = ui->closeButton->palette();
    pal.setColor(QPalette::Button, QColor(121, 140, 210));
    ui->closeButton->setAutoFillBackground(true);
    ui->closeButton->setPalette(pal);
    ui->closeButton->update();

    layout ()->setSizeConstraint (QLayout::SetFixedSize);

    if(!_graph.isEmpty ()){
        AddToListWidget (ui->listWidget);
        on_listWidget_currentRowChanged (ui->listWidget->indexAt (QPoint(0,0)).row ());
    }
}

Editor::~Editor()
{
    delete ui;
}

void Editor::notifySubscribers()
{
    foreach(Subscriber *s, _subscribers){
        s->Update ();
    }
}

void Editor::AddToListWidget(QListWidget* listWidget)
{
    iterator itr = _graph.Iterator ();
    for(itr.begin (); itr.isValid (); ++itr){
        listWidget->addItem ((*itr)->m_data.name());
    }
    listWidget->itemAt (0,0)->setSelected (true);
}

void Editor::on_pushButton_clicked()
{
    _newCityDialog.exec ();
    qDebug() << _newCityDialog.cityName ();

    if(_newCityDialog.cityName () == "")
        return;

    //Insert new City object into graph
    if(_graph.GetNode (_newCityDialog.cityName ()) == NULL){
        _graph.AddNode (QString(_newCityDialog.cityName ()));
        ui->listWidget->addItem (_newCityDialog.cityName ());
    }

    notifySubscribers ();
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

    ui->treeWidget->expandAll ();
}

void Editor::on_editButton_toggled(bool checked)
{
    ui->cityList->setVisible (checked);
    ui->cityList->setFocus ();
    ui->modeOfTransport->setVisible (checked);
    ui->arcEditOkButton->setVisible (checked);
    ui->costEdit->setVisible (checked);
    ui->costLabel->setVisible (checked);
    ui->removeButton->setEnabled (checked);
    ui->groupBox_2->setHidden (checked);

    ui->cityList->clear ();
    AddToListWidget (ui->cityList);
//    ui->cityList->takeItem (ui->listWidget->currentIndex ().row ());
    ui->cityList->item (ui->listWidget->currentIndex ().row ())->setHidden (true);

    qDebug() << "is here" << checked;
    if(checked)
        ui->editButton->setText (tr("Done"));
    else
        ui->editButton->setText (tr("Edit"));

}

void Editor::on_arcEditOkButton_clicked()
{

    ui->cityList->setFocus ();
    QListWidgetItem *currenitem = ui->cityList->currentItem ();
    if(currenitem){
        QString cityName = ui->cityList->currentItem ()->text ();
        QString modeOfTransport = ui->modeOfTransport->currentText ();
        int cost = ui->costEdit->text ().toInt ();

        if(ui->listWidget->currentItem () != nullptr){
            _graph.AddArc (ui->listWidget->currentItem ()->text (), cityName, Transport::create (modeOfTransport, cost));
            on_listWidget_currentRowChanged (ui->listWidget->currentIndex ().row ());
        }
    }

    notifySubscribers ();
}

void Editor::on_listWidget_currentRowChanged(int currentRow)
{
//    QModelIndex index = ui->listWidget->item (currentRow);
    QListWidgetItem *item = ui->listWidget->item (currentRow);
    ui->treeWidget->clear ();

    if(item){
        HGraphNode<City,Transport> *currentNode = _graph.GetNode (item->data (0).toString ());
        if(currentNode != NULL)
        {
            for(int i=0; i<currentNode->m_arcList.size (); ++i)
            {
                AddRoot (currentNode->m_arcList.at (i));
            }
        }
    }
}

void Editor::on_removeButton_clicked()
{
    QTreeWidgetItem *item = ui->treeWidget->currentItem ();
    if(item){
        City from(ui->listWidget->currentItem ()->text ());
        //if item is expandable, it means we've selected a city. Else we've selected a means of transportations
        if(item->childCount () != 0){
            QString cityName = item->text (0);
            _graph.RemoveArc (from, cityName);
        } else if(item->childCount () == 0){
            QString cityName = item->parent ()->text (0);
            _graph.RemoveArc (from, cityName, Transport::create (item->text (0), item->text (1).toInt ()));
        }
        on_listWidget_currentRowChanged (ui->listWidget->currentIndex ().row ());
    }

    notifySubscribers ();
}

void Editor::on_pushButton_2_clicked()
{
    //remove selected city
    QListWidgetItem *item = ui->listWidget->currentItem ();
    if(item){
        City current(item->text ());
        _graph.RemoveNode (current);
        ui->listWidget->takeItem (ui->listWidget->currentRow ());
        notifySubscribers ();
    }
}
