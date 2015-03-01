#include "gamemodel.h"

GameModel::GameModel(QDate date, QString homeTeam, int homeScore,
                     QString visitorTeam, int visitorScore)
{
    m_Date = date;
    m_HomeTeam = homeTeam;
    m_HomeScore = homeScore;
    m_VisitorTeam = visitorTeam;
    m_VisitorScore = visitorScore;
}

GameModel::~GameModel()
{

}

