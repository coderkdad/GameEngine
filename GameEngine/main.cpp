#include "widget.h"
#include <QApplication>
#include "mainwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    QSurfaceFormat formate;
//    formate.setSamples(16);
//    formate.setDepthBufferSize(24);
//    QSurfaceFormat::setDefaultFormat(formate);

    //Test
//    float aaa = 0.15;

//    if(aaa == 0.1)
//    {
//        qDebug()<<"AAAA : "<<aaa;
//    }
//    if (aaa > 0.1)
//    {
//        qDebug()<<"BBBB : "<<aaa;
//    }

    // problem with water show
    QSurfaceFormat formate2;
//    formate2.setColorSpace(QSurfaceFormat::DefaultColorSpace); // Me Add
    formate2.setSamples(16);
    formate2.setDepthBufferSize(24);
    formate2.setStencilBufferSize(8);
//    formate2.setVersion(3, 2);
    formate2.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(formate2);
//    wid.setFormat(format); // OR


    MainWidget wid;
    wid.show();

//    Widget w;
//    w.setObjectName("collMain");
//    w.setWindowTitle("collMain");

//    w.show();

    return a.exec();
}
