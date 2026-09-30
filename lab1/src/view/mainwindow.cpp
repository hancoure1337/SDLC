#include "view/mainwindow.h"

#include <QFont>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLocale>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

namespace {

const QString kEmpty = QStringLiteral("—");

QLabel *makeValueLabel(QWidget *parent)
{
    auto *label = new QLabel(kEmpty, parent);
    label->setTextInteractionFlags(Qt::TextSelectableByMouse);
    return label;
}

} // namespace

MainWindow::MainWindow(SinModel *model, QWidget *parent)
    : QWidget(parent)
    , m_model(model)
    , m_birthDate(makeValueLabel(this))
    , m_lies(makeValueLabel(this))
    , m_envy(makeValueLabel(this))
    , m_swearing(makeValueLabel(this))
    , m_socialHours(makeValueLabel(this))
    , m_penance(makeValueLabel(this))
    , m_consciousDays(makeValueLabel(this))
    , m_sinsPerWeek(makeValueLabel(this))
    , m_redemption(makeValueLabel(this))
    , m_totalSins(makeValueLabel(this))
    , m_verdict(makeValueLabel(this))
{
    setWindowTitle(QStringLiteral("Калькулятор грехов"));

    auto *inputBox = new QGroupBox(QStringLiteral("Последние введённые данные"), this);
    auto *inputForm = new QFormLayout(inputBox);
    inputForm->addRow(QStringLiteral("Дата рождения:"), m_birthDate);
    inputForm->addRow(QStringLiteral("Ложь (раз в неделю):"), m_lies);
    inputForm->addRow(QStringLiteral("Зависть (раз в неделю):"), m_envy);
    inputForm->addRow(QStringLiteral("Сквернословие (раз в неделю):"), m_swearing);
    inputForm->addRow(QStringLiteral("Соцсети (часов в день):"), m_socialHours);
    inputForm->addRow(QStringLiteral("Покаяние:"), m_penance);

    auto *resultBox = new QGroupBox(QStringLiteral("Результат расчёта"), this);
    auto *resultForm = new QFormLayout(resultBox);
    resultForm->addRow(QStringLiteral("Дней сознательной жизни:"), m_consciousDays);
    resultForm->addRow(QStringLiteral("Грехов в неделю:"), m_sinsPerWeek);
    resultForm->addRow(QStringLiteral("Коэффициент искупления:"), m_redemption);
    resultForm->addRow(QStringLiteral("Всего грехов:"), m_totalSins);
    resultForm->addRow(QStringLiteral("Вердикт:"), m_verdict);

    QFont totalFont = m_totalSins->font();
    totalFont.setBold(true);
    totalFont.setPointSize(totalFont.pointSize() + 2);
    m_totalSins->setFont(totalFont);

    auto *calculateButton = new QPushButton(QStringLiteral("Рассчитать"), this);
    connect(calculateButton, &QPushButton::clicked, this, &MainWindow::calculateRequested);

    auto *layout = new QVBoxLayout(this);
    layout->addWidget(calculateButton);
    layout->addWidget(inputBox);
    layout->addWidget(resultBox);
    layout->addStretch();

    // Подписка View на активную модель.
    connect(m_model, &SinModel::inputChanged, this, &MainWindow::showInput);
    connect(m_model, &SinModel::resultChanged, this, &MainWindow::showResult);
    connect(m_model, &SinModel::validationFailed, this, &MainWindow::showError);

    setMinimumWidth(380);
}

void MainWindow::showInput(const SinInput &input)
{
    m_birthDate->setText(QStringLiteral("%1.%2.%3").arg(input.day, input.month, input.year));
    m_lies->setText(input.lies);
    m_envy->setText(input.envy);
    m_swearing->setText(input.swearing);
    m_socialHours->setText(input.socialHours);
    m_penance->setText(SinModel::penanceOptions().value(input.penance, kEmpty));
}

void MainWindow::showResult(const SinResult &result)
{
    const QLocale locale;
    m_consciousDays->setText(locale.toString(result.consciousDays));
    m_sinsPerWeek->setText(locale.toString(result.sinsPerWeek, 'f', 2));
    m_redemption->setText(locale.toString(result.redemption, 'f', 2));
    m_totalSins->setText(locale.toString(result.totalSins));
    m_verdict->setText(result.verdict);
}

void MainWindow::showError(const QString &message)
{
    // Прошлый результат перестал соответствовать введённым данным.
    m_consciousDays->setText(kEmpty);
    m_sinsPerWeek->setText(kEmpty);
    m_redemption->setText(kEmpty);
    m_totalSins->setText(kEmpty);
    m_verdict->setText(kEmpty);

    QMessageBox::warning(this, QStringLiteral("Некорректные данные"), message);
}
