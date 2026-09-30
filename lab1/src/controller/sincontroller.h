#ifndef SINCONTROLLER_H
#define SINCONTROLLER_H

#include <QObject>

class QWidget;
class SinModel;

class SinController : public QObject
{
    Q_OBJECT

public:
    SinController(SinModel *model, QWidget *dialogParent, QObject *parent = nullptr);

public slots:
    void onCalculateRequested();

private:
    SinModel *m_model;
    QWidget  *m_dialogParent;
};

#endif // SINCONTROLLER_H
