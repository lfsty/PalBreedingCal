#include "PlayerListModel.h"

#include <PlayerModel.h>

#include <QDebug>

PlayerListModel::PlayerListModel()
{
}

PlayerListModel::~PlayerListModel()
{
    for (auto& player : m_playerList)
    {
        delete player;
    }
    m_playerList.clear();
}

bool PlayerListModel::LoadPlayerListDataFromJsonArray(const QJsonArray& jsonArray)
{
    for (auto& jsonPlayer : jsonArray)
    {
        PlayerModel* player = new PlayerModel();
        if (player->loadPlayerDataFromJsonObject(jsonPlayer.toObject()))
        {
            m_playerList.append(player);
        }
        else
        {
            delete player;
        }
    }

    return !m_playerList.empty();
}

bool PlayerListModel::LoadPlayerDataFromJsonObject(const QJsonObject& jsonObject)
{
    for (auto player : m_playerList)
    {
        if (player->loadPalListDataFromJsonObject(jsonObject))
        {
            return true;
        }
    }

    return false;
}

QDebug operator<<(QDebug debug, const PlayerListModel& data)
{
    for (auto player : data.m_playerList)
    {
        debug << *player << "\n";
    }
    return debug;
}
