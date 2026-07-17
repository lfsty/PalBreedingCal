#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCompleter>
#include <QFileDialog>
#include <QStandardItemModel>
#include <QStringListModel>
#include <QtConcurrent>

#include <BreedingListView.h>
#include <DisplayPalManager.h>
#include <DisplayPalModel.h>
#include <PlayerManager.h>
#include <PlayerModel.h>
#include <playermanagerview.h>

#include <QDebug>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent),
                                          ui(new Ui::MainWindow)
{
    qRegisterMetaType<QVector<BreedingModel>>("QVector<BreedingModel>");

    ui->setupUi(this);

    connect(DisplayPalManager::getInstance(), &DisplayPalManager::dataLoaded, this, &MainWindow::updateComboBox);
    connect(PlayerManager::getInstance(), &PlayerManager::CurrentPlayerChanged, DisplayPalManager::getInstance(), &DisplayPalManager::updataOwnedPal);
    connect(PlayerManager::getInstance(), &PlayerManager::CurrentPlayerChanged, this, [=](const PlayerModel* player)
            {
                if (player)
                {
                    setWindowTitle(QString("PalBreedingCal - %1").arg(player->getPlayerNickName()));
                }
            });

    connect(this, &MainWindow::requestUpdateBreedingView, ui->breedingViewFrame, &BreedingListView::updateBreedingView);

    connect(ui->parent1Combo, &QComboBox::currentTextChanged, this, &MainWindow::updateBreedingList);
    connect(ui->parent2Combo, &QComboBox::currentTextChanged, this, &MainWindow::updateBreedingList);
    connect(ui->childCombo, &QComboBox::currentTextChanged, this, &MainWindow::updateBreedingList);

    connect(ui->action_playerManager, &QAction::triggered, this, &MainWindow::showPlayerManagerDialog);

    DisplayPalManager::getInstance()->requestLoadDB(QStringLiteral(":/db/db.json"), QStringLiteral(":/db/breeding.json"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateComboBox()
{
    QSignalBlocker blocker(ui->parent1Combo);
    QSignalBlocker blocker2(ui->parent2Combo);
    QSignalBlocker blocker3(ui->childCombo);

    QStringList m_palNames = { "" };
    m_palNames.append(DisplayPalManager::getInstance()->getPalLocalNameList());

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

    updateBreedingList();
}

void MainWindow::showPlayerManagerDialog()
{
    PlayerManagerView dlg(this);
    dlg.exec();
}

void MainWindow::updateBreedingList()
{
    QString parent1Name = ui->parent1Combo->currentText();
    QString parent2Name = ui->parent2Combo->currentText();
    QString childName   = ui->childCombo->currentText();

    const bool hasParent1 = !parent1Name.isEmpty();
    const bool hasParent2 = !parent2Name.isEmpty();
    const bool hasChild   = !childName.isEmpty();

    QtConcurrent::run([=]()
                      {
                          QVector<BreedingModel> breedingList;

                          if (hasParent1 || hasParent2 || hasChild)
                          {
                              // 使用索引一次性完成过滤
                              QSet<BreedingModel*> breedingSet = DisplayPalManager::getInstance()->getBreedingListByFilter(parent1Name, parent2Name, childName);

                              breedingList.reserve(breedingSet.size());

                              for (auto breedData : breedingSet)
                              {
                                  bool needSwap = false;
                                  if (hasParent1)
                                  {
                                      needSwap = breedData->parent2->isLocalName(parent1Name);
                                  }
                                  else if (hasParent2)
                                  {
                                      needSwap = breedData->parent1->isLocalName(parent2Name);
                                  }

                                  breedingList.append(hasParent1 || hasParent2 ? breedData->copy(needSwap) : *breedData);
                              }
                          }

                          emit requestUpdateBreedingView(breedingList);
                      });
}
