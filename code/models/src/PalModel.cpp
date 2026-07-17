#include "PalModel.h"

#include <QJsonObject>

#include <QDebug>

PalModel::PalModel()
{
}

PalModel::~PalModel()
{
}

QDebug operator<<(QDebug debug, const PalModel& data)
{
    debug.nospace() << QStringLiteral("PalModel( InternalName: %1, LocalizedName: %2 )").arg(data.m_internalName).arg(data.m_localizedName);
    return debug;
}
