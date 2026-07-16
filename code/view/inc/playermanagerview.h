#ifndef PLAYERMANAGERVIEW_H
#define PLAYERMANAGERVIEW_H

#include <QDialog>
#include <QJsonDocument>

namespace Ui
{
class PlayerManagerView;
}

class PlayerManagerView : public QDialog
{
    Q_OBJECT

public:
    explicit PlayerManagerView(QWidget* parent = nullptr);
    ~PlayerManagerView();

protected slots:
    void onClickLoadFromFile();
    void onClickLoadFromNetWork();
    void updatePlayerList();

private:
    void updatePlayerData(const QJsonDocument& doc);

private:
    Ui::PlayerManagerView* ui;
};

#endif  // PLAYERMANAGERVIEW_H
