#ifndef NEWTHEORYSYSTEM_H
#define NEWTHEORYSYSTEM_H

#include <QMainWindow>

namespace Ui { class TheorySystem; }

class TheorySystem : public QMainWindow
{
Q_OBJECT

public:
	/// конструктор класса
	explicit TheorySystem(QWidget * parent = nullptr);
	/// деструктор класса
	~TheorySystem();

private:
	Ui::TheorySystem * ui;
	
	/// вывод на экран главного файла с теорией
	void displaySource();

private slots:
	/// переход к предыдущей странице истории
	void on_prevPageButton_clicked();
	/// переход к следующей странице истории
	void on_nextPageButton_clicked();
	/**
	 * @brief изменена URL
	 * @details включение и отключение кнопок перемщения по истории
	 */
    void urlChanged();
	/// открыто окно настроек
    void on_settingsMenuBar_triggered();
};

#endif // NEWTHEORYSYSTEM_H
