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

	#include <QFileDialog>

	#include <QtSql/QSql>
	#include <QtSql/QSqlDatabase>
	#include <QtSql/QSqlQuery>

	#include <QUdpSocket>
	#include <QHostAddress>
	#include <QNetworkDatagram>

	#include <QDebug>

	#include <QFile>

	#include <QByteArray>

	#include <QUrl>

	#include <QTimer>

	#include <QThread>

	#include <QDir>

	#include "thread.h"

	class WinMsg : public QWidget
	{
		Q_OBJECT

			public:
				explicit WinMsg(QWidget*, QString);
				~WinMsg();

			private slots:
				void clickButExit();
				void clickButSend();
				void clickButSendFile();
				void recive();
    //void changePathSlot(unsigned char);
    		void endReceiveFileSlot();

			private:
				QBoxLayout *layout;

				QPushButton *butsend;
				QPushButton *butsendfile;
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
				QString *path;
   			QString filenameclass;

				QFile *file;

				bool receivefile;

    		unsigned char mode;

    		QTimer *timer;

    		unsigned char choosedir;

    		QByteArray arr;

    		unsigned long sizefile;
    		unsigned long receivebytes;

    		QThread *thread;

    		Thread *thread2;

    	signals:
        //void changePathSignal(unsigned char);
				void endReceiveFileSignal();
				void startReceiveFileSignal();
	};

#endif
