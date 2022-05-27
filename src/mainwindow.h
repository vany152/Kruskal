#ifndef ASD_MAINWINDOW_H
#define ASD_MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
Q_OBJECT

public:
	explicit MainWindow(QWidget * parent = nullptr);
	~MainWindow() override;

private:
	Ui::MainWindow * ui;
	
	/// вывод приветствия в текстовое поле
	void displayHelloText();
	
private slots:
	/// запуск окна с теорией
	void on_theoryButton_clicked();

	/// запуск окна с демонстрацией
	void on_demoButton_clicked();
	
	/// запуск окна с тестированием
	void on_testingButton_clicked();
};


#endif //ASD_MAINWINDOW_H
