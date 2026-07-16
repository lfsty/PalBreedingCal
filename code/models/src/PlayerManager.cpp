#include "PlayerManager.h"

#include <PlayerModel.h>

#include <QDebug>

PlayerManager* PlayerManager::getInstance()
{
    static PlayerManager instance;
    return &instance;
}

PlayerManager::PlayerManager()
{
}

PlayerManager::~PlayerManager()
{
    for (auto player : m_playerMap.values())
    {
        delete player;
    }
    m_playerMap.clear();
}

void PlayerManager::setCurrentPlayer(const QString& playerUID)
{
    PlayerModel* player = m_playerMap.value(playerUID);
    if (player)
    {
        if (player != m_currentApplyPlayer)
        {
            m_currentApplyPlayer = player;
            emit CurrentPlayerChanged(m_currentApplyPlayer);
        }
    }
    else
    {
        m_currentApplyPlayer = nullptr;
    }
}

bool PlayerManager::LoadPlayerData(const QJsonArray& jsonArray)
{
    for (auto& jsonPlayer : jsonArray)
    {
        if (!LoadPlayerData(jsonPlayer.toObject()))
        {
            return false;
        }
    }

    return !m_playerMap.empty();
}

bool PlayerManager::LoadPlayerData(const QJsonObject& jsonObject)
{
    if (jsonObject.empty())
    {
        return false;
    }

    for (auto player : m_playerMap.values())
    {
        if (player->loadPlayerDataFromJsonObject(jsonObject))
        {
            return true;
        }
    }

    // 到这，说明当前jsonObject中的数据没有匹配到任何已有的player
    // 创建一个新的player，并添加到m_playerMap中

    return CreateNewPlayer(jsonObject);
}

bool PlayerManager::CreateNewPlayer(const QJsonObject& jsonObject)
{
    if (jsonObject.empty())
    {
        return false;
    }

    PlayerModel* player = new PlayerModel();
    if (player->loadPlayerDataFromJsonObject(jsonObject))
    {
        m_playerMap.insert(player->getPlayerUID(), player);
        return true;
    }
    else
    {
        delete player;
        return false;
    }
}

QDebug operator<<(QDebug debug, const PlayerManager& data)
{
    for (auto player : data.m_playerMap.values())
    {
        debug << *player << "\n";
    }
    return debug;
}
