#ifndef PERSISTENTCOOKIEJAR_H
#define PERSISTENTCOOKIEJAR_H

#include <QObject>
#include <QNetworkCookieJar>

class PersistentCookieJar : public QNetworkCookieJar
{
public:
    PersistentCookieJar(QObject *parent);
    ~PersistentCookieJar();

private:
    void save();
    void load();
};

#endif // PERSISTENTCOOKIEJAR_H
