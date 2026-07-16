#pragma once

#include <QJsonArray>
#include <QJsonObject>
#include <QVector>

class PlayerModel;

class PlayerListModel
{
public:
    PlayerListModel();
    ~PlayerListModel();

public:
    bool LoadPlayerListDataFromJsonArray(const QJsonArray& jsonArray);
    bool LoadPlayerDataFromJsonObject(const QJsonObject& jsonObject);

private:
    QVector<PlayerModel*> m_playerList;

public:
    friend QDebug operator<<(QDebug debug, const PlayerListModel& data);
};