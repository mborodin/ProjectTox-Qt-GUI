#ifndef CREATENEWACCOUNTWIZARDPAGE_HPP
#define CREATENEWACCOUNTWIZARDPAGE_HPP

#include <QWizardPage>

namespace Ui {
class CreateNewAccountWizardPage;
}

class CreateNewAccountWizardPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit CreateNewAccountWizardPage(QWidget *parent = 0);
    ~CreateNewAccountWizardPage();

private:
    Ui::CreateNewAccountWizardPage *ui;
};

#endif // CREATENEWACCOUNTWIZARDPAGE_HPP
