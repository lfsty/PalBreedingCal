#pragma once

#include <QJsonObject>
#include <QString>

class PalModel
{
public:
    PalModel();
    virtual ~PalModel();

public:
    virtual bool loadPalModel(const QJsonObject& jsonObject) = 0;

    const QString& getInternalName() const { return m_internalName; }
    const QString& getLocalizedName() const { return m_localizedName; }

protected:
    void setInternalName(const QString& internalName) { m_internalName = internalName; }
    void setLocalizedName(const QString& localizedName) { m_localizedName = localizedName; }

private:
    QString m_internalName;
    QString m_localizedName;

public:
    friend QDebug operator<<(QDebug debug, const PalModel& data);
};