#include "widget.h"
#include "ui_widget.h"

static HGraphNode<City,Transport>* p_destinationCityNode = NULL;
Widget::Widget(MultiGraph<City,Transport> &gr, Publisher *publisher, QWidget *parent) : m_graph(gr), QWidget(parent), ui(new Ui::Widget)
{
//    m_graph = gr;
    ui->setupUi (this);
    _visualizer = new Visualizer;
    ui->graphicsView->setScene (_visualizer->scene ());

    publisher->subscribe (this);

//    loadCities ();
    Update ();
}

void Widget::Update()
{
    loadCities ();
//    qDebug() << "here";
//    _visualizer->removeDeletedItem (m_graph);
    _visualizer->refresh (m_graph);
}

void Widget::loadCities()
{
    ui->cityComboBox->clear ();
    ui->destinationComboBox->clear ();
    if(!m_graph.isEmpty ()){
        MultiGraphIterator<City,Transport> it = m_graph.Iterator ();
        for(it.begin (); it.isValid (); ++it)
        {
            ui->cityComboBox->addItem ((*it)->m_data.name ());
            ui->destinationComboBox->addItem ((*it)->m_data.name ());
        }
    }
}

template<class City, class Transport>
bool process(HGraphNode<City, Transport> *node)
{
    qDebug() << "came to" << node->m_data.name ();
    qDebug() << "looking for" << p_destinationCityNode->m_data.name ();
    if(node->m_data.name () == p_destinationCityNode->m_data.name ()){
        qDebug() << "found:" << node->m_data.name ();
        return true;
    } else {
        return false;
    }
}

void Widget::on_pushButton_clicked()
{
    QString start = ui->cityComboBox->currentText ();
    QString destination = ui->destinationComboBox->currentText ();

    m_graph.clearMarks ();

    HGraphNode<City,Transport> *currentCityNode = m_graph.GetNode (start);
    p_destinationCityNode = m_graph.GetNode (destination);

    QVector<HGraphNode<City,Transport>*> path = m_graph.FindPath (currentCityNode, process);

    _visualizer->setPath(path);
    _visualizer->refresh (m_graph);
}
