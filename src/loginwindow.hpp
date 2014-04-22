#ifndef LOGINWINDOW_HPP
#define LOGINWINDOW_HPP

#include <memory>

#include <QMainWindow>
#include <QComboBox>
#include <QLineEdit>
#include <QWizard>

#include "account.h"
#include "mainwindow.hpp"

class LoginWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit LoginWindow(QWidget *parent = 0);

    void closeEvent(QCloseEvent*);

private:
    void loadUsers();

    QLineEdit* password;
    QComboBox* userName;

    std::shared_ptr<MainWindow> wnd;

    QWizard* wizard;

    QString tox;
signals:
    void accountSelected(AccountPtr account);

public slots:
    void onLoginClicked();
    void onCreateClicked();
    void onImportClicked();
    void userSelected(int index);
    void createUserFinish();
};

#endif // LOGINWINDOW_H
