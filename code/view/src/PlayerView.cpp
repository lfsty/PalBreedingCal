#include "PlayerView.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

#include <PlayerManager.h>
#include <PlayerModel.h>

PlayerView::PlayerView(QWidget* parent) : QWidget(parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    setFixedHeight(30);

    setLayout(new QHBoxLayout(this));

    layout()->setContentsMargins(0, 0, 0, 0);
    layout()->setSpacing(0);

    m_label  = new QLabel(this);
    m_button = new QPushButton(this);
    m_button->setText("apply");
    layout()->addWidget(m_label);
    layout()->addWidget(m_button);

    connect(m_button, &QPushButton::clicked, this, &PlayerView::applyButtonClicked);
    connect(PlayerManager::getInstance(), &PlayerManager::CurrentPlayerChanged, this, &PlayerView::updateView);
}

PlayerView::~PlayerView()
{
}

void PlayerView::setModel(PlayerModel* model)
{
    m_model = model;

    m_label->setText(m_model->getPlayerNickName());

    updateView();

    update();
}

void PlayerView::applyButtonClicked()
{
    if (m_model)
    {
        m_model->apply();
    }
}

void PlayerView::updateView()
{
    if (m_model && m_model->isApply())
    {
        m_button->setEnabled(false);
    }
    else
    {
        m_button->setEnabled(true);
    }
}
