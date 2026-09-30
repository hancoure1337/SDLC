#include "model/sinmodel.h"

#include <QtGlobal>
#include <cmath>

namespace {

// Возраст наступления «сознательности»: до 7 лет грехи не засчитываются.
constexpr int kAgeOfReason = 7;

// Веса грехов в формуле.
constexpr double kWeightLie      = 1.0;
constexpr double kWeightEnvy     = 0.8;
constexpr double kWeightSwearing = 0.5;
constexpr double kWeightIdleHour = 0.3;

// Коэффициенты искупления, соответствуют вариантам покаяния.
const double kRedemption[] = { 1.0, 0.9, 0.75, 0.5 };

// Разбор целого неотрицательного числа с проверкой диапазона.
bool parseInt(const QString &text, int minValue, int maxValue, int &out)
{
    bool ok = false;
    const int value = text.trimmed().toInt(&ok);
    if (!ok || value < minValue || value > maxValue)
        return false;
    out = value;
    return true;
}

// Разбор вещественного числа (запятая допускается как разделитель).
bool parseDouble(const QString &text, double minValue, double maxValue, double &out)
{
    bool ok = false;
    QString normalized = text.trimmed();
    normalized.replace(QLatin1Char(','), QLatin1Char('.'));
    const double value = normalized.toDouble(&ok);
    if (!ok || value < minValue || value > maxValue)
        return false;
    out = value;
    return true;
}

} // namespace

SinModel::SinModel(QObject *parent)
    : QObject(parent)
{
}

QStringList SinModel::penanceOptions()
{
    return { QStringLiteral("Никогда"),
             QStringLiteral("Раз в год"),
             QStringLiteral("Раз в месяц"),
             QStringLiteral("Каждую неделю") };
}

void SinModel::submit(const SinInput &input)
{
    // Введённые данные запоминаются в любом случае, даже если они
    // некорректны: при повторном вводе окно будет ими восстановлено.
    m_input = input;
    emit inputChanged(m_input);

    int day = 0;
    int month = 0;
    int year = 0;
    const int currentYear = QDate::currentDate().year();

    if (!parseInt(input.day, 1, 31, day)) {
        emit validationFailed(QStringLiteral("День рождения должен быть целым числом от 1 до 31."));
        return;
    }
    if (!parseInt(input.month, 1, 12, month)) {
        emit validationFailed(QStringLiteral("Месяц рождения должен быть целым числом от 1 до 12."));
        return;
    }
    if (!parseInt(input.year, 1900, currentYear, year)) {
        emit validationFailed(QStringLiteral("Год рождения должен быть целым числом от 1900 до %1.").arg(currentYear));
        return;
    }

    const QDate birthDate(year, month, day);
    if (!birthDate.isValid()) {
        emit validationFailed(QStringLiteral("Такой даты не существует в календаре."));
        return;
    }
    if (birthDate > QDate::currentDate()) {
        emit validationFailed(QStringLiteral("Дата рождения не может быть в будущем."));
        return;
    }

    int lies = 0;
    int envy = 0;
    int swearing = 0;
    double socialHours = 0.0;

    if (!parseInt(input.lies, 0, 1000, lies)) {
        emit validationFailed(QStringLiteral("Количество случаев лжи за неделю — целое число от 0 до 1000."));
        return;
    }
    if (!parseInt(input.envy, 0, 1000, envy)) {
        emit validationFailed(QStringLiteral("Количество случаев зависти за неделю — целое число от 0 до 1000."));
        return;
    }
    if (!parseInt(input.swearing, 0, 1000, swearing)) {
        emit validationFailed(QStringLiteral("Количество бранных слов за неделю — целое число от 0 до 1000."));
        return;
    }
    if (!parseDouble(input.socialHours, 0.0, 24.0, socialHours)) {
        emit validationFailed(QStringLiteral("Время в соцсетях — число от 0 до 24 часов в сутки."));
        return;
    }

    const int penanceCount = static_cast<int>(sizeof(kRedemption) / sizeof(kRedemption[0]));
    if (input.penance < 0 || input.penance >= penanceCount) {
        emit validationFailed(QStringLiteral("Не выбрана частота покаяния."));
        return;
    }

    m_result = calculate(birthDate, lies, envy, swearing, socialHours, input.penance);
    emit resultChanged(m_result);
}

SinResult SinModel::calculate(const QDate &birthDate,
                              int lies, int envy, int swearing,
                              double socialHours, int penance) const
{
    SinResult result;
    result.valid = true;

    // 1. Сколько дней человек грешит осознанно.
    const qint64 days = birthDate.addYears(kAgeOfReason).daysTo(QDate::currentDate());
    result.consciousDays = qMax<qint64>(0, days);

    // 2. Греховная нагрузка за одну неделю.
    result.sinsPerWeek = lies * kWeightLie
                       + envy * kWeightEnvy
                       + swearing * kWeightSwearing
                       + socialHours * 7.0 * kWeightIdleHour;

    // 3. Искупление уменьшает итог.
    result.redemption = kRedemption[penance];

    // 4. Итоговое количество грехов.
    const double total = result.sinsPerWeek / 7.0 * static_cast<double>(result.consciousDays) * result.redemption;
    result.totalSins = static_cast<qint64>(std::llround(total));

    // 5. Словесная оценка.
    if (result.totalSins == 0)
        result.verdict = QStringLiteral("Безгрешен");
    else if (result.totalSins < 1000)
        result.verdict = QStringLiteral("Праведник");
    else if (result.totalSins < 10000)
        result.verdict = QStringLiteral("Обычный грешник");
    else if (result.totalSins < 50000)
        result.verdict = QStringLiteral("Заядлый грешник");
    else
        result.verdict = QStringLiteral("Пора срочно на исповедь");

    return result;
}
