#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class Widget;
}

class MainWidget;
class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    /// =============== Items 1 ================== ///

    /// =============== Items 2 ================== ///

    /// =============== All Items ================== ///

    /// =============== Camera =================== ///
    void on_sliderCamRot_X_valueChanged(int value);
    void on_sliderCamRot_Y_valueChanged(int value);
    void on_sliderCamRot_Z_valueChanged(int value);
    void on_sliderCamTran_X_valueChanged(int value);
    void on_sliderCamTran_Y_valueChanged(int value);
    void on_sliderCamTran_Z_valueChanged(int value);
    void on_butQuit_clicked();



    void on_checkRotCamY_toggled(bool checked);

private:
    Ui::Widget *ui;
    MainWidget *wid;
};

#endif // WIDGET_H
///========== Mind Fekra =============///
// draw quarter or circle >> 
// any Elce Plane move in quarter clicked ShootGun >> 
// Rocket move To that Plane
// انشاء مربع لتصويب على الاهداف
// واي كائن يكون داخله يكون تلاحقه الصاروخ التي أطلقتها

///==================

// Elce Plane Start End Erth To Begin Erth >>
// Delete Plane in Begin Erth 
// تنطلق الطائرات الأخرى عكس الطائره الخاصه بي
// من نهاية الارض الى بدايتها
// وتنهي حياتها عند ذلك الطرف أو عندما تتخطى طائرتي

///===============
// أهداف وموارد على الأرض 
// 
