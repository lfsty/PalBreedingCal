#pragma once

#include <QWidget>

class QLabel;
class QPushButton;

class PlayerModel;

class PlayerView : public QWidget
{
    Q_OBJECT

public:
    PlayerView(QWidget* parent = nullptr);
    ~PlayerView();

public:
    void setModel(PlayerModel* model);

protected:
    void applyButtonClicked();
    void updateView();

private:
    PlayerModel* m_model  = nullptr;
    QLabel* m_label       = nullptr;
    QPushButton* m_button = nullptr;
};