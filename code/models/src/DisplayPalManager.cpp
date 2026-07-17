#include "DisplayPalManager.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QtConcurrent>

#include <DisplayPalModel.h>
#include <PlayerModel.h>

#include <QDebug>

DisplayPalManager* DisplayPalManager::getInstance()
{
    static DisplayPalManager instance;
    return &instance;
}

DisplayPalManager::DisplayPalManager()
{
}

DisplayPalManager::~DisplayPalManager()
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

void DisplayPalManager::requestLoadDB(const QString& palDBPath, const QString& breedingDbPath)
{
    QtConcurrent::run([=]()
                      {
                          bool ret = loadPalDB(palDBPath);
                          if (!ret)
                          {
                              qDebug() << "load PalDB failed: " << palDBPath;
                              return;
                          }

                          ret = loadBreedingDB(breedingDbPath);
                          if (!ret)
                          {
                              qDebug() << "load BreedingDB failed: " << breedingDbPath;
                              return;
                          }

                          // 构建索引以加速查询
                          buildIndexes();

                          emit dataLoaded();
                      });
}

const DisplayPalModel* DisplayPalManager::getDisplayPalModel(const QString& internalPalName) const
{
    if (!m_palMap.contains(internalPalName))
    {
        return nullptr;
    }
    return m_palMap[internalPalName];
}

void DisplayPalManager::updataOwnedPal(const PlayerModel* playerModel)
{
    if (!playerModel)
    {
        return;
    }

    for (auto iter = m_palMap.begin(); iter != m_palMap.end(); iter++)
    {
        const QString& internalPalName = iter.key();
        iter.value()->setOwned(playerModel->isOwnPalByInterName(internalPalName));
        iter.value()->setOwnedGender(playerModel->getOwnedPalGendersByInterName(internalPalName));
    }
}

bool DisplayPalManager::loadPalDB(const QString& palDBPath)
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
        DisplayPalModel* palModel = new DisplayPalModel();
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
    else
    {
        return true;
    }
}

bool DisplayPalManager::loadBreedingDB(const QString& breedingDbPath)
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

        DisplayPalModel* parent1PalModel = m_palMap.value(parent1, nullptr);
        DisplayPalModel* parent2PalModel = m_palMap.value(parent2, nullptr);
        DisplayPalModel* childPalModel   = m_palMap.value(child, nullptr);

        if (!parent1PalModel || !parent2PalModel || !childPalModel)
        {
            qDebug() << "DisplayPalModel not loaded " << breedingObject;
            continue;
        }

        BreedingModel* breeding = new BreedingModel{ parent1PalModel, parent2PalModel, childPalModel };

        m_breedingList.insert(breeding);
    }

    if (m_breedingList.empty())
    {
        return false;
    }
    else
    {
        return true;
    }
}

void DisplayPalManager::buildIndexes()
{
    m_parentIndex.clear();
    m_childIndex.clear();

    for (BreedingModel* breeding : m_breedingList)
    {
        const QString& p1Name = breeding->parent1->getLocalizedName();
        const QString& p2Name = breeding->parent2->getLocalizedName();
        const QString& cName  = breeding->child->getLocalizedName();

        m_parentIndex[p1Name].insert(breeding);
        m_parentIndex[p2Name].insert(breeding);
        m_childIndex[cName].insert(breeding);
    }
}

QSet<BreedingModel*> DisplayPalManager::getBreedingListByFilter(const QString& parent1Name, const QString& parent2Name, const QString& childName) const
{
    const bool hasParent1 = !parent1Name.isEmpty();
    const bool hasParent2 = !parent2Name.isEmpty();
    const bool hasChild   = !childName.isEmpty();

    if (!hasParent1 && !hasParent2 && !hasChild)
    {
        return {};
    }

    QSet<BreedingModel*> result;

    if (hasParent1 && hasParent2 && parent1Name == parent2Name)
    {
        // 两个父本相同：要求 breeding 的两个父本都匹配
        const QSet<BreedingModel*>& set1 = m_parentIndex.value(parent1Name);
        QSet<BreedingModel*> filtered;
        for (BreedingModel* b : set1)
        {
            if (b->parent1->getLocalizedName() == parent1Name && b->parent2->getLocalizedName() == parent1Name)
            {
                filtered.insert(b);
            }
        }
        result = filtered;
    }
    else
    {
        if (hasParent1)
        {
            result = m_parentIndex.value(parent1Name);
        }

        if (hasParent2)
        {
            if (result.empty())
            {
                result = m_parentIndex.value(parent2Name);
            }
            else
            {
                result.intersect(m_parentIndex.value(parent2Name));
            }
        }
    }

    if (hasChild)
    {
        if (result.empty())
        {
            result = m_childIndex.value(childName);
        }
        else
        {
            result.intersect(m_childIndex.value(childName));
        }
    }

    return result;
}
