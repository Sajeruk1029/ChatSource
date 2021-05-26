/*
	*(С) Egor Kuznecov (goga.petrovich12@gmail.com)
*/

#ifndef THREAD_H
#define THREAD_H

	#include <QFile>

	#include <QObject>

	#include <QUdpSocket>

	#include <QString>

	#include <QHostAddress>

	#include <QThread>

	#include <QProgressDialog>

	class Thread : public QObject
	{
  	Q_OBJECT
			public:
    		Thread(QFile*, QUdpSocket*, QString, unsigned short, long);

			private:
    		QFile *File;

    		QUdpSocket *Socket;

    		QHostAddress Address;

    		unsigned short Port;

    		long bytes;
    		long bytesmax;

    		void run();

			signals:
    		void finished();

				private slots:
    			void start();
	};

#endif
