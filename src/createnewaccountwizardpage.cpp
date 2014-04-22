#include "createnewaccountwizardpage.hpp"
#include "ui_createnewaccountwizardpage.h"

CreateNewAccountWizardPage::CreateNewAccountWizardPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::CreateNewAccountWizardPage)
{
    ui->setupUi(this);
}

CreateNewAccountWizardPage::~CreateNewAccountWizardPage()
{
    delete ui;
}
