/*
    Copyright (C) 2014 by Maksym Borodin <Borodin.Maksym@gmail.com>
    
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

#ifndef ICHATPAGEWIDGET_HPP
#define ICHATPAGEWIDGET_HPP

#include <QWidget>

#include <core.hpp>

class IChatPageWidget: public QWidget {
    Q_OBJECT
protected:
    int64_t friendId;
public:
    explicit IChatPageWidget(QWidget *parent = 0): QWidget(parent) {}
    virtual void setTitle(const QString& title) = 0;
    int64_t getFriendId() const {
	return friendId;
    }
    void setFriendId(int64_t id) {
	friendId = ENSURE_ONE_OF(id);
    }
public slots:
    virtual void messageReceived(const QString& message) = 0;
    virtual void messageSentResult(const QString& message, int messageId) = 0;
    virtual void actionReceived(const QString& message) = 0;
    virtual void actionSentResult(const QString& message) = 0;

signals:
    void sendMessage(const QString& message);
    void sendAction(const QString& action);
}; 

#endif // ICHATPAGEWIDGET_HPP
