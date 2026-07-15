#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCompleter>
#include <QStandardItemModel>
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

    QCompleter* _parent1Completer = new QCompleter(m_palNames, this);
    QCompleter* _parent2Completer = new QCompleter(m_palNames, this);
    QCompleter* _childCompleter   = new QCompleter(m_palNames, this);
    _parent1Completer->setFilterMode(Qt::MatchContains);
    _parent2Completer->setFilterMode(Qt::MatchContains);
    _childCompleter->setFilterMode(Qt::MatchContains);
    ui->parent1Combo->setCompleter(_parent1Completer);
    ui->parent2Combo->setCompleter(_parent2Completer);
    ui->childCombo->setCompleter(_childCompleter);

    ui->parent1Combo->addItems(m_palNames);
    ui->parent2Combo->addItems(m_palNames);
    ui->childCombo->addItems(m_palNames);
    ui->parent1Combo->setCurrentIndex(0);
    ui->parent2Combo->setCurrentIndex(0);
    ui->childCombo->setCurrentIndex(0);

    ui->breedingTable->setModel(new QStandardItemModel(this));
    ui->breedingTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->breedingTable->horizontalHeader()->setSectionsMovable(false);
    ui->breedingTable->verticalHeader()->hide();

    updateBreedingList();

    connect(ui->parent1Combo, &QComboBox::currentTextChanged, this, &MainWindow::updateBreedingList);
    connect(ui->parent2Combo, &QComboBox::currentTextChanged, this, &MainWindow::updateBreedingList);
    connect(ui->childCombo, &QComboBox::currentTextChanged, this, &MainWindow::updateBreedingList);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateBreedingList()
{
    QString parent1Name = ui->parent1Combo->currentText();
    QString parent2Name = ui->parent2Combo->currentText();
    QString childName   = ui->childCombo->currentText();

    const bool hasParent1 = !parent1Name.isEmpty();
    const bool hasParent2 = !parent2Name.isEmpty();
    const bool hasChild   = !childName.isEmpty();

    QVector<BreedingData> breedingList;

    if (hasParent1 || hasParent2 || hasChild)
    {
        // 逐步叠加过滤条件：从全集出发，依次按 parent1 → parent2 → child 过滤
        QSet<BreedingData*> breedingSet = PalManager::getInstance()->getBreedingList();
        if (hasParent1)
        {
            breedingSet = PalManager::getBreedingListByParentOneName(breedingSet, parent1Name);
        }

        if (hasParent2)
        {
            breedingSet = PalManager::getBreedingListByParentOneName(breedingSet, parent2Name);
        }

        if (hasChild)
        {
            breedingSet = PalManager::getBreedingListByChildName(breedingSet, childName);
        }

        // 遍历结果集，必要时通过 copy(swap) 确保"被搜索的父本"始终显示为 parent1

        for (auto breedData : breedingSet)
        {
            bool needSwap = false;
            if (hasParent1)
            {
                // 指定了 parent1：若数据中匹配到的是 parent2，则交换，使 parent1Name 出现在 parent1 位置
                needSwap = (breedData->parent2->getLocalizedName() == parent1Name);
            }
            else if (hasParent2)
            {
                // 仅指定了 parent2：若数据中匹配到的是 parent1，则交换，使 parent2Name 出现在 parent1 位置
                needSwap = (breedData->parent1->getLocalizedName() == parent2Name);
            }

            // hasParent1 和 hasParent2 均为 false（仅指定了 child）：无需交换
            breedingList.append(hasParent1 || hasParent2 ? breedData->copy(needSwap) : *breedData);
        }
    }

    updateBreedingView(breedingList);
}

void MainWindow::updateBreedingView(const QVector<BreedingData>& breedingList)
{
    QStandardItemModel* breedingDataModel = qobject_cast<QStandardItemModel*>(ui->breedingTable->model());
    if (breedingDataModel == nullptr)
    {
        return;
    }

    const QStringList headLabels = QStringList() << "parent1" << "parent2" << "child";
    breedingDataModel->clear();
    breedingDataModel->setRowCount(breedingList.size());
    breedingDataModel->setColumnCount(3);
    breedingDataModel->setHorizontalHeaderLabels(headLabels);

    if (!breedingList.empty())
    {
        for (int i = 0; i < breedingList.size(); i++)
        {
            breedingDataModel->setItem(i, 0, new QStandardItem(breedingList[i].parent1->getLocalizedName()));
            breedingDataModel->setItem(i, 1, new QStandardItem(breedingList[i].parent2->getLocalizedName()));
            breedingDataModel->setItem(i, 2, new QStandardItem(breedingList[i].child->getLocalizedName()));
        }
    }
}
