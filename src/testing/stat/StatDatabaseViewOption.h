#ifndef KRUSKAL_STATDATABASEVIEWOPTION_H
#define KRUSKAL_STATDATABASEVIEWOPTION_H

#include <string>

#include <QWidget>

class StatDatabaseViewOption
{
public:
    /**
     * @brief конструктор класса
     * @param optionName название опции
     * @param fields поля, отображающиеся при выборе текущей опции
     */
    explicit StatDatabaseViewOption(const QString & optionName, std::vector<QWidget *> & fields) :
            optionName(optionName), fields(fields) {}
    virtual ~StatDatabaseViewOption() = 0;
    
     /// установка видимости полей, соответствующих опции фильтра
    void SetFieldsVisibility(bool visible) { for (auto field : fields) { field->setVisible(visible); } }
    QString GetName() const noexcept { return optionName; }
    
private:
    const std::vector<QWidget *> fields; ///< поля, отображающиеся при выборе текущей опции
    const QString optionName; ///< название опции
};

class UsernameOption final : public StatDatabaseViewOption
{
public:
    /**
     * @brief конструктор класса
     * @param fields поля, отображающиеся при выборе текущей опции
     */
    UsernameOption(std::vector<QWidget *> & fields) :
            StatDatabaseViewOption("Имя пользователя", fields) {}
    ~UsernameOption() final = default;
};

class TestingPeriodOption final : public StatDatabaseViewOption
{
public:
    /**
     * @brief конструктор класса
     * @param fields поля, отображающиеся при выборе текущей опции
     */
    explicit TestingPeriodOption(std::vector<QWidget *> & fields) :
            StatDatabaseViewOption("Период прохождения тестирования", fields) {}
    ~TestingPeriodOption() final = default;
};

class TestingDurationOption final : public StatDatabaseViewOption
{
public:
    /**
     * @brief конструктор класса
     * @param fields поля, отображающиеся при выборе текущей опции
     */
    explicit TestingDurationOption(std::vector<QWidget *> & fields) :
            StatDatabaseViewOption("Продолжительность тестирования", fields) {}
    ~TestingDurationOption() final = default;
};

class CompletionPercentage final : public StatDatabaseViewOption
{
public:
    /**
     * @brief конструктор класса
     * @param fields поля, отображающиеся при выборе текущей опции
     */
    explicit CompletionPercentage(std::vector<QWidget *> & fields) :
            StatDatabaseViewOption("Процент выполнения", fields) {}
    ~CompletionPercentage() final = default;
};

#endif //KRUSKAL_STATDATABASEVIEWOPTION_H
