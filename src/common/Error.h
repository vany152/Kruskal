#ifndef ASD_ERROR_H
#define ASD_ERROR_H

#include <string>
#include <exception>

#include <QErrorMessage>

class Error : public std::exception
{
protected:
	std::string what; ///< текст ошибки
	
public:
	Error() : what("") {}
	Error(const std::string & what) : what(what) {}
	std::string What() { return what; }
	
	void Show()
	{
		QErrorMessage * err = new QErrorMessage(nullptr);
		err->showMessage(QString::fromStdString(what));
		err->exec();
	}
};

class FileError : public Error
{
public:
	FileError() { what = ""; }
	FileError(const std::string & what) { this->what = what; }
};

#endif //ASD_ERROR_H
