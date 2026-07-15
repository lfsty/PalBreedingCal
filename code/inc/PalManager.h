#pragma once

#include <QHash>
#include <QSet>
#include <QString>

#include <BreedingModel.h>

class PalManager
{
public:
    static PalManager* getInstance();

private:
    PalManager();
    ~PalManager();

public:
    bool loadDB(const QString& palDBPath, const QString& breedingDbPath);
    const QSet<BreedingModel*>& getBreedingList() { return m_breedingList; }
    const QStringList& getPalLocalNameList() { return m_palLocalNameList; }

public:
    static QSet<BreedingModel*> getBreedingListByParentOneName(const QSet<BreedingModel*>& breedingList, const QString& parentName);
    static QSet<BreedingModel*> getBreedingListByChildName(const QSet<BreedingModel*>& breedingList, const QString& chindName);

private:
    QHash<QString, PalModel*> m_palMap;
    QSet<BreedingModel*> m_breedingList;
    QStringList m_palLocalNameList;
};