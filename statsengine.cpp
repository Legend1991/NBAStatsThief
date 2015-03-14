#include "statsengine.h"
#include "gamemodel.h"
#include "QPair"
#include <QDebug>

StatsEngine::StatsEngine(QList<GameModel> game, QList<QPair<QString, QString> > gamesForCalc)
{
    m_Games = game;
    m_GamesForCalc = gamesForCalc;
}

StatsEngine::~StatsEngine()
{

}

void StatsEngine::findScores()
{
    foreach (Game findGame, m_GamesForCalc)
    {
        //    qDebug() << findGame.getDate()
        //             << findGame.getHomeTeam() << findGame.getHomeScore()
        //             << findGame.getVisitorScore() << findGame.getVisitorTeam();

        QList<GameModel> homeGS;
        for (int i = m_Games.count() - 1, n = 0; i > 0 && n < 4; --i)
        {
            if (m_Games.value(i).getHomeTeam() == findGame.first)
            {
                homeGS.append(m_Games.value(i));
                //            qDebug() << m_Games.value(i).getDate()
                //                     << m_Games.value(i).getHomeTeam() << m_Games.value(i).getHomeScore()
                //                     << m_Games.value(i).getVisitorScore() << m_Games.value(i).getVisitorTeam();
                n++;
            }
            else if (m_Games.value(i).getVisitorTeam() == findGame.first)
            {
                homeGS.append(m_Games.value(i));
                //            qDebug() << m_Games.value(i).getDate()
                //                     << m_Games.value(i).getHomeTeam() << m_Games.value(i).getHomeScore()
                //                     << m_Games.value(i).getVisitorScore() << m_Games.value(i).getVisitorTeam();
                n++;
            }
        }

        QList<GameModel> visitorGS;
        for (int i = m_Games.count() - 1, n = 0; i > 0 && n < 4; --i)
        {
            if (m_Games.value(i).getHomeTeam() == findGame.second)
            {
                visitorGS.append(m_Games.at(i));
                n++;
            }
            else if ((m_Games.value(i)).getVisitorTeam() == findGame.second)
            {
                visitorGS.append(m_Games.at(i));
                n++;
            }
        }

        if (homeGS.count() != 4 || visitorGS.count() != 4)
        {
            qDebug() << "Can't find enouth numbers of game";
            continue;
        }

        QPair<float, float> homeT;
        homeT.first = 0;
        homeT.second = 0;
        QPair<float, float> visitorT;
        visitorT.first = 0;
        visitorT.second = 0;
        QPair<QList<int>, QList<int> > homeS;
        QPair<QList<int>, QList<int> > visitorS;

        foreach (GameModel game, homeGS) {
            if (game.getHomeTeam() == findGame.first)
            {
                //            homeT.first += game.getHomeScore();
                //            homeT.second += game.getVisitorScore();
                homeS.first.append(game.getHomeScore());
                homeS.second.append(game.getVisitorScore());
            }
            else
            {
                //            homeT.first += game.getVisitorScore();
                //            homeT.second += game.getHomeScore();
                homeS.first.append(game.getVisitorScore());
                homeS.second.append(game.getHomeScore());
            }
        }

        foreach (GameModel game, visitorGS) {
            if (game.getHomeTeam() == findGame.second)
            {
                //            visitorT.first += game.getHomeScore();
                //            visitorT.second += game.getVisitorScore();
                visitorS.first.append(game.getHomeScore());
                visitorS.second.append(game.getVisitorScore());
            }
            else
            {
                //            visitorT.first += game.getVisitorScore();
                //            visitorT.second += game.getHomeScore();
                visitorS.first.append(game.getVisitorScore());
                visitorS.second.append(game.getHomeScore());
            }
        }

        int homeTAMax = 0;
        int homeTAMin = 0;
        int homeTDMax = 0;
        int homeTDMin = 0;
        int visitorTAMax = 0;
        int visitorTAMin = 0;
        int visitorTDMax = 0;
        int visitorTDMin = 0;
        for (int i = 1; i < 4; ++i)
        {
            if (homeS.first.at(i) > homeS.first.at(homeTAMax)) homeTAMax = i;
            if (homeS.first.at(i) < homeS.first.at(homeTAMin)) homeTAMin = i;
            if (visitorS.first.at(i) > visitorS.first.at(visitorTAMax)) visitorTAMax = i;
            if (visitorS.first.at(i) < visitorS.first.at(visitorTAMin)) visitorTAMin = i;
            if (homeS.second.at(i) > homeS.second.at(homeTDMax)) homeTDMax = i;
            if (homeS.second.at(i) < homeS.second.at(homeTDMin)) homeTDMin = i;
            if (visitorS.second.at(i) > visitorS.second.at(visitorTDMax)) visitorTDMax = i;
            if (visitorS.second.at(i) < visitorS.second.at(visitorTDMin)) visitorTDMin = i;
        }
        for (int i = 0; i < 4; ++i)
        {
            if (i != homeTAMax && i != homeTAMin) homeT.first += homeS.first.at(i);
            if (i != homeTDMax && i != homeTDMin) homeT.second += homeS.second.at(i);
            if (i != visitorTAMax && i != visitorTAMin) visitorT.first += visitorS.first.at(i);
            if (i != visitorTDMax && i != visitorTDMin) visitorT.second += visitorS.second.at(i);
        }

        homeT.first = homeT.first / 2;
        homeT.second = homeT.second / 2;
        visitorT.first = visitorT.first / 2;
        visitorT.second = visitorT.second / 2;

        int homeTeamScore = qRound(homeT.first + visitorT.second - 100);
        int visitorTeamScore = qRound(visitorT.first + homeT.second - 100);
        qDebug() << findGame.first << homeTeamScore << findGame.second << visitorTeamScore;
    }
}

