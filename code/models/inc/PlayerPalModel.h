#pragma once

#include <PalModel.h>

class PlayerPalModel : public PalModel
{
public:
    enum Gender
    {
        NOTSET,
        Male,
        Female
    };

public:
    PlayerPalModel();
    ~PlayerPalModel() override;

public:
    bool loadPalModel(const QJsonObject& jsonObject) override;
    PlayerPalModel& operator=(const PalModel& palModel);

protected:
    Gender m_gender = NOTSET;
    bool m_isBoss   = false;

public:
    friend QDebug operator<<(QDebug debug, const PlayerPalModel& data);
};