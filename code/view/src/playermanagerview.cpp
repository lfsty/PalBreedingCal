#include "playermanagerview.h"
#include "ui_playermanagerview.h"

#include <QFile>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QtConcurrent>

#include <PlayerManager.h>
#include <PlayerModel.h>
#include <PlayerView.h>

#include <QDebug>

namespace
{
QString g_address = "127.0.0.1:8080";
}

PlayerManagerView::PlayerManagerView(QWidget* parent) : QDialog(parent),
                                                        ui(new Ui::PlayerManagerView)
{
    ui->setupUi(this);

    connect(ui->pushButton_loadFromFile, &QPushButton::clicked, this, &PlayerManagerView::onClickLoadFromFile);
    connect(ui->pushButton_loadFromNetWork, &QPushButton::clicked, this, &PlayerManagerView::onClickLoadFromNetWork);
    ui->lineEdit_networkAddress->setText(g_address);

    updatePlayerList();
}

PlayerManagerView::~PlayerManagerView()
{
    delete ui;
}

void PlayerManagerView::onClickLoadFromFile()
{
    QStringList filePaths = QFileDialog::getOpenFileNames(this, "Open File", "", "Text Files (*.json)");

    QtConcurrent::run([this, filePaths]()
                      {
                          for (auto filePath : filePaths)
                          {
                              QFile file(filePath);
                              if (!file.open(QIODevice::ReadOnly))
                              {
                                  qDebug() << "Failed to open file: " << filePath;
                                  continue;
                              }

                              QJsonDocument jsonDoc = QJsonDocument::fromJson(file.readAll());
                              if (jsonDoc.isArray())
                              {
                                  PlayerManager::getInstance()->LoadPlayerData(jsonDoc.array());
                              }
                              else if (jsonDoc.isObject())
                              {
                                  PlayerManager::getInstance()->LoadPlayerData(jsonDoc.object());
                              }
                              else
                              {
                                  qDebug() << "Invalid JSON format";
                              }
                          }

                          QTimer::singleShot(0, this, &PlayerManagerView::updatePlayerList);
                      });
}

void PlayerManagerView::onClickLoadFromNetWork()
{
    qDebug() << "NOT SUPPORT YET";
}

void PlayerManagerView::updatePlayerList()
{
    if (PlayerManager::getInstance()->getPlayerMap().empty())
    {
        return;
    }

    while (QLayoutItem* child = ui->playerViewLayout->takeAt(0))
    {
        if (child->widget())
        {
            delete child->widget();
        }

        delete child;
    }

    for (auto& player : PlayerManager::getInstance()->getPlayerMap().values())
    {
        PlayerView* playerView = new PlayerView();
        playerView->setModel(player);

        ui->playerViewLayout->addWidget(playerView);
    }

    ui->playerViewLayout->addStretch();
}
