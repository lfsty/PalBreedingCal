#include "PlayerModel.h"

#include <QJsonArray>

#include <PlayerManager.h>
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
    clearAllPalData();
}

bool PlayerModel::loadPlayerDataFromJsonObject(const QJsonObject& jsonObject)
{
    if (jsonObject.empty())
    {
        return false;
    }

    if (!jsonObject.contains(PLAYER_UID_KEY) || !jsonObject.contains(PLAYER_NICK_NAME_KEY))
    {
        qDebug() << "PlayerPalData donot contains acquired keys";
        return false;
    }

    if (!m_playerUID.isEmpty() && m_playerUID != jsonObject[PLAYER_UID_KEY].toString())
    {
        return false;
    }

    m_playerUID      = jsonObject[PLAYER_UID_KEY].toString();
    m_playerNickName = jsonObject[PLAYER_NICK_NAME_KEY].toString();

    if (jsonObject.contains(PAL_LIST_KEY))
    {
        // 如果有pals列表，直接读取
        clearAllPalData();

        QJsonArray palList = jsonObject[PAL_LIST_KEY].toArray();
        for (auto palDataJsonValue : palList)
        {
            QJsonObject palDataJsonObject  = palDataJsonValue.toObject();
            PlayerPalModel* playerPalModel = new PlayerPalModel();
            if (playerPalModel->loadPalModel(palDataJsonObject))
            {
                m_palList.append(playerPalModel);
                const QString& palInterName = playerPalModel->getInternalName();
                m_ownedPalInterNameSet.insert(palInterName);
                if (m_ownedPalGenderMap.contains(palInterName))
                {
                    m_ownedPalGenderMap[palInterName] |= playerPalModel->getGender();
                }
                else
                {
                    m_ownedPalGenderMap[palInterName] = playerPalModel->getGender();
                }
            }
            else
            {
                delete playerPalModel;
            }
        }

        m_palDataLoaded = true;
    }
    else
    {
        m_palDataLoaded = false;
    }

    return true;
}

const Genders PlayerModel::getOwnedPalGendersByInterName(const QString& palInterName) const
{
    if (m_ownedPalGenderMap.contains(palInterName))
    {
        return m_ownedPalGenderMap.value(palInterName);
    }
    else
    {
        return Gender::NOTSET;
    }
}

void PlayerModel::apply()
{
    PlayerManager::getInstance()->setCurrentPlayer(m_playerUID);
}

bool PlayerModel::isApply()
{
    return this == PlayerManager::getInstance()->getCurrentPlayer();
}

void PlayerModel::clearAllPalData()
{
    for (auto pal : m_palList)
    {
        delete pal;
    }
    m_palList.clear();
    m_ownedPalInterNameSet.clear();
    m_ownedPalGenderMap.clear();
}

QDebug operator<<(QDebug debug, const PlayerModel& data)
{
    debug.nospace() << "PlayerModel(" << data.m_playerNickName << ", " << data.m_playerUID << ")";
    return debug;
}
