#ifndef ACCOUNT_H
#define ACCOUNT_H

#include<memory>

#include <QObject>

#include <tox.h>

#include "status.hpp"

class Core;
class Account;

typedef std::shared_ptr<Account> AccountPtr;

class Account : public QObject
{
    Q_OBJECT
public:
    explicit Account(QObject *parent = 0);
    ~Account();

    void init();
    void bind(Core* core);
    void save(const QString& file, const QString& password) const;
    int load(const QString &file, const QString& password);

    int acceptFriendRequest(uint8_t* userId);
    int sendFriendRequest(uint8_t* userId, uint8_t* message, size_t len);
    int sendMessage(int userId, uint8_t* message, size_t len);
    int sendAction(int userId, uint8_t* action, size_t len);
    int removeFriend(int userId);
    QString getAddress() const;
    int setAlias(uint8_t* alias, size_t len);
    QString getAlias() const;
    int setStatusMessage(uint8_t* message, size_t len);
    QString getStatusMessage() const;
    int setStatus(const Status& status);

    QString getFriendUserId(int fId) const;
    QString getFriendName(int fId) const;
    Status getFriendStatus(int fid) const;

    void setIPv6Enabled(bool enabled);
    bool getIPv6Enabled() const;
    void addDHTServer(const char* addr, uint16_t port, uint8_t* public_key);
    bool isConnected() const;
    void process();

    std::list<int> getFriendList() const;

private:
    static void onFriendRequest(Tox* tox, uint8_t* cUserId, uint8_t* cMessage, uint16_t cMessageSize, void* core);
    static void onFriendMessage(Tox* tox, int friendId, uint8_t* cMessage, uint16_t cMessageSize, void* core);
    static void onFriendNameChange(Tox* tox, int friendId, uint8_t* cName, uint16_t cNameSize, void* core);
    static void onStatusMessageChanged(Tox* tox, int friendId, uint8_t* cMessage, uint16_t cMessageSize, void* core);
    static void onUserStatusChanged(Tox* tox, int friendId, uint8_t userstatus, void* core);
    static void onConnectionStatusChanged(Tox* tox, int friendId, uint8_t status, void* core);
    static void onAction(Tox* tox, int friendId, uint8_t* cMessage, uint16_t cMessageSize, void* core);
    
    void saveState();

    Tox* tox;
    bool ipV6Enabled;
    QString pass;
    QString stateFile;

signals:
    void accountAdded(AccountPtr account);

public slots:

};

#endif // ACCOUNT_H
