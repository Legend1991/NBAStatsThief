#include "gamemodel.h"

GameModel::GameModel()
{
    _isValid = false;
}

GameModel::GameModel(QDate date, QString homeTeam, QString visitorTeam,
                     int hQ1, int vQ1,
                     int hQ2, int vQ2,
                     int hQ3, int vQ3,
                     int hQ4, int vQ4,
                     int hTotal, int vTotal)
{
    _isValid = true;
    m_Date = date;
    m_HomeTeam = homeTeam;
    m_HTotal = hTotal;
    m_VisitorTeam = visitorTeam;
    m_VTotal = vTotal;
    m_hQ1 = hQ1;
    m_vQ1 = vQ1;
    m_hQ2 = hQ2;
    m_vQ2 = vQ2;
    m_hQ3 = hQ3;
    m_vQ3 = vQ3;
    m_hQ4 = hQ4;
    m_vQ4 = vQ4;
}

GameModel::~GameModel()
{

}

