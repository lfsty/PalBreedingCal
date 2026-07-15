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
    QSet<BreedingModel*> getBreedingListByFilter(const QString& parent1Name, const QString& parent2Name, const QString& childName) const;

private:
    void buildIndexes();

    QHash<QString, PalModel*> m_palMap;
    QSet<BreedingModel*> m_breedingList;
    QStringList m_palLocalNameList;

    // 索引：名称 → 包含该名称的育种条目集合
    QHash<QString, QSet<BreedingModel*>> m_parentIndex;
    QHash<QString, QSet<BreedingModel*>> m_childIndex;
};