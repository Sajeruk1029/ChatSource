/*
	*(С) 2021 Egor Kuznecov (goga.petrovich12@gmail.com)
*/

#ifndef WINREG_H
#define WINREG_H

#include <QObject>

#include <QWidget>
#include <QBoxLayout>
#include <QLineEdit>
#include <QPushButton>

#include <QMessageBox>

#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

#include <QCryptographicHash>

class WinReg : public QWidget
{
	Q_OBJECT

	public:
		WinReg(QSqlDatabase*, QSqlQuery*, QWidget*);
		~WinReg();

	private slots:
		void clickButReg();
		void clickButBack();

	private:
		QBoxLayout *layout;

		QPushButton *butreg;
		QPushButton *butback;

		QLineEdit *linelogin;
		QLineEdit *linepassword;

		QSqlDatabase *db;

		QSqlQuery *query;

		QWidget *winLogin;
};

#endif
