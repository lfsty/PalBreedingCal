#pragma once

#include <QHash>
#include <QSet>
#include <QString>
#include <QVector>

class PalModel;

struct BreedingData
{
    PalModel* parent1;
    PalModel* parent2;
    PalModel* child;
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
    QSet<BreedingData*> getBreedingListByParentOneName(const QString& parentName);
    QSet<BreedingData*> getBreedingListByChildName(const QString& childName);
    static QSet<BreedingData*> getBreedingListByParentOneName(const QSet<BreedingData*>& breedingList, const QString& parentName);
    static QSet<BreedingData*> getBreedingListByChildName(const QSet<BreedingData*>& breedingList, const QString& chindName);

private:
    QHash<QString, PalModel*> m_palMap;
    QSet<BreedingData*> m_breedingList;
    QStringList m_palLocalNameList;
};