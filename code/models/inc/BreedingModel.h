#pragma once

class DisplayPalModel;
class QDebug;

class BreedingModel
{
public:
    BreedingModel()  = default;
    ~BreedingModel() = default;

public:
    DisplayPalModel* parent1 = nullptr;
    DisplayPalModel* parent2 = nullptr;
    DisplayPalModel* child   = nullptr;

    BreedingModel copy(bool swapParent) const;

public:
    friend QDebug operator<<(QDebug debug, const BreedingModel& data);
};