#pragma once

class PalModel;
class QDebug;

class BreedingModel
{
public:
    BreedingModel()  = default;
    ~BreedingModel() = default;

public:
    PalModel* parent1 = nullptr;
    PalModel* parent2 = nullptr;
    PalModel* child   = nullptr;

    BreedingModel copy(bool swapParent) const;

    friend QDebug operator<<(QDebug debug, const BreedingModel& data);
};