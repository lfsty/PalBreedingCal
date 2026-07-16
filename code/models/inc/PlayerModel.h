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

    const QString& getPlayerUID() const { return m_playerUID; }
    const QString& getPlayerNickName() const { return m_playerNickName; }
    const bool isPalDataLoaded() const { return m_palDataLoaded; }

    void apply();
    bool isApply();

private:
    QString m_playerUID      = "";
    QString m_playerNickName = "";
    QVector<PlayerPalModel*> m_palList;

    bool m_palDataLoaded = false;

public:
    friend QDebug operator<<(QDebug debug, const PlayerModel& data);
};