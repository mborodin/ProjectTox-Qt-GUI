#ifndef CREATENEWACCOUNTWIZARDPAGE_HPP
#define CREATENEWACCOUNTWIZARDPAGE_HPP

#include <QWizardPage>
#include <QLineEdit>

class CreateNewAccountWizardPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit CreateNewAccountWizardPage(QWidget *parent = 0);
    ~CreateNewAccountWizardPage();

private:
    QLineEdit *user;
    QLineEdit *password;

};

#endif // CREATENEWACCOUNTWIZARDPAGE_HPP
