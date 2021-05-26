/*
	*(С) 2021 Egor Kuznecov (goga.petrovich12@gmail.com)
*/

#ifndef WINLOGIN_H
#define WINLOGIN_H

	#include <QObject>

	#include <QBoxLayout>

	#include <QWidget>
	#include <QLineEdit>
	#include <QPushButton>

	#include <QMessageBox>

	#include <QtSql/QSql>
	#include <QtSql/QSqlDatabase>
	#include <QtSql/QSqlQuery>

	#include <QCryptographicHash>

	#include "WinReg.h"
	#include "WinMsg.h"

	class WinLogin : public QWidget
	{
		Q_OBJECT

		public:
			explicit WinLogin(QSqlDatabase*, QSqlQuery*, QWidget*);
			~WinLogin();

		private slots:
			void clickButLogin();
			void clickButReg();
			void clickButBack();

		private:
			QBoxLayout *layout;

			QLineEdit *linelogin;
			QLineEdit *linepassword;

			QPushButton *butlogin;
			QPushButton *butreg;
			QPushButton *butback;

			QSqlDatabase *db;

			QSqlQuery *query;

			QWidget *winServerLogin;

			WinReg *winReg;
			WinMsg *winMsg;
	};

#endif
