#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QMainWindow>

namespace Ui { class SettingsWindow; }

class SettingsWindow : public QMainWindow
{
    Q_OBJECT

public:
    ~SettingsWindow();
    
    /// возвращение состояния объекта
    static SettingsWindow & Instance();

private:
    Ui::SettingsWindow *ui;
    
    // приватный конструктор для синглтона
    explicit SettingsWindow(QWidget *parent = nullptr);
    
    /// обновление полей значениями из файла конфигурации
    void update();

private slots:
    /**
     * @brief выбор нового пути к главному файлу теории
     * @details используется QFileDialog
     */
    void on_changeTheorySourceButton_clicked();
    
    /// сохранение конфигурации теории в файл
    void on_saveTheorySettingsButton_clicked();
    
    /**
     * @brief выбор нового пути к главному файлу теории
     * @details используется QFileDialog
     */
    void on_changeQuestionsSourceButton_clicked();
    
    /// сохранение конфигурации тестирования в файл
    void on_saveTestingsWettingsButton_clicked();
};

#endif // SETTINGSWINDOW_H
