#include <QList>
#include <QSettings>
#include <QNetworkCookie>
#include "persistentcookiejar.h"

PersistentCookieJar::PersistentCookieJar(QObject *parent) :
    QNetworkCookieJar(parent)
{
    load();
}

PersistentCookieJar::~PersistentCookieJar()
{
    save();
}

void PersistentCookieJar::save()
{
    QList<QNetworkCookie> list = allCookies();
    QByteArray data;
    foreach (QNetworkCookie cookie, list) {
        if (!cookie.isSessionCookie()) {
            data.append(cookie.toRawForm());
            data.append("\n");
        }
    }
    QSettings settings;
    settings.setValue("Cookies",data);
}

void PersistentCookieJar::load()
{
    QSettings settings;
    QByteArray data = settings.value("Cookies").toByteArray();
    setAllCookies(QNetworkCookie::parseCookies(data));
}
