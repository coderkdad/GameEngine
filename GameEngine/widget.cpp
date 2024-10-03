#include "widget.h"
#include "ui_widget.h"
#include "mainwidget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    // Learn 3 Rusian : 2:00 minte تحبب الصورة كالتشوبش
//    QSurfaceFormat formate;
//    formate.setSamples(16);
//    formate.setDepthBufferSize(24);
//    QSurfaceFormat::setDefaultFormat(formate);

    wid = new MainWidget;
    wid->show();
//    wid->showFullScreen();

    ui->sliderCamTran_Z->setValue(-10); //postion camera
}

Widget::~Widget()
{
    qDebug("Finish Destroy Main");
    delete ui;
    delete wid;
    qDebug("WWW");
    wid=nullptr;
}

void Widget::on_sliderCamRot_X_valueChanged(int value)
{
//    wid->setRotationsCamera(ui->sliderCamRot_X->value(), ui->sliderCamRot_Y->value(), ui->sliderCamRot_Z->value());
}
void Widget::on_sliderCamRot_Y_valueChanged(int value)
{
//    wid->setRotationsCamera(ui->sliderCamRot_X->value(), ui->sliderCamRot_Y->value(), ui->sliderCamRot_Z->value());
}
void Widget::on_sliderCamRot_Z_valueChanged(int value)
{
//    wid->setRotationsCamera(ui->sliderCamRot_X->value(), ui->sliderCamRot_Y->value(), ui->sliderCamRot_Z->value());
}
void Widget::on_sliderCamTran_X_valueChanged(int value)
{
//    wid->setTranstionCamera(ui->sliderCamTran_X->value(), ui->sliderCamTran_Y->value(), ui->sliderCamTran_Z->value());
}
void Widget::on_sliderCamTran_Y_valueChanged(int value)
{
//    wid->setTranstionCamera(ui->sliderCamTran_X->value(), ui->sliderCamTran_Y->value(), ui->sliderCamTran_Z->value());
}
void Widget::on_sliderCamTran_Z_valueChanged(int value)
{
//    wid->setTranstionCamera(ui->sliderCamTran_X->value(), ui->sliderCamTran_Y->value(), ui->sliderCamTran_Z->value());
}

void Widget::on_checkRotCamY_toggled(bool checked)
{
//    wid->setAutoRotCamY(checked);
}

void Widget::on_butQuit_clicked()
{
    qApp->quit();
}

