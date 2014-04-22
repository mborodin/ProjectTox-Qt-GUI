#include "account.h"

#include <QFile>

#include <list>
#include <algorithm>

#include "core.hpp"

Account::Account(QObject *parent) :
    QObject(parent), ipV6Enabled(false)
{
    tox = tox_new(ipV6Enabled);
}

int Account::acceptFriendRequest(uint8_t *userId)
{
    return tox_add_friend_norequest(tox, userId);
}

int Account::sendFriendRequest(uint8_t *userId, uint8_t *message, size_t len)
{
    return tox_add_friend(tox, userId, message, len);
}

int Account::removeFriend(int userId) {
    return tox_del_friend(tox, userId);
}

QString Account::getAddress() const
{
    uint8_t address[TOX_FRIEND_ADDRESS_SIZE];
    tox_get_address(tox, address);
    return Core::CFriendAddress::toString(address);
}

int Account::sendMessage(int userId, uint8_t *message, size_t len)
{
    return tox_send_message(tox, userId, message, len);
}

int Account::sendAction(int userId, uint8_t *action, size_t len)
{
    return tox_send_action(tox, userId, action, len);
}

int Account::setAlias(uint8_t *alias, size_t len)
{
    return tox_set_name(tox, alias, len);
}

QString Account::getAlias() const {
    uint8_t name[TOX_FRIEND_ADDRESS_SIZE];
    int len = tox_get_self_name_size(tox);
    tox_get_self_name(tox, name);
    return Core::CString::toString(name, len);
}

int Account::setStatusMessage(uint8_t *message, size_t len)
{
    return tox_set_status_message(tox, message, len);
}

QString Account::getStatusMessage() const
{
    const int len = tox_get_self_status_message_size(tox);
    uint8_t message[len];
    tox_get_self_status_message(tox, message, len);
    return Core::CString::toString(message, len);
}

int Account::setStatus(const Status &status) {
    TOX_USERSTATUS userstatus;
    switch (status) {
        case Status::Online:
            userstatus = TOX_USERSTATUS_NONE;
            break;
        case Status::Away:
            userstatus = TOX_USERSTATUS_AWAY;
            break;
        case Status::Busy:
            userstatus = TOX_USERSTATUS_BUSY;
            break;
        default:
            userstatus = TOX_USERSTATUS_INVALID;
            break;
    }

    return tox_set_user_status(tox, userstatus);
}

QString Account::getFriendUserId(int fId) const
{
    uint8_t uid[TOX_CLIENT_ID_SIZE];

    tox_get_client_id(tox, fId, uid);

    QString ret = Core::CUserId::toString(uid);

    return ret;
}

QString Account::getFriendName(int fId) const
{
    uint8_t name[TOX_MAX_NAME_LENGTH];
    int len;

    if((len=tox_get_name_size(tox, fId)) != 0) {

        if(tox_get_name(tox, fId, name) == len)
            return Core::CString::toString(name, len);
    }
    return QString();
}

Status Account::getFriendStatus(int fid) const
{
    switch (tox_get_friend_connection_status(tox, fid)) {
        case TOX_USERSTATUS_NONE:
            return Status::Online;
        case TOX_USERSTATUS_AWAY:
            return Status::Away;
        case TOX_USERSTATUS_BUSY:
            return Status::Busy;
        default:
            return Status::Offline;
    }

}

void Account::addDHTServer(const char* address, uint16_t port, uint8_t* public_key) {
    tox_bootstrap_from_address(tox, address, ipV6Enabled, port, public_key);
}

void Account::setIPv6Enabled(bool enabled){
    ipV6Enabled = enabled;
}

bool Account::getIPv6Enabled() const {
    return ipV6Enabled;
}

Account::~Account() {
    if (tox) {

        tox_kill(tox);
    }
}

bool Account::isConnected() const
{
    return tox_isconnected(tox);
}

void Account::process() {
    tox_do(tox);
}

std::list<int> Account::getFriendList() const
{
    const size_t numFriends = 256;
    int friends[numFriends];
    uint32_t nRealFriends = tox_get_friendlist(tox, friends, numFriends);

    std::list<int> ret;

    uint32_t idx;
    for(idx = 0; idx < nRealFriends; ++idx)
        ret.push_back(friends[idx]);

    return ret;
}

void Account::bind(Core* core) {
    tox_callback_friend_request(tox, &Account::onFriendRequest, core);
    tox_callback_friend_message(tox, &Account::onFriendMessage, core);
    tox_callback_friend_action(tox, &Account::onAction, core);
    tox_callback_name_change(tox, &Account::onFriendNameChange, core);
    tox_callback_status_message(tox, &Account::onStatusMessageChanged, core);
    tox_callback_user_status(tox, &Account::onUserStatusChanged, core);
    tox_callback_connection_status(tox, &Account::onConnectionStatusChanged, core);
}

void Account::save(const QString &file, const QString &password) const
{
    QFile fout(file);

    fout.open(QIODevice::WriteOnly);

    uint8_t* data;
    uint32_t len;

    //tox = tox_new(ipV6Enabled);

    if(password.length()) {
        len = tox_size_encrypted(tox);
        data = new uint8_t[len];
        tox_save_encrypted(tox, data, (uint8_t*)password.toLocal8Bit().data(), password.length());
    } else {
        len = tox_size(tox);
        data = new uint8_t[len];
        tox_save(tox, data);
    }

    fout.write((char*)data, len);

    delete [] data;

    fout.close();

}

int Account::load(const QString &file, const QString& password)
{
    QFile fin(file);
    int res;

    fin.open(QIODevice::ReadOnly);

    uint32_t len = fin.size();
    uint8_t* data = new uint8_t[len];
    fin.read((char*)data, len);

    //tox = tox_new(ipV6Enabled);

    if(password.length())
        res=tox_load_encrypted(tox, data, len, (uint8_t*)password.toLocal8Bit().data(), password.length());
    else
        res=tox_load(tox, data, len);

    fin.close();
    delete [] data;

    return res;
}

void Account::onFriendRequest(Tox*/* tox*/, uint8_t* cUserId, uint8_t* cMessage, uint16_t cMessageSize, void* core)
{
    emit static_cast<Core*>(core)->friendRequestRecieved(Core::CUserId::toString(cUserId), Core::CString::toString(cMessage, cMessageSize));
}

void Account::onFriendMessage(Tox*/* tox*/, int friendId, uint8_t* cMessage, uint16_t cMessageSize, void* core)
{
    emit static_cast<Core*>(core)->friendMessageRecieved(friendId, Core::CString::toString(cMessage, cMessageSize));
}

void Account::onFriendNameChange(Tox*/* tox*/, int friendId, uint8_t* cName, uint16_t cNameSize, void* core)
{
    emit static_cast<Core*>(core)->friendUsernameChanged(friendId, Core::CString::toString(cName, cNameSize));
}

void Account::onStatusMessageChanged(Tox*/* tox*/, int friendId, uint8_t* cMessage, uint16_t cMessageSize, void* core)
{
    emit static_cast<Core*>(core)->friendStatusMessageChanged(friendId, Core::CString::toString(cMessage, cMessageSize));
}

void Account::onUserStatusChanged(Tox*/* tox*/, int friendId, uint8_t userstatus, void* core)
{
    Status status;
    switch (userstatus) {
        case TOX_USERSTATUS_NONE:
            status = Status::Online;
            break;
        case TOX_USERSTATUS_AWAY:
            status = Status::Away;
            break;
        case TOX_USERSTATUS_BUSY:
            status = Status::Busy;
            break;
        default:
            status = Status::Online;
            break;
    }
    emit static_cast<Core*>(core)->friendStatusChanged(friendId, status);
}

void Account::onConnectionStatusChanged(Tox*/* tox*/, int friendId, uint8_t status, void* core)
{
    emit static_cast<Core*>(core)->friendStatusChanged(friendId, status ? Status::Online : Status::Offline);
}

void Account::onAction(Tox*/* tox*/, int friendId, uint8_t *cMessage, uint16_t cMessageSize, void *core)
{
    emit static_cast<Core*>(core)->actionReceived(friendId, Core::CString::toString(cMessage, cMessageSize));
}
