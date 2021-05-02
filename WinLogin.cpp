/*
	*(С) 2021 Egor Kuznecov (goga.petrovich12@gmail.com)
*/

#include "WinLogin.h"

WinLogin::WinLogin(QSqlDatabase *bd, QSqlQuery *querydb, QWidget *wsl) : layout(new QBoxLayout(QBoxLayout::Down)), linelogin(new QLineEdit()), linepassword(new QLineEdit()),butlogin(new QPushButton("Вход")), butreg(new QPushButton("Регистрация")), butback(new QPushButton("Назад")),
db(bd), query(querydb), winServerLogin(wsl),
winReg(nullptr), winMsg(nullptr)
{
	setWindowTitle("Вход");
	setLayout(layout);
	setFixedSize(300, 150);

	linelogin->setPlaceholderText("Логин");
	linepassword->setPlaceholderText("Пароль");

	linelogin->setMaxLength(20);
	linepassword->setMaxLength(20);

	linepassword->setEchoMode(QLineEdit::EchoMode::Password);

	layout->addWidget(linelogin);
	layout->addWidget(linepassword);
	layout->addWidget(butlogin);
	layout->addWidget(butreg);
	layout->addWidget(butback);

	QObject::connect(butlogin, SIGNAL(clicked()), this, SLOT(clickButLogin()));
	QObject::connect(butreg, SIGNAL(clicked()), this, SLOT(clickButReg()));
	QObject::connect(butback, SIGNAL(clicked()), this, SLOT(clickButBack()));
}

WinLogin::~WinLogin()
{
	if(winReg){ delete winReg; }
	if(winMsg){ delete winMsg; }

	delete linelogin;
	delete linepassword;

	delete butlogin;
	delete butreg;
	delete butback;

	delete layout;
}

void WinLogin::clickButLogin()
{
	if(linelogin->text() == "")
	{
		QMessageBox::warning(this, "Ошибка", "Поле логина не должно быть пустым!");

		return;
	}

	if(linepassword->text() == "")
	{
		QMessageBox::warning(this, "Ошибка", "Поле пароля не должно быть пустым!");

		return;
	}

	query->prepare("select id from accounts where login= :Login and password= :Password");

	query->bindValue(":Login", linelogin->text());
	query->bindValue(":Password", QCryptographicHash::hash(linepassword->text().toUtf8(), QCryptographicHash::Sha256).toHex());

	query->exec();

	if(!query->next())
	{
		QMessageBox::warning(this, "Ошибка", "Неверные данные для входа");
		return;
	}

	winMsg = new WinMsg(this, linelogin->text());

	winMsg->show();

	hide();
}

void WinLogin::clickButReg()
{
	winReg = new WinReg(db, query, this);

	hide();

	winReg->show();
}

void WinLogin::clickButBack()
{
	winServerLogin->show();

	close();
}
