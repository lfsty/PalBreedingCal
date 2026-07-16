#include "PlayerModel.h"

#include <QJsonArray>

#include <PlayerPalModel.h>

#include <QDebug>

namespace
{
constexpr const char* PLAYER_UID_KEY       = "player_uid";
constexpr const char* PLAYER_NICK_NAME_KEY = "nickname";
constexpr const char* PAL_LIST_KEY         = "pals";
}  // namespace

PlayerModel::PlayerModel()
{
}

PlayerModel::~PlayerModel()
{
    for (auto pal : m_palList)
    {
        delete pal;
    }
    m_palList.clear();
}

bool PlayerModel::loadPlayerDataFromJsonObject(const QJsonObject& jsonObject)
{
    if (jsonObject.empty())
    {
        return false;
    }

    if (!jsonObject.contains(PLAYER_UID_KEY) || !jsonObject.contains(PLAYER_NICK_NAME_KEY))
    {
        return false;
    }

    m_playerUID      = jsonObject["player_uid"].toString();
    m_playerNickName = jsonObject["nickname"].toString();

    return true;
}

bool PlayerModel::loadPalListDataFromJsonObject(const QJsonObject& jsonObject)
{
    if (jsonObject.empty())
    {
        return false;
    }

    if (!jsonObject.contains(PLAYER_UID_KEY) || !jsonObject.contains(PAL_LIST_KEY))
    {
        qDebug() << "PlayerPalData not contains player_uid or pal_list";

        return false;
    }

    if (jsonObject[PLAYER_UID_KEY] != m_playerUID)
    {
        return false;
    }

    for (auto pal : m_palList)
    {
        delete pal;
    }
    m_palList.clear();

    QJsonArray palList = jsonObject[PAL_LIST_KEY].toArray();
    for (auto palDataJsonValue : palList)
    {
        QJsonObject palDataJsonObject  = palDataJsonValue.toObject();
        PlayerPalModel* playerPalModel = new PlayerPalModel();
        if (playerPalModel->loadPalModel(palDataJsonObject))
        {
            m_palList.append(playerPalModel);
        }
        else
        {
            delete playerPalModel;
        }
    }

    m_palDataLoaded = true;

    return true;
}

QDebug operator<<(QDebug debug, const PlayerModel& data)
{
    debug.nospace() << "PlayerModel(" << data.m_playerNickName << ", " << data.m_playerUID << ")";
    return debug;
}
