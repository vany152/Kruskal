#ifndef ASD_WELCOMETOTESTING_H
#define ASD_WELCOMETOTESTING_H

#include <QWidget>
#include <QCloseEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class WelcomeToTesting; }
QT_END_NAMESPACE

class WelcomeToTesting : public QWidget
{
Q_OBJECT

public:
	explicit WelcomeToTesting(QWidget * parent = nullptr);

	~WelcomeToTesting() override;
	
private:
	Ui::WelcomeToTesting * ui;
	
	void closeEvent(QCloseEvent * e) override;

signals:
	/// сигнал закрытия окна
	void closed(QString username);
	/// нажата кнопка начала тестирования
	void testingRulesButtonClicked();
	
private slots:
	void on_startTestingButton_clicked();
	/**
	 * @brief изменение имени пользователя
	 * @details кнопка начала тестирования неактивна, пока имя пустое
	 */
	void on_usernameLineEdit_textChanged();
	void on_showTestingRulesButton_clicked();
};

#endif //ASD_WELCOMETOTESTING_H
