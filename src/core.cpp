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

#include "core.hpp"
#include "mainwindow.hpp"
#include "Settings/settings.hpp"

#include <QThread>
#include <QTime>

#define MESSENGER_ACCOUNT_DIR "~/.tox"
#define MESSENGER_ACCOUNT_INFO "account.xml"

Core::Core() :
    account(nullptr)
{
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Core::process);
    connect(&Settings::getInstance(), &Settings::dhtServerListChanged, this, &Core::bootstrapDht);
}

Core::~Core()
{

}

void Core::init(QThread* coreThread) {
    this->moveToThread(coreThread);
    connect(coreThread, &QThread::started, this, &Core::start);
}

void Core::acceptFriendRequest(const QString& userId)
{
    int friendId = account->acceptFriendRequest(CUserId(userId).data());
    if (friendId == -1) {
        emit failedToAddFriend(userId);
    } else {
        emit friendAdded(friendId, userId, Status::Offline);
    }
}

void Core::requestFriendship(const QString& friendAddress, const QString& message)
{
    CString cMessage(message);

    int friendId = account->sendFriendRequest(CFriendAddress(friendAddress).data(), cMessage.data(), cMessage.size());
    const QString userId = friendAddress.mid(0, TOX_CLIENT_ID_SIZE * 2);
    // TODO: better error handling
    if (friendId < 0) {
        emit failedToAddFriend(userId);
    } else {
        emit friendAdded(friendId, userId, Status::Offline);
    }
}

void Core::sendMessage(int friendId, const QString& message)
{
    CString cMessage(message);

    int messageId = account->sendMessage(friendId, cMessage.data(), cMessage.size());
    emit messageSentResult(friendId, message, messageId);
}

void Core::sendAction(int friendId, const QString &action)
{
    CString cMessage(action);
    int ret = account->sendAction(friendId, cMessage.data(), cMessage.size());
    emit actionSentResult(friendId, action, ret);
}

void Core::removeFriend(int friendId)
{
    if (account->removeFriend(friendId) == -1) {
        emit failedToRemoveFriend(friendId);
    } else {
        emit friendRemoved(friendId);
    }
}

void Core::setUsername(const QString& username)
{
    CString cUsername(username);

    if (account->setAlias(cUsername.data(), cUsername.size()) == -1) {
        emit failedToSetUsername(username);
    } else {
        emit usernameSet(username);
    }
}

void Core::setStatusMessage(const QString& message)
{
    CString cMessage(message);

    if (account->setStatusMessage(cMessage.data(), cMessage.size()) == -1) {
        emit failedToSetStatusMessage(message);
    } else {
        emit statusMessageSet(message);
    }
}

void Core::setStatus(const Status& status)
{
    if (account->setStatus(status) == 0) {
        emit statusSet(status);
    } else {
        emit failedToSetStatus(status);
    }
}

void Core::bootstrapDht()
{
    const Settings& s = Settings::getInstance();
    QList<Settings::DhtServer> dhtServerList = s.getDhtServerList();

    for (const Settings::DhtServer& dhtServer : dhtServerList) {
       account->addDHTServer(dhtServer.address.toLatin1().data(), htons(dhtServer.port), CUserId(dhtServer.userId).data());
    }
}

void Core::checkConnection()
{
    static bool isConnected = false;

    if (account == nullptr)
        return;

    bool accountConnected = account->isConnected();

    if (accountConnected && !isConnected) {
        emit connected();
        isConnected = true;
    } else if (!accountConnected && isConnected) {
        emit disconnected();
        isConnected = false;
    }
}

void Core::process()
{
    if(account != nullptr)
        account->process();

#ifdef DEBUG
    //we want to see the debug messages immediately
    fflush(stdout);
#endif
    checkConnection();
}

void Core::start()
{
    // tox = tox_new(0);
    timer->setInterval(45);
    timer->start();
}

void Core::accountAdded(std::shared_ptr<Account> account)
{
    this->account = account;

    if (account == nullptr) {
        emit failedToStart();
        return;
    }

    account->bind(this);

    emit friendAddressGenerated(account->getAddress());

    /*CString cUsername(Settings::getInstance().getUsername());
    account->setAlias(cUsername.data(), cUsername.size());

    CString cStatusMessage(Settings::getInstance().getStatusMessage());
    account->setStatusMessage(cStatusMessage.data(), cStatusMessage.size());
    */

    emit usernameSet(account->getAlias());
    emit statusMessageSet(account->getStatusMessage());

    bootstrapDht();

    for(auto fId: account->getFriendList()) {
        QString cid = account->getFriendUserId(fId);
        QString name = account->getFriendName(fId);
        Status status = account->getFriendStatus(fId);
        emit friendAdded(fId, cid, status);
        if (name.length() != 0)
            emit friendUsernameChanged(fId, name);
    }
}


// CData

Core::CData::CData(const QString &data, uint16_t byteSize)
{
    cData = new uint8_t[byteSize];
    cDataSize = fromString(data, cData);
}

Core::CData::~CData()
{
    delete[] cData;
}

uint8_t* Core::CData::data()
{
    return cData;
}

uint16_t Core::CData::size()
{
    return cDataSize;
}

QString Core::CData::toString(uint8_t* cData, uint16_t cDataSize)
{
    return QString(QByteArray(reinterpret_cast<char*>(cData), cDataSize).toHex()).toUpper();
}

uint16_t Core::CData::fromString(const QString& data, uint8_t* cData)
{
    QByteArray arr = QByteArray::fromHex(data.toLower().toLatin1());
    memcpy(cData, reinterpret_cast<uint8_t*>(arr.data()), arr.size());
    return arr.size();
}


// CUserId

Core::CUserId::CUserId(const QString &userId) :
    CData(userId, SIZE)
{
    // intentionally left empty
}

QString Core::CUserId::toString(uint8_t* cUserId)
{
    return CData::toString(cUserId, SIZE);
}


// CFriendAddress

Core::CFriendAddress::CFriendAddress(const QString &friendAddress) :
    CData(friendAddress, SIZE)
{
    // intentionally left empty
}

QString Core::CFriendAddress::toString(uint8_t* cFriendAddress)
{
    return CData::toString(cFriendAddress, SIZE);
}


// CString

Core::CString::CString(const QString& string)
{
    cString = new uint8_t[string.length() * MAX_SIZE_OF_UTF8_ENCODED_CHARACTER]();
    cStringSize = fromString(string, cString);
}

Core::CString::~CString()
{
    delete[] cString;
}

uint8_t* Core::CString::data()
{
    return cString;
}

uint16_t Core::CString::size()
{
    return cStringSize;
}

QString Core::CString::toString(uint8_t* cString, uint16_t cStringSize)
{
    return QString::fromUtf8(reinterpret_cast<char*>(cString), cStringSize);
}

uint16_t Core::CString::fromString(const QString& string, uint8_t* cString)
{
    QByteArray byteArray = QByteArray(string.toUtf8());
    memcpy(cString, reinterpret_cast<uint8_t*>(byteArray.data()), byteArray.size());
    return byteArray.size();
}
