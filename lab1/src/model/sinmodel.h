#ifndef SINMODEL_H
#define SINMODEL_H

#include <QDate>
#include <QObject>
#include <QString>
#include <QStringList>

// Сырые данные, введённые пользователем в окне вопросов.
// Хранятся строками: проверку корректности выполняет модель, а не View.
struct SinInput
{
    QString day;
    QString month;
    QString year;
    QString lies;         // сколько раз в неделю лжёте
    QString envy;         // сколько раз в неделю завидуете
    QString swearing;     // сколько раз в неделю сквернословите
    QString socialHours;  // часов в день в соцсетях (праздность)
    int     penance = 0;  // индекс варианта покаяния
};

// Результат расчёта по формуле.
struct SinResult
{
    bool    valid          = false;
    qint64  consciousDays  = 0;    // дней сознательной жизни (с 7 лет)
    double  sinsPerWeek    = 0.0;  // греховная нагрузка за неделю
    double  redemption     = 1.0;  // коэффициент искупления
    qint64  totalSins      = 0;    // итоговое количество грехов
    QString verdict;               // словесная оценка
};

// АКТИВНАЯ модель: не знает ни о View, ни о контроллере.
// О любом своём изменении сама оповещает подписчиков сигналами.
class SinModel : public QObject
{
    Q_OBJECT

public:
    explicit SinModel(QObject *parent = nullptr);

    const SinInput  &input() const  { return m_input; }
    const SinResult &result() const { return m_result; }

    static QStringList penanceOptions();

public slots:
    // Единственная точка входа для контроллера.
    void submit(const SinInput &input);

signals:
    void inputChanged(const SinInput &input);
    void resultChanged(const SinResult &result);
    void validationFailed(const QString &message);

private:
    SinResult calculate(const QDate &birthDate,
                        int lies, int envy, int swearing,
                        double socialHours, int penance) const;

    SinInput  m_input;
    SinResult m_result;
};

#endif // SINMODEL_H
