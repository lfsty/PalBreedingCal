#pragma once

#include <PalDef.h>
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

    const Genders getOwnedGender() const { return m_ownedGender; }
    void setOwnedGender(Genders gender) { m_ownedGender = gender; }

private:
    bool m_owned          = false;
    Genders m_ownedGender = Gender::NOTSET;

public:
    friend QDebug operator<<(QDebug debug, const DisplayPalModel& data);
};