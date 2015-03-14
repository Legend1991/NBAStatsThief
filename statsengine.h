#ifndef STATSENGINE_H
#define STATSENGINE_H

#include <QList>
#include <gamemodel.h>

class StatsEngine
{
public:
    StatsEngine(QList<GameModel> game);
    ~StatsEngine();
    void findScores();

private:
    QList<GameModel> m_Games;
};

#endif // STATSENGINE_H
