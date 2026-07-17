#include "playermanagerview.h"
#include "ui_playermanagerview.h"

#include <QFile>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
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
                              updatePlayerData(jsonDoc);
                          }

                          QTimer::singleShot(0, this, &PlayerManagerView::updatePlayerList);
                      });
}

void PlayerManagerView::onClickLoadFromNetWork()
{
    QString url = ui->lineEdit_networkAddress->text();
    if (url.isEmpty())
    {
        return;
    }

    QtConcurrent::run([this, url]()
                      {
                          constexpr const char* api = "/api/player";

                          QNetworkAccessManager manager;

                          {
                              // 加载玩家列表
                              QNetworkRequest playerListRequest(QString("http://%1%2").arg(url).arg(api));
                              QNetworkReply* playerListreply = manager.get(playerListRequest);
                              QEventLoop loop;
                              QObject::connect(playerListreply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
                              loop.exec();

                              // 处理响应结果
                              if (playerListreply->error() == QNetworkReply::NoError)
                              {
                                  QByteArray responseData = playerListreply->readAll();
                                  QJsonDocument doc       = QJsonDocument::fromJson(responseData);
                                  updatePlayerData(doc);
                                  playerListreply->deleteLater();
                              }
                              else
                              {
                                  QString errorString = playerListreply->errorString();
                                  QMetaObject::invokeMethod(qApp, [errorString]()
                                                            {
                                                                QMessageBox::critical(nullptr, "请求失败", QStringLiteral("获取玩家列表失败: %1").arg(errorString));
                                                            });
                                  playerListreply->deleteLater();
                                  return;
                              }
                          }

                          // 获取玩家数据
                          for (auto player : PlayerManager::getInstance()->getPlayerMap().values())
                          {
                              QNetworkRequest playerDataRequest(QString("http://%1%2/%3").arg(url).arg(api).arg(player->getPlayerUID()));
                              QNetworkReply* playerDatareply = manager.get(playerDataRequest);
                              QEventLoop loop;
                              connect(playerDatareply, &QNetworkReply::finished, this, [this, playerDatareply, player, &loop]()
                                      {
                                          if (playerDatareply->error() == QNetworkReply::NoError)
                                          {
                                              QByteArray responseData = playerDatareply->readAll();
                                              QJsonDocument doc       = QJsonDocument::fromJson(responseData);
                                              updatePlayerData(doc);
                                          }
                                          else
                                          {
                                              QString errorString = playerDatareply->errorString();
                                              QMetaObject::invokeMethod(qApp, [errorString]()
                                                                        {
                                                                            QMessageBox::critical(nullptr, "请求失败", QStringLiteral("获取玩家信息失败: %1").arg(errorString));
                                                                        });
                                          }
                                          loop.quit();

                                          playerDatareply->deleteLater();
                                      });
                              loop.exec();
                          }

                          QTimer::singleShot(0, this, &PlayerManagerView::updatePlayerList);
                      });
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

void PlayerManagerView::updatePlayerData(const QJsonDocument& doc)
{
    if (doc.isArray())
    {
        PlayerManager::getInstance()->LoadPlayerData(doc.array());
    }
    else if (doc.isObject())
    {
        PlayerManager::getInstance()->LoadPlayerData(doc.object());
    }
    else
    {
        qDebug() << "Invalid JSON format";
    }
}
