#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <QDate>
#include <QString>

class GameModel
{
public:
    GameModel(QDate date, QString homeTeam, QString visitorTeam,
              int hQ1, int vQ1,
              int hQ2, int vQ2,
              int hQ3, int vQ3,
              int hQ4, int vQ4,
              int hTotal, int vTotal);
    ~GameModel();
    QDate getDate() { return m_Date; }
    QString getHomeTeam() { return m_HomeTeam; }
    int getHTotal() { return m_HTotal; }
    QString getVisitorTeam() { return m_VisitorTeam; }
    int getHQ1() { return m_hQ1; }
    int getVQ1() { return m_vQ1; }
    int getHQ2() { return m_hQ2; }
    int getVQ2() { return m_vQ2; }
    int getHQ3() { return m_hQ3; }
    int getVQ3() { return m_vQ3; }
    int getHQ4() { return m_hQ4; }
    int getVQ4() { return m_vQ4; }

private:
    QDate m_Date;
    QString m_HomeTeam;
    int m_HTotal;
    QString m_VisitorTeam;
    int m_VTotal;
    int m_hQ1;
    int m_vQ1;
    int m_hQ2;
    int m_vQ2;
    int m_hQ3;
    int m_vQ3;
    int m_hQ4;
    int m_vQ4;
};

#endif // GAMEMODEL_H
