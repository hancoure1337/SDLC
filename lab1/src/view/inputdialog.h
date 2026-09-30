#ifndef INPUTDIALOG_H
#define INPUTDIALOG_H

#include <QDialog>

#include "model/sinmodel.h"

class QComboBox;
class QLineEdit;

// Отдельное окно с вопросами утилиты.
// Ничего не вычисляет и не проверяет — только собирает введённый текст.
class InputDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InputDialog(QWidget *parent = nullptr);

    void setInput(const SinInput &input); // восстановление прошлого ввода
    SinInput input() const;               // выдача введённого контроллеру

private:
    QLineEdit *m_day;
    QLineEdit *m_month;
    QLineEdit *m_year;
    QLineEdit *m_lies;
    QLineEdit *m_envy;
    QLineEdit *m_swearing;
    QLineEdit *m_socialHours;
    QComboBox *m_penance;
};

#endif // INPUTDIALOG_H
