/*
	*(С) 2021 Egor Kuznecov (goga.petrovich12@gmail.com)
*/

#include "WinServerLogin.h"

WinServerLogin::WinServerLogin() : layout(new QBoxLayout(QBoxLayout::Down)), linehostserver(new QLineEdit()), lineloginserver(new QLineEdit()),
linepasswordserver(new QLineEdit()), butconnect(new QPushButton("Подключиться к серверу")), butdisconnect(new QPushButton("Отключиться от сервера")),
butcontinue(new QPushButton("Продолжить")), butloginserver(new QPushButton("Войти как сервер")), db(nullptr),
query(nullptr), winLogin(nullptr), winRecMsg(nullptr)
{
	setWindowTitle("Авторизация на сервере");
	setLayout(layout);
	setFixedSize(300, 200);

	linehostserver->setPlaceholderText("Хост");
	lineloginserver->setPlaceholderText("Логин");
	linepasswordserver->setPlaceholderText("Пароль");

	linehostserver->setMaxLength(20);
	lineloginserver->setMaxLength(20);
	linepasswordserver->setMaxLength(20);

	linepasswordserver->setEchoMode(QLineEdit::EchoMode::Password);

	butdisconnect->setEnabled(false);
	butcontinue->setEnabled(false);

	layout->addWidget(linehostserver);
	layout->addWidget(lineloginserver);
	layout->addWidget(linepasswordserver);

	layout->addWidget(butconnect);
	layout->addWidget(butloginserver);
	layout->addWidget(butdisconnect);
	layout->addWidget(butcontinue);

	QObject::connect(butconnect, SIGNAL(clicked()), this, SLOT(clickButConnect()));
	QObject::connect(butdisconnect, SIGNAL(clicked()), this, SLOT(clickButDisconnect()));
	QObject::connect(butcontinue, SIGNAL(clicked()), this, SLOT(clickButContinue()));
	QObject::connect(butloginserver, SIGNAL(clicked()), this, SLOT(clickButServerLogin()));
}

WinServerLogin::~WinServerLogin()
{
	if(query){ delete query; }
	if(db){ delete db; }
	if(winLogin){ delete winLogin; }

	delete linehostserver;
	delete lineloginserver;
	delete linepasswordserver;

	delete butconnect;
	delete butdisconnect;
	delete butcontinue;

	delete layout;
}

void WinServerLogin::clickButConnect()
{
	if(linehostserver->text() == "")
	{
		QMessageBox::warning(this, "Ошибка", "Поле хоста не должно быть пустым!");

		return;
	}

	if(lineloginserver->text() == "")
	{
		QMessageBox::warning(this, "Ошибка", "Поле логина не должно быть пустым!");

		return;
	}

	if(linepasswordserver->text() == "")
	{
		QMessageBox::warning(this, "Ошибка", "Поле пароля не должно быть пустым!");

		return;
	}

	db = new QSqlDatabase();

	*db = QSqlDatabase::addDatabase("QMYSQL", "conn");

	db->setHostName(linehostserver->text());
	db->setUserName(lineloginserver->text());
	db->setPassword(linepasswordserver->text());
	db->setDatabaseName("chat");

	if(db->open())
	{
		query = new QSqlQuery(*db);

		QMessageBox::warning(this, "Сообщение", "Успех");

		butconnect->setEnabled(false);
		butdisconnect->setEnabled(true);
		butcontinue->setEnabled(true);

		linehostserver->setEnabled(false);
		lineloginserver->setEnabled(false);
		linepasswordserver->setEnabled(false);
	}
	else
	{
		QMessageBox::warning(this, "Ошибка", "неверные данные для входа");

		QSqlDatabase::removeDatabase("conn");

		delete query;
		query = nullptr;

		delete db;
		db = nullptr;
	}
}

void WinServerLogin::clickButDisconnect()
{
	delete query;
	query = nullptr;

	db->close();

	QSqlDatabase::removeDatabase("conn");

	delete db;
	db = nullptr;

	butconnect->setEnabled(true);
	butdisconnect->setEnabled(false);
	butcontinue->setEnabled(false);

	linehostserver->setEnabled(true);
	lineloginserver->setEnabled(true);
	linepasswordserver->setEnabled(true);
}

void WinServerLogin::clickButContinue()
{
	winLogin = new WinLogin(db, query, this);

	winLogin->show();

	hide();
}

void WinServerLogin::clickButServerLogin()
{
	winRecMsg = new WinRecMsg(this);

	winRecMsg->show();

	close();
}
