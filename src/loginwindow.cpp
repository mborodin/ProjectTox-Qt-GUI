#include "loginwindow.hpp"

#include <QApplication>
#include <QDesktopWidget>
#include <QFontDatabase>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QApplication>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QDomDocument>
#include <QFile>
#include <QDir>
#include <QPair>
#include <QVariant>
#include <QWizard>
#include <QWizardPage>

#include <memory>

#include <QDebug>

#include "account.h"

#include "createnewaccountwizardpage.hpp"

LoginWindow::LoginWindow(QWidget *parent) :
    QMainWindow(parent)
{
    const int screenWidth = QApplication::desktop()->width();
    const int screenHeight = QApplication::desktop()->height();
    const int appWidth = 300;
    const int appHeight = 500;

    setGeometry((screenWidth - appWidth) / 2, (screenHeight - appHeight) / 2, appWidth, appHeight);

    setObjectName("LoginWindow");
    setWindowTitle("QTox");
    setWindowIcon(QIcon(":/icons/icon64.png"));
    setContextMenuPolicy(Qt::PreventContextMenu);

    // install Unicode 6.1 supporting font
    QFontDatabase::addApplicationFont("://DejaVuSans.ttf");

    QWidget *loginLayout = new QWidget(this);

    loginLayout->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    //QWidget* loginLayout = this;
    QVBoxLayout *layout = new QVBoxLayout(loginLayout);

    //layout->setMargin(0);
    //layout->setSpacing(0);

    QWidget *userNameLayout = new QWidget(loginLayout);
    QHBoxLayout *line = new QHBoxLayout(userNameLayout);

    QLabel* lbl = new QLabel(userNameLayout);
    lbl->setText(tr("User name:"));

    userName = new QComboBox(userNameLayout);

    connect(userName, SIGNAL(currentIndexChanged(int)), this, SLOT(userSelected(int)));

    //lbl->setBuddy(userName);

    line->addWidget(lbl);
    line->addWidget(userName);

    userNameLayout->setLayout(line);

    QWidget* passwordLayout = new QWidget(loginLayout);
    line = new QHBoxLayout(passwordLayout);

    lbl = new QLabel(passwordLayout);
    lbl->setText(tr("Password:"));

    password = new QLineEdit(passwordLayout);
    password->setEchoMode(QLineEdit::Password);
    lbl->setBuddy(password);

    line->addWidget(lbl);
    line->addWidget(password);

    passwordLayout->setLayout(line);

    QPushButton* loginButton = new QPushButton(loginLayout);
    loginButton->setText(tr("Login"));
    connect(loginButton, SIGNAL(clicked()), this, SLOT(onLoginClicked()));

    QWidget* newAccount = new QWidget(loginLayout);
    line = new QHBoxLayout(newAccount);

    QPushButton* createAccount = new QPushButton(newAccount);
    createAccount->setText(tr("Create account"));
    connect(createAccount, SIGNAL(clicked()), this, SLOT(onCreateClicked()));

    QPushButton* importAccount = new QPushButton(newAccount);
    importAccount->setText(tr("Import"));
    connect(importAccount, SIGNAL(clicked()), this, SLOT(onImportClicked()));

    line->addWidget(createAccount);
    line->addWidget(importAccount);

    newAccount->setLayout(line);

    layout->addWidget(userNameLayout);
    layout->addWidget(passwordLayout);
    layout->addWidget(loginButton);
    layout->addWidget(newAccount);

    layout->setAlignment(Qt::AlignVCenter);

    loginLayout->setLayout(layout);

    setCentralWidget(loginLayout);

    wnd = std::shared_ptr<MainWindow>(new MainWindow());

    loadUsers();
}

void LoginWindow::onCreateClicked() {
    wizard = new QWizard(this);
    wizard->addPage(new CreateNewAccountWizardPage(wizard));

    connect(wizard->button(QWizard::FinishButton), SIGNAL(clicked()), this, SLOT(createUserFinish()));

    wizard->show();
}

void LoginWindow::createUserFinish() {
    QString user = wizard->field("username").toString();
    QString password = wizard->field("password").toString();

    QString toxPath = QDir::home().absoluteFilePath(".config/tox");
    QString tox = user + ".tox";

    std::shared_ptr<Account> account(new Account());

    account->save(toxPath + '/' + tox, password);

    QString s = toxPath + '/' + "accounts.xml";
    QFile fin(s);
    QDomDocument doc;
    doc.setContent(&fin);

    QDomElement accounts = doc.namedItem("accounts").toElement();

    QDomElement accountNode = doc.createElement("account");
    QDomAttr toxAttr = doc.createAttribute("tox");
    QDomAttr encAttr = doc.createAttribute("encrypted");
    QDomAttr nameAttr = doc.createAttribute("name");

    toxAttr.setValue(tox);
    encAttr.setValue(password.length()>0 ? "true" : "false");
    nameAttr.setValue(user);

    accountNode.setAttributeNode(nameAttr);
    accountNode.setAttributeNode(encAttr);
    accountNode.setAttributeNode(toxAttr);

    accounts.appendChild(accountNode);

    QFile fout(s);
    fout.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream stream(&fout);
    stream << doc.toString();
    fout.close();

    userName->clear();
    loadUsers();
    int idx = userName->findText(user);
    userName->setCurrentIndex(idx);
}

void LoginWindow::onImportClicked() {

}

void LoginWindow::onLoginClicked() {
    QString pass = password->text();
    std::shared_ptr<Account> account(new Account);

    account->load(tox, pass);
    wnd->setAccount(account);

    wnd->show();
    this->hide();
}

void LoginWindow::loadUsers() {
    QString s = QDir::home().absoluteFilePath(".config/tox/accounts.xml");
    QFile fin(s);
    QDomDocument doc;
    doc.setContent(&fin);

    QDomElement accounts = doc.namedItem("accounts").toElement();

    QDomElement account = accounts.namedItem("account").toElement();

    while (!account.isNull()) {
        qDebug() << "Name: " << account.attribute("name");
        userName->addItem(account.attribute("name"),
                           QVariant::fromValue(QPair<bool,QString>(
                                   account.attribute("encrypted") == "true",
                               account.attribute("tox")
                               )));
        account = account.nextSiblingElement("account");
    }

    if (userName->count() > 0)
        emit userName->currentIndexChanged(0);
}

void LoginWindow::userSelected(int index) {
    QPair<bool, QString> data = qvariant_cast<QPair<bool,QString>>(userName->itemData(index));
    bool isEnabled = password->isEnabled();
    password->setDisabled(!data.first);
    
    if(isEnabled == password->isEnabled() && !isEnabled) {
      password->setText("");
    }
    
    if(data.second.startsWith('/'))
        tox = data.second;
    else
        tox = QDir::home().absoluteFilePath(".config/tox") + "/" + data.second;
}

void LoginWindow::closeEvent(QCloseEvent *) {
    qApp->quit();
}
