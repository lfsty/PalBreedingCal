#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCompleter>
#include <QStringListModel>

#include <PalManager.h>
#include <PalModel.h>

#include <QDebug>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent),
                                          ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    bool result = PalManager::getInstance()->loadDB(QStringLiteral(":/db/db.json"), QStringLiteral(":/db/breeding.json"));

    if (!result)
    {
        qDebug() << "Failed to load DB";
    }

    QStringList m_palNames = { "" };
    m_palNames.append(PalManager::getInstance()->getPalLocalNameList());

    m_parent1Completer = new QCompleter(m_palNames, this);
    m_parent2Completer = new QCompleter(m_palNames, this);
    m_childCompleter   = new QCompleter(m_palNames, this);
    m_parent1Completer->setFilterMode(Qt::MatchContains);
    m_parent2Completer->setFilterMode(Qt::MatchContains);
    m_childCompleter->setFilterMode(Qt::MatchContains);
    ui->parent1Combo->addItems(m_palNames);
    ui->parent2Combo->addItems(m_palNames);
    ui->childCombo->addItems(m_palNames);

    ui->parent1Combo->setCurrentIndex(0);
    ui->parent2Combo->setCurrentIndex(0);
    ui->childCombo->setCurrentIndex(0);
    ui->parent1Combo->setCompleter(m_parent1Completer);
    ui->parent2Combo->setCompleter(m_parent2Completer);
    ui->childCombo->setCompleter(m_childCompleter);

    m_breedingModel = new QStandardItemModel(this);
    ui->breedingTable->setModel(m_breedingModel);
    ui->breedingTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->breedingTable->horizontalHeader()->setSectionsMovable(false);
    ui->breedingTable->verticalHeader()->hide();

    updateComboAndList();

    connect(ui->parent1Combo, &QComboBox::currentTextChanged, this, &MainWindow::updateComboAndList);
    connect(ui->parent2Combo, &QComboBox::currentTextChanged, this, &MainWindow::updateComboAndList);
    connect(ui->childCombo, &QComboBox::currentTextChanged, this, &MainWindow::updateComboAndList);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateComboAndList()
{
    QString parent1Name = ui->parent1Combo->currentText();
    QString parent2Name = ui->parent2Combo->currentText();
    QString childName   = ui->childCombo->currentText();

    const QSet<BreedingData*>& allBreedingSet = PalManager::getInstance()->getBreedingList();

    QVector<BreedingData> breedingList;
    if ((!parent1Name.isEmpty() && parent2Name.isEmpty() && childName.isEmpty()) ||
        (parent1Name.isEmpty() && !parent2Name.isEmpty() && childName.isEmpty()) ||
        (parent1Name.isEmpty() && parent2Name.isEmpty() && !childName.isEmpty()))
    {
        // 有且仅有一个为不空的情况下
        if (!parent1Name.isEmpty())
        {
            QSet<BreedingData*> breedingSet = PalManager::getBreedingListByParentOneName(allBreedingSet, parent1Name);

            for (auto breedData : breedingSet)
            {
                BreedingData _tmpBreedData;
                if (breedData->parent1->getLocalizedName() == parent1Name)
                {
                    _tmpBreedData.parent1 = breedData->parent1;
                    _tmpBreedData.parent2 = breedData->parent2;
                }
                else
                {
                    _tmpBreedData.parent1 = breedData->parent2;
                    _tmpBreedData.parent2 = breedData->parent1;
                }
                _tmpBreedData.child = breedData->child;
                breedingList.append(_tmpBreedData);
            }
        }
        else if (!parent2Name.isEmpty())
        {
            QSet<BreedingData*> breedingSet = PalManager::getBreedingListByParentOneName(allBreedingSet, parent2Name);

            for (auto breedData : breedingSet)
            {
                BreedingData _tmpBreedData;
                if (breedData->parent2->getLocalizedName() == parent2Name)
                {
                    _tmpBreedData.parent1 = breedData->parent1;
                    _tmpBreedData.parent2 = breedData->parent2;
                }
                else
                {
                    _tmpBreedData.parent1 = breedData->parent2;
                    _tmpBreedData.parent2 = breedData->parent1;
                }
                _tmpBreedData.child = breedData->child;
                breedingList.append(_tmpBreedData);
            }
        }
        else if (!childName.isEmpty())
        {
            // 孩子不空的情况下
            QSet<BreedingData*> breedingSet = PalManager::getBreedingListByChildName(allBreedingSet, childName);
            for (auto breedData : breedingSet)
            {
                BreedingData _tmpBreedData;
                _tmpBreedData.parent1 = breedData->parent1;
                _tmpBreedData.parent2 = breedData->parent2;
                _tmpBreedData.child   = breedData->child;
                breedingList.append(_tmpBreedData);
            }
        }
    }
    else if (!parent1Name.isEmpty() && !parent2Name.isEmpty() && childName.isEmpty())
    {
        // 父母都不为空，孩子为空
        QSet<BreedingData*> breedingSet = PalManager::getBreedingListByParentOneName(allBreedingSet, parent1Name);
        breedingSet                     = PalManager::getBreedingListByParentOneName(breedingSet, parent2Name);
        for (auto breedData : breedingSet)
        {
            BreedingData _tmpBreedData;
            if (breedData->parent1->getLocalizedName() == parent1Name)
            {
                _tmpBreedData.parent1 = breedData->parent1;
                _tmpBreedData.parent2 = breedData->parent2;
            }
            else
            {
                _tmpBreedData.parent1 = breedData->parent2;
                _tmpBreedData.parent2 = breedData->parent1;
            }
            _tmpBreedData.child = breedData->child;
            breedingList.append(_tmpBreedData);
        }
    }
    else if ((!parent1Name.isEmpty() && parent2Name.isEmpty() && !childName.isEmpty()) ||
             (parent1Name.isEmpty() && !parent2Name.isEmpty() && !childName.isEmpty()))
    {
        // 孩子不为空，父母有一个为空
        QSet<BreedingData*> breedingSet = PalManager::getBreedingListByChildName(allBreedingSet, childName);
        if (!parent1Name.isEmpty())
        {
            breedingSet = PalManager::getBreedingListByParentOneName(breedingSet, parent1Name);
            for (auto breedData : breedingSet)
            {
                BreedingData _tmpBreedData;
                if (breedData->parent1->getLocalizedName() == parent1Name)
                {
                    _tmpBreedData.parent1 = breedData->parent1;
                    _tmpBreedData.parent2 = breedData->parent2;
                }
                else
                {
                    _tmpBreedData.parent1 = breedData->parent2;
                    _tmpBreedData.parent2 = breedData->parent1;
                }
                _tmpBreedData.child = breedData->child;
                breedingList.append(_tmpBreedData);
            }
        }
        else if (!parent2Name.isEmpty())
        {
            breedingSet = PalManager::getBreedingListByParentOneName(breedingSet, parent2Name);
            for (auto breedData : breedingSet)
            {
                BreedingData _tmpBreedData;
                if (breedData->parent2->getLocalizedName() == parent2Name)
                {
                    _tmpBreedData.parent1 = breedData->parent1;
                    _tmpBreedData.parent2 = breedData->parent2;
                }
                else
                {
                    _tmpBreedData.parent1 = breedData->parent2;
                    _tmpBreedData.parent2 = breedData->parent1;
                }
                _tmpBreedData.child = breedData->child;
                breedingList.append(_tmpBreedData);
            }
        }
    }
    else if (!parent1Name.isEmpty() && !parent2Name.isEmpty() && !childName.isEmpty())
    {
        // 三个都不为空
        QSet<BreedingData*> breedingSet = PalManager::getBreedingListByParentOneName(allBreedingSet, parent1Name);
        breedingSet                     = PalManager::getBreedingListByParentOneName(breedingSet, parent2Name);
        breedingSet                     = PalManager::getBreedingListByChildName(breedingSet, childName);
        for (auto breedData : breedingSet)
        {
            BreedingData _tmpBreedData;
            if (breedData->parent1->getLocalizedName() == parent1Name)
            {
                _tmpBreedData.parent1 = breedData->parent1;
                _tmpBreedData.parent2 = breedData->parent2;
            }
            else
            {
                _tmpBreedData.parent1 = breedData->parent2;
                _tmpBreedData.parent2 = breedData->parent1;
            }
            _tmpBreedData.child = breedData->child;
            breedingList.append(_tmpBreedData);
        }
    }

    m_breedingModel->clear();
    m_breedingModel->setRowCount(breedingList.size());
    m_breedingModel->setColumnCount(3);
    m_breedingModel->setHorizontalHeaderLabels(QStringList() << "parent1" << "parent2" << "child");
    for (int i = 0; i < breedingList.size(); i++)
    {
        m_breedingModel->setItem(i, 0, new QStandardItem(breedingList[i].parent1->getLocalizedName()));
        m_breedingModel->setItem(i, 1, new QStandardItem(breedingList[i].parent2->getLocalizedName()));
        m_breedingModel->setItem(i, 2, new QStandardItem(breedingList[i].child->getLocalizedName()));
    }
}
