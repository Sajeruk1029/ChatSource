/*
	*(С) 2021 Egor Kuznecov (goga.petrovich12@gmail.com)
*/

#ifndef WINSERVERLOGIN_H
#define WINSERVERLOGIN_H

#include <QObject>

#include <QWidget>

#include <QBoxLayout>
#include <QLineEdit>
#include <QPushButton>

#include <QMessageBox>

#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

#include "WinLogin.h"
#include "WinRecMsg.h"

class WinServerLogin : public QWidget
{
	Q_OBJECT

	public:
		WinServerLogin();
		~WinServerLogin();

	private slots:
		void clickButConnect();
		void clickButDisconnect();
		void clickButContinue();
		void clickButServerLogin();

	private:
		QBoxLayout *layout;

		QLineEdit *linehostserver;
		QLineEdit *lineloginserver;
		QLineEdit *linepasswordserver;

		QPushButton *butconnect;
		QPushButton *butdisconnect;
		QPushButton *butcontinue;
		QPushButton *butloginserver;

		QSqlDatabase *db;

		QSqlQuery *query;

		WinLogin *winLogin;
		WinRecMsg *winRecMsg;
};

#endif
