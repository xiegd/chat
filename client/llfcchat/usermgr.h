#ifndef USERMGR_H
#define USERMGR_H
#include <QObject>
#include <memory>
#include <singleton.h>

class UserMgr:public QObject,public Singleton<UserMgr>,
        public std::enable_shared_from_this<UserMgr>
{
    Q_OBJECT
public:
    ~ UserMgr();
    void SetName(std::string name);
    void SetUid(std::string uid);
private:
    UserMgr();
};

#endif // USERMGR_H
