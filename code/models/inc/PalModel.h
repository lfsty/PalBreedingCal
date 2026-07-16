#pragma once

#include <QJsonObject>
#include <QString>

class PalModel
{
public:
    PalModel();
    ~PalModel();

public:
    virtual bool loadPalModel(const QJsonObject& jsonObject);

    const QString& getInternalName() const { return m_internalName; }
    const QString& getLocalizedName() const { return m_localizedName; }
    const bool isOwned() const { return m_owned; }
    void setOwned(bool owned) { m_owned = owned; }

protected:
    QString m_internalName;
    QString m_localizedName;
    bool m_owned = false;
};