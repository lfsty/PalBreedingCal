#include "DisplayPalModel.h"

#include <QDebug>

namespace
{
constexpr const char* INTERNAL_NAME_KEY   = "InternalName";
constexpr const char* LOCALIZED_NAMES_KEY = "LocalizedNames";
constexpr const char* LOCAL_LANGUAGE_KEY  = "zh-Hans";
}  // namespace

DisplayPalModel::DisplayPalModel()
{
}

DisplayPalModel::~DisplayPalModel()
{
}

bool DisplayPalModel::loadPalModel(const QJsonObject& jsonObject)
{
    if (!jsonObject.contains(INTERNAL_NAME_KEY) || !jsonObject.contains(LOCALIZED_NAMES_KEY))
    {
        return false;
    }

    setInternalName(jsonObject[INTERNAL_NAME_KEY].toString());
    setLocalizedName(jsonObject[LOCALIZED_NAMES_KEY][LOCAL_LANGUAGE_KEY].toString());

    return true;
}

QDebug operator<<(QDebug debug, const DisplayPalModel& data)
{
    debug.nospace() << QStringLiteral("DisplayPalModel( InternalName: %1, LocalizedName: %2, Owned: %3 )").arg(data.getInternalName()).arg(data.getLocalizedName()).arg(data.m_owned);

    return debug;
}
