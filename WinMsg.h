/*
	*(С) 2021 Egor Kuznecov (goga.petrovich12@gmail.com)
*/

#ifndef WINMSG_H
#define WINMSG_H

#include <QObject>

#include <QWidget>
#include <QBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QTextEdit>

#include <QMessageBox>

#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

#include <QUdpSocket>
#include <QHostAddress>
#include <QNetworkDatagram>

#include <QDebug>

class WinMsg : public QWidget
{
	Q_OBJECT

	public:
		explicit WinMsg(QWidget*, QString);
		~WinMsg();

	private slots:
		void clickButExit();
		void clickButSend();
		void recive();

	private:
		QBoxLayout *layout;

		QPushButton *butsend;
		QPushButton *butexit;

		QLabel *labellogin;
		QLabel *labelhost;
		QLabel *labelport;

		QLineEdit *linemsg;
		QLineEdit *linehost;

		QTextEdit *linechat;

		QWidget *winLogin;

		QUdpSocket *socket;
		QUdpSocket *server;

		QString *login;
};

#endif
