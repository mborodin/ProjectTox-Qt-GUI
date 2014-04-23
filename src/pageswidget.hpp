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

#ifndef PAGESWIDGET_HPP
#define PAGESWIDGET_HPP

#include "chatpagewidget.hpp"

#include <QStackedWidget>
#include <QHash>

class PagesWidget : public QStackedWidget
{
    Q_OBJECT
public:
    PagesWidget(QWidget* parent);

private:
    ChatPageWidget* widget(int64_t friendId) const;
    
    QHash<int64_t, ChatPageWidget*> pages;

private slots:
    void onMessageSent(const QString& message);
    void onActionToSend(const QString& action);

public slots:
    void addPage(int64_t friendId, const QString& username);
    void removePage(int64_t friendId);
    void activatePage(int64_t friendId);
    void statusChanged(int64_t friendId, Status status);
    void usernameChanged(int64_t friendId, const QString& username);
    void statusMessageChanged(int64_t friendId, const QString& statusMessage);
    void messageReceived(int64_t friendId, const QString& message);
    void actionReceived(int64_t friendId, const QString& message);

    void messageSentResult(int64_t friendId, const QString& message, int messageId);
    void actionResult(int64_t friendId, const QString &action, int success);

signals:
    void sendMessage(int64_t friendId, const QString& message);
    void sendAction(int64_t friendId, const QString& action);

};

#endif // PAGESWIDGET_HPP
