#ifndef KRUSKAL_ERROR_H
#define KRUSKAL_ERROR_H

#include <string>
#include <exception>

#include <QErrorMessage>

class Error : public std::exception
{
protected:
	std::string _what; ///< текст ошибки
	
public:
	Error() = default;
	Error(const std::string & what) : _what(what) {}
	const char * what() const noexcept override { return _what.c_str(); }
	
	void Show()
	{
		QErrorMessage * err = new QErrorMessage(nullptr);
		err->showMessage(QString::fromStdString(_what));
		err->exec();
	}
};

class FileError : public Error
{
public:
	FileError() = default;
	FileError(const std::string & what) : Error(what) {}
};

#endif // KRUSKAL_ERROR_H
