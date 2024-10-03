#ifndef TESTWEDGET_H
#define TESTWEDGET_H

#include <QWidget>
#include <QObject>

class TestWedget : public QObject
{
    Q_OBJECT
public:
    explicit TestWedget(QObject *parent = nullptr);
    ~TestWedget();

signals:

};

#endif // TESTWEDGET_H
