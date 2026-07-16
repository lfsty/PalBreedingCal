#pragma once

#include <QHash>
#include <QJsonArray>
#include <QJsonObject>

class PlayerModel;

class PlayerListModel
{
public:
    PlayerListModel();
    ~PlayerListModel();

public:
    bool LoadPlayerData(const QJsonArray& jsonArray);
    bool LoadPlayerData(const QJsonObject& jsonObject);

private:
    bool CreateNewPlayer(const QJsonObject& jsonObject);

private:
    // <uid, playerModel>
    QHash<QString, PlayerModel*> m_playerMap;

public:
    friend QDebug operator<<(QDebug debug, const PlayerListModel& data);
};