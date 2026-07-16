#pragma once

#include <QJsonObject>
#include <QObject>
#include <QString>
#include <QVector>

class PlayerPalModel;

class PlayerModel
{
public:
    explicit PlayerModel();
    ~PlayerModel();

public:
    bool loadPlayerDataFromJsonObject(const QJsonObject& jsonObject);
    bool loadPalListDataFromJsonObject(const QJsonObject& jsonObject);

private:
    QString m_playerUID      = "";
    QString m_playerNickName = "";
    QVector<PlayerPalModel*> m_palList;

    bool m_palDataLoaded = false;

public:
    friend QDebug operator<<(QDebug debug, const PlayerModel& data);
};