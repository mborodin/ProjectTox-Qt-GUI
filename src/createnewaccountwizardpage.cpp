#include "createnewaccountwizardpage.hpp"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

CreateNewAccountWizardPage::CreateNewAccountWizardPage(QWidget *parent) :
    QWizardPage(parent)
{
    user = new QLineEdit(this);
    QLabel* lblUser = new QLabel(this);
    lblUser->setText(tr("Username:"));
    lblUser->setBuddy(user);

    password = new QLineEdit(this);
    QLabel* lblPassword = new QLabel(this);
    lblPassword->setText(tr("Password:"));
    lblPassword->setBuddy(password);

    registerField("username*", user);
    registerField("password", password);

    QWidget* userLine = new QWidget(this);
    QHBoxLayout* line = new QHBoxLayout(userLine);
    line->addWidget(lblUser);
    line->addWidget(user);

    QWidget* passwordLine = new QWidget(this);
    line = new QHBoxLayout(passwordLine);
    line->addWidget(lblPassword);
    line->addWidget(password);

    QVBoxLayout *layout = new QVBoxLayout(this);

    layout->addWidget(userLine);
    layout->addWidget(passwordLine);
    layout->setAlignment(Qt::AlignCenter);

    setLayout(layout);
}

CreateNewAccountWizardPage::~CreateNewAccountWizardPage()
{
}
