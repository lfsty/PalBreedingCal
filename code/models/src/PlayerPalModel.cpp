#include "PlayerPalModel.h"

#include <QJsonObject>

#include <PalManager.h>

#include <QDebug>

namespace
{
constexpr const char* PAL_TYPE_KEY   = "type";
constexpr const char* PAL_GENDER_KEY = "gender";
}  // namespace

PlayerPalModel::PlayerPalModel()
{
}

PlayerPalModel::~PlayerPalModel()
{
}

bool PlayerPalModel::loadPalModel(const QJsonObject& jsonObject)
{
    if (jsonObject.empty())
    {
        return false;
    }

    if (!jsonObject.contains(PAL_TYPE_KEY) || !jsonObject.contains(PAL_GENDER_KEY))
    {
        return false;
    }

    QString gender = jsonObject[PAL_GENDER_KEY].toString();
    if (gender == "Male")
    {
        m_gender = Gender::Male;
    }
    else if (gender == "Female")
    {
        m_gender = Gender::Female;
    }

    QString type = jsonObject[PAL_TYPE_KEY].toString();
    QString internamPalName;
    if (type.contains(QStringLiteral("BOSS_")) || type.contains(QStringLiteral("Boss_")))
    {
        m_isBoss        = true;
        internamPalName = type.mid(5);
    }
    else
    {
        m_isBoss        = false;
        internamPalName = type;
    }

    const PalModel* palModel = PalManager::getInstance()->getPalModel(internamPalName);
    if (!palModel)
    {
        qDebug() << "PalModel not found: " << internamPalName;
        return false;
    }

    *this = *palModel;

    return true;
}

PlayerPalModel& PlayerPalModel::operator=(const PalModel& palModel)
{
    this->m_internalName  = palModel.getInternalName();
    this->m_localizedName = palModel.getLocalizedName();

    return *this;
}

QDebug operator<<(QDebug debug, const PlayerPalModel& data)
{
    debug.nospace() << "PlayerPalModel(" << data.getLocalizedName() << ", ";
    switch (data.m_gender)
    {
    case PlayerPalModel::Gender::Male:
        debug.nospace() << "Male" << ", ";
        break;
    case PlayerPalModel::Gender::Female:
        debug.nospace() << "Female" << ", ";
        break;
    case PlayerPalModel::Gender::NOTSET:
        debug.nospace() << "Unknown" << ", ";
        break;
    }
    debug.nospace() << (data.m_isBoss ? "Boss" : "Normal");
    debug.nospace() << ")";
    return debug;
}
