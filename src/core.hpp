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

#ifndef CORE_HPP
#define CORE_HPP

#include "status.hpp"
#include "account.h"

#include <QObject>
#include <QTimer>
#include <QList>
#include <QThread>

class MainWindow;

#define INVALID_ID 0xFFFF
#define GET_USER_ID(a) ( (int)((a) & 0x0000FFFF) )
#define GET_GROUP_ID(a) ( (a) >> 32 )
#define MAKE_USER_ID(a) ( (a) | 0xFFFF0000 )
#define MAKE_GROUP_ID(a) ( ( ((int64_t)(a)) << 32 ) | 0x0000FFFF )
#define MAKE_GROUP_USER_ID(a, b) ( ( ((int64_t)(a)) << 32 ) | b )
#define ENSURE_GID(a) ( ( (a) & 0xFFFF0000 ) | 0x0000FFFF )
#define ENSURE_UID(a) ( ( 0xFFFF0000 | ( (a) & 0x0000FFFF ) ) )
#define ENSURE_ONE_OF(a) ( GET_GROUP_ID(a) == INVALID_ID ? a : ENSURE_GID(a) )

class Core : public QObject
{
    Q_OBJECT
public:
    explicit Core();
    ~Core();

    void init(QThread* coreThread);
    
    void groupActionReceived(int groupnumber, int friendgroupnumber, const QString& action);
    void groupMessageReceived(int groupnumber, int friendgroupnumber, const QString& message);

private:

    void checkConnection();

    QTimer* timer;
    std::shared_ptr<Account> account;

    class CData
    {
    public:
        uint8_t* data();
        uint16_t size();

    protected:
        explicit CData(const QString& data, uint16_t byteSize);
        virtual ~CData();

        static QString toString(uint8_t* cData, uint16_t cDataSize);

    private:
        uint8_t* cData;
        uint16_t cDataSize;

        static uint16_t fromString(const QString& userId, uint8_t* cData);
    };

    public:
    class CUserId : public CData
    {
    public:
        explicit CUserId(const QString& userId);

        static QString toString(uint8_t* cUserId);

    private:
        static const uint16_t SIZE = TOX_CLIENT_ID_SIZE;

    };

    class CFriendAddress : public CData
    {
    public:
        explicit CFriendAddress(const QString& friendAddress);

        static QString toString(uint8_t* cFriendAddress);

    private:
        static const uint16_t SIZE = TOX_FRIEND_ADDRESS_SIZE;

    };

    class CString
    {
    public:
        explicit CString(const QString& string);
        ~CString();

        uint8_t* data();
        uint16_t size();

        static QString toString(uint8_t* cMessage, uint16_t cMessageSize);

    private:
        const static int MAX_SIZE_OF_UTF8_ENCODED_CHARACTER = 4;

        uint8_t* cString;
        uint16_t cStringSize;

        static uint16_t fromString(const QString& message, uint8_t* cMessage);
    };

public slots:
    void start();

    void accountAdded(std::shared_ptr<Account> account);

    void acceptFriendRequest(const QString& userId);
    void requestFriendship(const QString& friendAddress, const QString& message);

    void removeFriend(int friendId);

    void sendMessage(int64_t friendId, const QString& message);
    void sendAction(int64_t friendId, const QString& action);

    void setUsername(const QString& username);
    void setStatusMessage(const QString& message);
    void setStatus(const Status& status);

    void process();

    void bootstrapDht();

signals:
    void connected();
    void disconnected();

    void friendRequestReceived(const QString& userId, const QString& message);
    void friendMessageReceived(int64_t friendId, const QString& message);

    void friendAdded(int friendId, const QString& userId, const Status& status);

    void friendStatusChanged(int friendId, Status status);
    void friendStatusMessageChanged(int friendId, const QString& message);
    void friendUsernameChanged(int friendId, const QString& username);

    void friendAddressGenerated(const QString& friendAddress);

    void friendRemoved(int friendId);

    void usernameSet(const QString& username);
    void statusMessageSet(const QString& message);
    void statusSet(const Status& status);

    void messageSentResult(int64_t friendId, const QString& message, int messageId);
    void actionSentResult(int64_t friendId, const QString& action, int success);
    
    void typingChanged(int friendId, bool isTyping);

    void failedToAddFriend(const QString& userId);
    void failedToRemoveFriend(int friendId);
    void failedToSetUsername(const QString& username);
    void failedToSetStatusMessage(const QString& message);
    void failedToSetStatus(const Status& status);
    
    void groupInviteReceived(int friendnumber, uint8_t *group_public_key);
    void groupPeerAdded(int groupnumber, int peernumber);
    void groupPeerRemoved(int groupnumber, int peernumber);
    void groupPeerRenamed(int groupnumber, int peernumber);

    void actionReceived(int friendId, const QString& acionMessage);

    void failedToStart();

};

#endif // CORE_HPP

