#ifndef KRUSKAL_HTMLVIEWER_H
#define KRUSKAL_HTMLVIEWER_H

#include <QMainWindow>

namespace Ui { class HtmlViewer; }

class HtmlViewer : public QMainWindow
{
Q_OBJECT

public:
	/// конструктор класса
	explicit HtmlViewer(QWidget * parent = nullptr);
	/// деструктор класса
	~HtmlViewer();

    /**
     * @brief открытие файла-источника
     * @param source путь к источнику
     */
    void Open(const QString & source);

    /**
     * @brief установка заголовка окна
     * @param titleText текст заголовка
     */
    void SetWindowTitle(const QString & title) { setWindowTitle(title); }

private:
	Ui::HtmlViewer * ui;
 
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

#endif // KRUSKAL_HTMLVIEWER_H
