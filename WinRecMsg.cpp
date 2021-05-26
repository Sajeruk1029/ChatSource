/*
	*(С) 2021 Egor Kuznecov (goga.petrovich12@gmail.com)
*/
#include "WinRecMsg.h"

WinRecMsg::WinRecMsg(QWidget *wl) : layout(new QBoxLayout(QBoxLayout::Down)), labelhost(new QLabel("Ваш адрес: ")), labelport(new QLabel("Ваш порт: ")),
linemessages(new QTextEdit()), butexit(new QPushButton("Выход")), winLogin(wl),
server(new QUdpSocket(this)), socket(new QUdpSocket(this)),
list(new QList<QString>()), recfile(false)
{
	setWindowTitle("Сервер");
	setFixedSize(500, 500);
	setLayout(layout);

	linemessages->setPlaceholderText("Сообщения");
	linemessages->setReadOnly(true);

	if(!socket->bind(QHostAddress::Any, 5555))
	{
		QMessageBox::critical(this, "Ошибка", "Порт уже используется!");

		if(server){ delete server; }

		server = nullptr;

		if(socket){ delete socket; }

		socket = nullptr;

		winLogin->show();

		close();
	}

	labelhost->setText(labelhost->text() + server->localAddress().toString());
	labelport->setText(labelport->text() + QString::number(5555));

	layout->addWidget(linemessages);
	layout->addWidget(butexit);

	QObject::connect(butexit, SIGNAL(clicked()), this, SLOT(clickButExit()));
	QObject::connect(socket, SIGNAL(readyRead()), this, SLOT(ready()));
}

WinRecMsg::~WinRecMsg()
{
	if(server)
	{
		delete server;
		server = nullptr;
	}

	}

	if(socket)
	{
		delete socket;

		socket = nullptr;
	}

	delete labelhost;
	delete labelport;

	delete linemessages;

	delete butexit;

	delete list;

	delete layout;
}

void WinRecMsg::clickButExit()
{
	if(server)
	{
		server->close();

		delete server;

		server = nullptr;
	}

	if(socket)
	{
		socket->close();

		delete socket;

		socket = nullptr;
	}

	winLogin->show();

	close();
}

void WinRecMsg::ready()
{
	QNetworkDatagram datagram = socket->receiveDatagram();
	QString msg = "";

	msg =  QString(datagram.senderAddress().toString().split(':')[3]) + ":" + QString::number(datagram.senderPort()) + QString(datagram.data() + "\n");

	if(msg.indexOf("%ENDFILE%") > -1)
	{
		recfile = false;
	}

	if(msg.indexOf("%ENDCHANNEL%") > -1)
  {
  	list->removeAt(list->indexOf(datagram.senderAddress().toString().split(':')[3]));
  }

	linemessages->setText(linemessages->toPlainText() + msg);

	if(list->indexOf(datagram.senderAddress().toString().split(':')[3]) == -1)
	{
		list->append(datagram.senderAddress().toString().split(':')[3]);
	}

	//qDebug() << datagram.senderAddress().toString().split(':')[3];

	for(int counter = 0; counter < list->count(); ++counter)
	{
  	if(!recfile)
    {
    	server->writeDatagram(msg.toUtf8(), QHostAddress(list->value(counter)), 5554);
    }
    else
    {
    	server->writeDatagram(datagram.data(), QHostAddress(list->value(counter)), 5554);
    }
	}

  if(msg.indexOf("%FILE%:") > -1)
  {
  	recfile = true;
  }
}
