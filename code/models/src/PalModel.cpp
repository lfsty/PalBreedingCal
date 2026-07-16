#include "PalModel.h"

#include <QJsonObject>

#include <QDebug>

PalModel::PalModel()
{
}

PalModel::~PalModel()
{
}

bool PalModel::loadPalModel(const QJsonObject& jsonObject)
{
    if (!jsonObject.contains("InternalName") || !jsonObject.contains("LocalizedNames"))
    {
        return false;
    }

    m_internalName  = jsonObject["InternalName"].toString();
    m_localizedName = jsonObject["LocalizedNames"]["zh-Hans"].toString();

    return true;
}
