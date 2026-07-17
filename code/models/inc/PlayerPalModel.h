#pragma once

#include <PalDef.h>
#include <PalModel.h>

class PlayerPalModel : public PalModel
{
public:
    PlayerPalModel();
    ~PlayerPalModel() override;

public:
    bool loadPalModel(const QJsonObject& jsonObject) override;
    PlayerPalModel& operator=(const PalModel& palModel);

    Gender getGender() const { return m_gender; }

protected:
    Gender m_gender = NOTSET;
    bool m_isBoss   = false;

public:
    friend QDebug operator<<(QDebug debug, const PlayerPalModel& data);
};