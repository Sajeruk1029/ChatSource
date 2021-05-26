/*
	*(С) Egor Kuznecov (goga.petrovich12@gmail.com)
*/

#include "thread.h"

Thread::Thread(QFile *file, QUdpSocket *socket, QString address, unsigned short port, long max) : File(file), Socket(socket), Address(address), Port(port), bytes(0), bytesmax(max){}

void Thread::run()
{
	QProgressDialog *progress = new QProgressDialog("Отправка данных...", "Прервать", 0, bytesmax);
	progress->setMinimumDuration(0);
	progress->setWindowTitle("Подождите...");

	while (!File->atEnd())
  {
  	if(progress->wasCanceled())
    {
    	break;
    }

    bytes += (File->bytesAvailable() >= 65507)? 65507 : File->bytesAvailable();

    progress->setValue(bytes);

   	Socket->writeDatagram(File->read((File->bytesAvailable() >= 65507)? 65507 : File->bytesAvailable()), Address, Port);

   	qDebug() << "WORKED! " << bytes;
    QThread::msleep(((bytesmax / 65507) * 100) + 500);
 }

 delete progress;

 emit finished();
}

void Thread::start(){ run(); }
