#include "view/inputdialog.h"

#include <QComboBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

InputDialog::InputDialog(QWidget *parent)
    : QDialog(parent)
    , m_day(new QLineEdit(this))
    , m_month(new QLineEdit(this))
    , m_year(new QLineEdit(this))
    , m_lies(new QLineEdit(this))
    , m_envy(new QLineEdit(this))
    , m_swearing(new QLineEdit(this))
    , m_socialHours(new QLineEdit(this))
    , m_penance(new QComboBox(this))
{
    setWindowTitle(QStringLiteral("Вопросы утилиты"));
    setModal(true);

    // Дата рождения вводится тремя отдельными полями:
    // стандартный элемент ввода даты по условию использовать нельзя.
    m_day->setPlaceholderText(QStringLiteral("дд"));
    m_month->setPlaceholderText(QStringLiteral("мм"));
    m_year->setPlaceholderText(QStringLiteral("гггг"));
    m_day->setMaximumWidth(60);
    m_month->setMaximumWidth(60);
    m_year->setMaximumWidth(80);

    auto *dateLayout = new QHBoxLayout;
    dateLayout->addWidget(m_day);
    dateLayout->addWidget(new QLabel(QStringLiteral("."), this));
    dateLayout->addWidget(m_month);
    dateLayout->addWidget(new QLabel(QStringLiteral("."), this));
    dateLayout->addWidget(m_year);
    dateLayout->addStretch();

    m_penance->addItems(SinModel::penanceOptions());

    auto *form = new QFormLayout;
    form->addRow(QStringLiteral("Дата рождения:"), dateLayout);
    form->addRow(QStringLiteral("Сколько раз в неделю вы лжёте:"), m_lies);
    form->addRow(QStringLiteral("Сколько раз в неделю вы завидуете:"), m_envy);
    form->addRow(QStringLiteral("Сколько раз в неделю вы сквернословите:"), m_swearing);
    form->addRow(QStringLiteral("Часов в день в соцсетях:"), m_socialHours);
    form->addRow(QStringLiteral("Как часто вы каетесь:"), m_penance);

    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttons->button(QDialogButtonBox::Ok)->setText(QStringLiteral("Рассчитать"));
    buttons->button(QDialogButtonBox::Cancel)->setText(QStringLiteral("Отмена"));
    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

    auto *layout = new QVBoxLayout(this);
    layout->addLayout(form);
    layout->addWidget(buttons);
}

void InputDialog::setInput(const SinInput &input)
{
    m_day->setText(input.day);
    m_month->setText(input.month);
    m_year->setText(input.year);
    m_lies->setText(input.lies);
    m_envy->setText(input.envy);
    m_swearing->setText(input.swearing);
    m_socialHours->setText(input.socialHours);
    m_penance->setCurrentIndex(input.penance);
}

SinInput InputDialog::input() const
{
    SinInput input;
    input.day         = m_day->text();
    input.month       = m_month->text();
    input.year        = m_year->text();
    input.lies        = m_lies->text();
    input.envy        = m_envy->text();
    input.swearing    = m_swearing->text();
    input.socialHours = m_socialHours->text();
    input.penance     = m_penance->currentIndex();
    return input;
}
