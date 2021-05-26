/*
	*(С) 2021 Egor Kuznecov (goga.petrovich12@gmail.com)
*/

#include "WinMsg.h"

WinMsg::WinMsg(QWidget *wl, QString name) : layout(new QBoxLayout(QBoxLayout::Down)), butsend(new QPushButton("Отправить")), butsendfile(new QPushButton("Файл")),
butexit(new QPushButton("Выход")), labellogin(new QLabel("Ваш ник: " + name)), labelhost(new QLabel("Ваш адрес: ")),
labelport(new QLabel("Ваш порт: ")), linemsg(new QLineEdit()), linehost(new QLineEdit()),
linechat(new QTextEdit()), winLogin(wl), socket(new QUdpSocket(this)),
server(new QUdpSocket(this)), login(new QString(name)), path(new QString("")),
filenameclass(""), file(nullptr), receivefile(false),
mode(0), timer(nullptr), choosedir(0), sizefile(0), receivebytes(0), thread(nullptr), thread2(nullptr)
{
	setWindowTitle("Чат");
	setLayout(layout);
	setFixedSize(500, 500);

	if(!socket->bind(QHostAddress::Any, 5554))
	{
		QMessageBox::critical(this, "Ошибка", "Порт уже используется!");

		if(socket)
		{
			socket->close();

			delete socket;

			socket = nullptr;
		}

		if(server)
		{
			server->close();

			delete server;

			server = nullptr;
		}

		winLogin->show();

		close();
	}

	labelhost->setText(labelhost->text() + socket->localAddress().toString());
	labelport->setText(labelport->text() + QString::number(5554));

	linechat->setReadOnly(true);
	linechat->setPlaceholderText("Сообщения");

	linehost->setPlaceholderText("Адрес");
	linemsg->setPlaceholderText("Текст сообщения...");

	linehost->setMaxLength(20);
	linemsg->setMaxLength(500);

	layout->addWidget(labellogin);
	layout->addWidget(linechat);
	layout->addWidget(linemsg);
	layout->addWidget(linehost);
	layout->addWidget(butsend);
	layout->addWidget(butsendfile);
	layout->addWidget(butexit);

	QObject::connect(butsend, SIGNAL(clicked()), this, SLOT(clickButSend()));
	QObject::connect(butsendfile, SIGNAL(clicked()), this, SLOT(clickButSendFile()));
	QObject::connect(butexit, SIGNAL(clicked()), this, SLOT(clickButExit()));
	QObject::connect(socket, SIGNAL(readyRead()), this, SLOT(recive()));

  //QObject::connect(this, SIGNAL(changePathSignal(unsigned char)), this, SLOT(changePathSlot(unsigned char)));
  QObject::connect(this, SIGNAL(endReceiveFileSignal()), this, SLOT(endReceiveFileSlot()));
}

WinMsg::~WinMsg()
{

	if(socket)
	{
		socket->close();

		delete socket;

		socket = nullptr;
	}

	if(server)
	{
		server->close();

		delete server;

		server = nullptr;
	}

	delete login;

	delete butsend;
	delete butexit;

	delete labellogin;
	delete labelhost;

	delete linemsg;

	delete linehost;

	delete linechat;

	delete layout;
}

void WinMsg::clickButSend()
{
	if(linemsg->text() == "")
	{
		QMessageBox::warning(this, "Ошибка", "Поле с текстом сообщения не должно быть пустым!");

		return;
	}

	if(linehost->text() == "")
	{
		QMessageBox::warning(this, "Ошибка", "Поле с хостом не должно быть пустым!");

		return;
	}

	server->writeDatagram(("[" + *login + "] : " + linemsg->text()).toUtf8(), QHostAddress(linehost->text()), 5555);
}

void WinMsg::clickButExit()
{
  server->writeDatagram(QString("%ENDCHANNEL%").toUtf8(), QHostAddress(linehost->text()), 5555);

	if(socket)
	{
		socket->close();

		delete socket;

		socket = nullptr;
	}

	if(server)
	{
		server->close();

		delete server;

		server = nullptr;
	}

	winLogin->show();

	close();
}

void WinMsg::recive()
{
  QNetworkDatagram datagram = socket->receiveDatagram();

	QString msg = "";
	QString filename = "";
	QString pathtofile = "";

	msg = QString(datagram.data());

	if(msg.indexOf("%FILE%") > -1)
	{
		//pathtofile = QFileDialog::getExistingDirectoryUrl(this, "Выбор папки...", QUrl("/")).toLocalFile();
		if(!QDir::current().exists("Downloads")){ QDir::current().mkdir("Downloads"); }

    receivebytes = 0;

   	//if(pathtofile == "")
   	//{
            //emit changePathSignal(1);
            //return;
    //}
    //else
   	//{
            //emit changePathSignal(0);
    //}

		receivefile = true;

		filename = msg.split(":")[2].replace("\n", "");

		pathtofile = QDir::currentPath() + "/Downloads/" + filename;

    file = new QFile(pathtofile);

    file->open(QIODevice::Append);

    return;
	}

    if(msg.indexOf("%ENDFILE%") > -1)
    {
        receivefile = false;

        return;
    }

    if(receivefile)
    {
      ++receivebytes;
      qDebug() << "Receive: " << receivebytes << " sizefile: " << sizefile;
			file->write(datagram.data());

			return;
    }
    else{ linechat->setText(linechat->toPlainText() + msg); }
}

void WinMsg::clickButSendFile()
{
	QString filename = "";
	QString pathtofile = "";

	if(linehost->text() == "")
	{
		QMessageBox::warning(this, "Ошибка", "Поле с хостом не должно быть пустым!");

		return;
	}

  QByteArray bytearr = {};

  pathtofile = QFileDialog::getOpenFileUrl(this, "Выбор файла...", QUrl("/"), "*").toLocalFile();

	if(pathtofile == ""){ return; }

	filename = pathtofile.mid(pathtofile.lastIndexOf("/") + 1);

  file = new QFile(pathtofile);

  if(!file->open(QIODevice::ReadOnly))
  {
      delete file;
      return;
  }

  sizefile = file->readAll().count();

  qDebug() << sizefile;

  file->seek(0);

  filenameclass = filename;

  arr = bytearr;

  server->writeDatagram(QString("%FILE%:" + filename).toUtf8(), QHostAddress(linehost->text()), 5555);

  thread = new QThread(this);

  thread2 = new Thread(file, socket, linehost->text(), 5555, sizefile);

  thread2->moveToThread(thread);

  QObject::connect(thread, SIGNAL(started()), thread2, SLOT(start()));

  QObject::connect(thread2, SIGNAL(finished()), thread, SLOT(terminate()));
 	QObject::connect(thread2, SIGNAL(finished()), this, SLOT(endReceiveFileSlot()));

  thread->start();
}
/*
void WinMsg::changePathSlot(unsigned char mode)
{
    if(mode == 0)
    {
        thread = new QThread(this);

        thread2 = new Thread(file, socket, linehost->text(), 5555, sizefile);

        thread2->moveToThread(thread);

        QObject::connect(thread, SIGNAL(started()), thread2, SLOT(start()));

        QObject::connect(thread2, SIGNAL(finished()), thread, SLOT(terminate()));
        QObject::connect(thread2, SIGNAL(finished()), this, SLOT(endReceiveFileSlot()));

        thread->start();

    }
    else
    {
        file->close();

        delete file;
    }
}
*/
void WinMsg::endReceiveFileSlot()
{
    if(thread->isFinished())
    {
        delete thread;
    }
    server->writeDatagram(QString("%ENDFILE%").toUtf8(), QHostAddress(linehost->text()), 5555);

    file->flush();

    file->close();

    delete file;

    receivefile = false;
}
