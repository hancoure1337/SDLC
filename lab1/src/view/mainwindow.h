#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

#include "model/sinmodel.h"

class QLabel;

// Главное окно. В активной модели View подписывается на сигналы модели
// напрямую и обновляет себя сам, без участия контроллера.
class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(SinModel *model, QWidget *parent = nullptr);

signals:
    // Намерение пользователя. Обрабатывает контроллер.
    void calculateRequested();

private slots:
    void showInput(const SinInput &input);
    void showResult(const SinResult &result);
    void showError(const QString &message);

private:
    SinModel *m_model;

    QLabel *m_birthDate;
    QLabel *m_lies;
    QLabel *m_envy;
    QLabel *m_swearing;
    QLabel *m_socialHours;
    QLabel *m_penance;

    QLabel *m_consciousDays;
    QLabel *m_sinsPerWeek;
    QLabel *m_redemption;
    QLabel *m_totalSins;
    QLabel *m_verdict;
};

#endif // MAINWINDOW_H
