#include "BreedingModel.h"

#include <PalModel.h>

#include <QDebug>

BreedingModel BreedingModel::copy(bool swapParent) const
{
    {
        BreedingModel _tmpBreedData;
        if (swapParent)
        {
            _tmpBreedData.parent1 = parent2;
            _tmpBreedData.parent2 = parent1;
        }
        else
        {
            _tmpBreedData.parent1 = parent1;
            _tmpBreedData.parent2 = parent2;
        }
        _tmpBreedData.child = child;

        return _tmpBreedData;
    }
}

QDebug operator<<(QDebug debug, const BreedingModel& data)
{
    debug.nospace() << data.parent1->getLocalizedName() << " + " << data.parent2->getLocalizedName() << " = " << data.child->getLocalizedName();
    return debug;
}
