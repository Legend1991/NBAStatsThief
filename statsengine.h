#ifndef STATSENGINE_H
#define STATSENGINE_H

#include <QList>
#include <gamemodel.h>

typedef QPair<QString, QString> Game;

class StatsEngine
{
public:
    StatsEngine(QList<GameModel> game, QList<QPair<QString, QString> > gamesForCalc);
    ~StatsEngine();
    void findScores();

private:
    QList<GameModel> m_Games;
    QList<Game> m_GamesForCalc;
};

#endif // STATSENGINE_H
