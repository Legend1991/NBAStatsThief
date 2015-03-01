#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <QDate>
#include <QString>

class GameModel
{
public:
    GameModel(QDate date, QString homeTeam, int homeScore,
              QString visitorTeam, int visitorScore);
    ~GameModel();
    QDate getDate() { return m_Date; }
    QString getHomeTeam() { return m_HomeTeam; }
    int getHomeScore() { return m_HomeScore; }
    QString getVisitorTeam() { return m_VisitorTeam; }
    int getVisitorScore() { return m_VisitorScore; }

private:
    QDate m_Date;
    QString m_HomeTeam;
    int m_HomeScore;
    QString m_VisitorTeam;
    int m_VisitorScore;
};

#endif // GAMEMODEL_H
