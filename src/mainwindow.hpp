/*
    Copyright (C) 2013 by Maxim Biro <nurupo.contributions@gmail.com>
    
    This file is part of Tox Qt GUI.
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
    
    See the COPYING file for more details.
*/

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "core.hpp"
#include "friendswidget.hpp"
#include "ouruseritemwidget.hpp"

#include <QLineEdit>
#include <QListView>
#include <QMainWindow>
#include <QTextBrowser>
#include <QThread>
#include <QSplitter>
#include <QSystemTrayIcon>

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setAccount(std::shared_ptr<Account>& account);

protected:
    void closeEvent(QCloseEvent *event);

private:
    Core* core;
    QThread* coreThread;
    FriendsWidget* friendsWidget;
    OurUserItemWidget* ourUserItem;
    QSplitter* splitterWidget;
    QSystemTrayIcon* trayIcon;
    QAction* settingsAction;
    QAction* trayMenuShowHideAction;
    QList<QAction*> trayMenuStatusActions;

private slots:
    void onAddFriendButtonClicked();
    void onConnected();
    void onDisconnected();
    void onFriendRequestRecieved(const QString &userId, const QString &message);
    void onFailedToRemoveFriend(int friendId);
    void onFailedToAddFriend(const QString& userId);
    void onFailedToStartCore();
    void onSettingsActionTriggered();
    void onAboutAppActionTriggered();
    void onTrayMenuStatusActionTriggered();
    void onTrayMenuQuitApplicationActionTriggered();
    void onShowHideWindow();
    void onTrayIconClick(QSystemTrayIcon::ActivationReason reason);
    void onStatusSet(Status status);

signals:
    void friendRequestAccepted(const QString& userId);
    void friendRequested(const QString& friendAddress, const QString& message);
    void statusSet(Status status);

};

#endif // MAINWINDOW_HPP
