#include "delivery.h"
#include "editor.h"
#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    Widget w;
//    w.show();

    Delivery d;
    d.show ();

//    MultiGraph<City, Transport> ch;

////    ch.AddNode (City("Moscow"));
////    ch.AddNode (City("Petersburg"));
////    ch.AddNode (City("Rostov"));

////    ch.AddArc (QString("Moscow"), QString("Petersburg"), Road(54));
////    ch.AddArc (QString("Moscow"), QString("Petersburg"), Air(12));
////    ch.AddArc (QString("Moscow"), QString("Rostov"), Air(12));

//    Editor e(ch);
//    e.show ();

    return a.exec();
}
