#pragma once

#include <QHash>
#include <QSet>
#include <QString>

class PalModel;

struct BreedingData
{
    PalModel* parent1;
    PalModel* parent2;
    PalModel* child;

    BreedingData copy(bool swapParent) const
    {
        BreedingData _tmpBreedData;
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
};

class PalManager
{
public:
    static PalManager* getInstance();

private:
    PalManager();
    ~PalManager();

public:
    bool loadDB(const QString& palDBPath, const QString& breedingDbPath);
    const QSet<BreedingData*>& getBreedingList() { return m_breedingList; }
    const QStringList& getPalLocalNameList() { return m_palLocalNameList; }

public:
    static QSet<BreedingData*> getBreedingListByParentOneName(const QSet<BreedingData*>& breedingList, const QString& parentName);
    static QSet<BreedingData*> getBreedingListByChildName(const QSet<BreedingData*>& breedingList, const QString& chindName);

private:
    QHash<QString, PalModel*> m_palMap;
    QSet<BreedingData*> m_breedingList;
    QStringList m_palLocalNameList;
};