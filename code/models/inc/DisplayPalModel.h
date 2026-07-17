#pragma once

#include <PalModel.h>

class DisplayPalModel : public PalModel
{
public:
    DisplayPalModel();
    ~DisplayPalModel() override;

public:
    bool loadPalModel(const QJsonObject& jsonObject) override;
    bool isLocalName(const QString& localName) const { return getLocalizedName() == localName; }

    const bool isOwned() const { return m_owned; }
    void setOwned(bool owned) { m_owned = owned; }

private:
    bool m_owned = false;

public:
    friend QDebug operator<<(QDebug debug, const DisplayPalModel& data);
};