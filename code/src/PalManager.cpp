#include "PalManager.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include <PalModel.h>

#include <QDebug>

PalManager* PalManager::getInstance()
{
    static PalManager instance;
    return &instance;
}

PalManager::PalManager()
{
}

PalManager::~PalManager()
{
    for (auto& pal : m_palMap)
    {
        delete pal;
    }
    m_palMap.clear();

    for (auto m_breedingList : m_breedingList)
    {
        delete m_breedingList;
    }
    m_breedingList.clear();
}

bool PalManager::loadDB(const QString& palDBPath, const QString& breedingDbPath)
{
    {
        // 加载db.json
        QFile palDBFile(palDBPath);
        if (!palDBFile.open(QIODevice::ReadOnly))
        {
            return false;
        }

        QJsonDocument jsonDoc       = QJsonDocument::fromJson(palDBFile.readAll());
        QJsonObject palDBRootObject = jsonDoc.object();
        QJsonArray palArray         = palDBRootObject["Pals"].toArray();

        for (const QJsonValue& palValue : palArray)
        {
            PalModel* palModel = new PalModel();
            if (!palModel->loadPalModel(palValue.toObject()))
            {
                qDebug() << "load error: " << palValue;
                delete palModel;
            }
            else
            {
                m_palMap[palModel->getInternalName()] = palModel;
                m_palLocalNameList.append(palModel->getLocalizedName());
            }
        }

        if (m_palMap.empty())
        {
            return false;
        }
    }

    {
        // 加载breeding.json
        QFile breedingDBFile(breedingDbPath);
        if (!breedingDBFile.open(QIODevice::ReadOnly))
        {
            return false;
        }

        QJsonDocument jsonDoc       = QJsonDocument::fromJson(breedingDBFile.readAll());
        QJsonObject palDBRootObject = jsonDoc.object();
        QJsonArray breedingArray    = palDBRootObject["Breeding"].toArray();

        for (const QJsonValue& breedingValue : breedingArray)
        {
            const QJsonObject& breedingObject = breedingValue.toObject();

            QString parent1 = breedingObject["Parent1InternalName"].toString();
            QString parent2 = breedingObject["Parent2InternalName"].toString();
            QString child   = breedingObject["ChildInternalName"].toString();

            if (parent1.isEmpty() || parent2.isEmpty() || child.isEmpty())
            {
                qDebug() << "load breeding error: " << breedingObject;
                continue;
            }

            PalModel* parent1PalModel = m_palMap.value(parent1, nullptr);
            PalModel* parent2PalModel = m_palMap.value(parent2, nullptr);
            PalModel* childPalModel   = m_palMap.value(child, nullptr);

            if (!parent1PalModel || !parent2PalModel || !childPalModel)
            {
                qDebug() << "PalModel not loaded " << breedingObject;
                continue;
            }

            BreedingData* breeding = new BreedingData{ parent1PalModel, parent2PalModel, childPalModel };

            // qDebug() << parent1PalModel->getLocalizedName() + " + " + parent2PalModel->getLocalizedName() + " = " + childPalModel->getLocalizedName();

            m_breedingList.insert(breeding);
        }

        if (m_breedingList.empty())
        {
            return false;
        }
    }

    return true;
}

QSet<BreedingData*> PalManager::getBreedingListByParentOneName(const QString& parentName)
{
    return getBreedingListByParentOneName(m_breedingList, parentName);
}

QSet<BreedingData*> PalManager::getBreedingListByChildName(const QString& childName)
{
    return getBreedingListByChildName(m_breedingList, childName);
}

QSet<BreedingData*> PalManager::getBreedingListByParentOneName(const QSet<BreedingData*>& breedingList, const QString& parentName)
{
    if (parentName.isEmpty() || breedingList.empty())
    {
        return QSet<BreedingData*>();
    }
    else
    {
        QSet<BreedingData*> retData;

        for (auto breeding : breedingList)
        {
            if (breeding->parent1->getLocalizedName() == parentName || breeding->parent2->getLocalizedName() == parentName)
            {
                retData.insert(breeding);
            }
        }

        return retData;
    }
}

QSet<BreedingData*> PalManager::getBreedingListByChildName(const QSet<BreedingData*>& breedingList, const QString& chindName)
{
    if (chindName.isEmpty() || breedingList.empty())
    {
        return QSet<BreedingData*>();
    }
    else
    {
        QSet<BreedingData*> retData;

        for (auto breeding : breedingList)
        {
            if (breeding->child->getLocalizedName() == chindName)
            {
                retData.insert(breeding);
            }
        }

        return retData;
    }
}
