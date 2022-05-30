#ifndef KRUSKAL_TESTINGSETTINGSWINDOW_H
#define KRUSKAL_TESTINGSETTINGSWINDOW_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class TestingSettingsWindow; }
QT_END_NAMESPACE

class TestingSettingsWindow : public QWidget
{
Q_OBJECT

public:
	/// конструктор класса
	explicit TestingSettingsWindow(QWidget * parent = nullptr);
	/// деструктор класса
	~TestingSettingsWindow() override;

private:
	Ui::TestingSettingsWindow * ui;
	
private slots:
	/**
	 * @brief выбор нового пути к главному файлу теории
	 * @details используется QFileDialog
	 */
	void on_changeQuestionsSourceButton_clicked();
	
	/// сохранение конфигурации в файл
	void on_saveButton_clicked();
};


#endif // KRUSKAL_TESTINGSETTINGSWINDOW_H
