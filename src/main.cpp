#include <QApplication>
#include <QPushButton>

#include "mainwindow.h"
#include "common/Error.h"

int main(int argc, char * argv[])
{
	QApplication a(argc, argv);
	
	QFont font;
	font.setPointSize(11);
	font.setStyleHint(QFont::Monospace);
	a.setFont(font);
	
	QLocale::setDefault(QLocale(QLocale::Russian, QLocale::RussianFederation));
	
	MainWindow mw;
	mw.show();
    
    int res;
    try { res = QApplication::exec(); }
    catch(std::exception & err) { Error(err.what()).Show(); }
	
	return res;
}
