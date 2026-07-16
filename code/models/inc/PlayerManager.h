#pragma once

#include <QHash>
#include <QJsonArray>
#include <QJsonObject>
#include <QObject>

class PlayerModel;

class PlayerManager : public QObject
{
    Q_OBJECT
public:
    static PlayerManager* getInstance();

private:
    PlayerManager();
    ~PlayerManager();

public:
    const QHash<QString, PlayerModel*>& getPlayerMap() const { return m_playerMap; }
    const PlayerModel* getCurrentPlayer() const { return m_currentApplyPlayer; }
    void setCurrentPlayer(const QString& playerUID);

public:
    bool LoadPlayerData(const QJsonArray& jsonArray);
    bool LoadPlayerData(const QJsonObject& jsonObject);

private:
    bool CreateNewPlayer(const QJsonObject& jsonObject);

private:
    // <uid, playerModel>
    QHash<QString, PlayerModel*> m_playerMap;

    PlayerModel* m_currentApplyPlayer = nullptr;

signals:
    void CurrentPlayerChanged(const PlayerModel* currentPlayerModel);

public:
    friend QDebug operator<<(QDebug debug, const PlayerManager& data);
};