#ifndef ASD_THEORYSETTINGSWINDOW_H
#define ASD_THEORYSETTINGSWINDOW_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class TheorySettingsWindow; }
QT_END_NAMESPACE

class TheorySettingsWindow : public QWidget
{
Q_OBJECT

public:
	/// конструктор класса
	explicit TheorySettingsWindow(QWidget * parent = nullptr);
	/// деструктор класса
	~TheorySettingsWindow() override;

private:
	Ui::TheorySettingsWindow * ui;
	
private slots:
	/**
	 * @brief выбор нового пути к главному файлу теории
	 * @details используется QFileDialog
	 */
	void on_changeTheorySourceButton_clicked();
	
	/// сохранение конфигурации в файл
	void on_saveButton_clicked();
	
};

#endif //ASD_THEORYSETTINGSWINDOW_H
