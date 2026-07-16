#include "BreedingListView.h"

#include <QLabel>
#include <QScrollArea>
#include <QVBoxLayout>

#include <BreedingModel.h>
#include <BreedingView.h>
#include <PalModel.h>

#include <QDebug>

BreedingListView::BreedingListView(QWidget* parent) : QFrame(parent)
{
    // 父布局
    QVBoxLayout* frameLayout = new QVBoxLayout(this);
    frameLayout->setContentsMargins(0, 0, 0, 0);

    // 滚动区域
    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    frameLayout->addWidget(scrollArea);

    // 滚动区域内容
    m_contentWidget = new QWidget(scrollArea);
    scrollArea->setWidget(m_contentWidget);
    QVBoxLayout* breedingLayout = new QVBoxLayout(m_contentWidget);
    breedingLayout->setContentsMargins(0, 0, 0, 0);
    breedingLayout->setSpacing(10);
}

BreedingListView::~BreedingListView()
{
    // 清理 widget 池
    for (auto w : m_itemPool)
    {
        delete w;
    }
    for (auto s : m_separatorPool)
    {
        delete s;
    }
}

void BreedingListView::updateBreedingView(const QVector<BreedingModel>& breedingList)
{
    QVBoxLayout* breedingLayout = qobject_cast<QVBoxLayout*>(m_contentWidget->layout());
    const int newCount          = breedingList.size();

    // 1. 确保池中有足够的 BreedingView
    while (m_itemPool.size() < newCount)
    {
        // 用空模型创建，稍后 setModel 更新
        auto* view = new BreedingView(BreedingModel{}, m_contentWidget);
        m_itemPool.append(view);
    }

    // 2. 确保池中有足够的分隔线（每个条目一条，顶部一条）
    const int sepNeeded = newCount > 0 ? newCount + 1 : 0;
    while (m_separatorPool.size() < sepNeeded)
    {
        auto* line = new QFrame(m_contentWidget);
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        m_separatorPool.append(line);
    }

    // 3. 清空布局（不删除 widget，只从布局中移除）
    while (QLayoutItem* item = breedingLayout->takeAt(0))
    {
        delete item;  // 只删除 layout item，不删 widget
    }

    // 4. 按顺序添加需要的 widget 到布局
    if (newCount > 0)
    {
        // 顶部分割线
        breedingLayout->addWidget(m_separatorPool[0]);
        m_separatorPool[0]->show();

        for (int i = 0; i < newCount; ++i)
        {
            // 更新数据
            m_itemPool[i]->setModel(breedingList[i]);
            m_itemPool[i]->show();
            breedingLayout->addWidget(m_itemPool[i]);

            // 底部分割线
            breedingLayout->addWidget(m_separatorPool[i + 1]);
            m_separatorPool[i + 1]->show();
        }
    }

    // 5. 隐藏多余的 widget
    for (int i = newCount; i < m_itemPool.size(); ++i)
    {
        m_itemPool[i]->hide();
    }
    for (int i = sepNeeded; i < m_separatorPool.size(); ++i)
    {
        m_separatorPool[i]->hide();
    }

    // 6. 底部弹簧
    breedingLayout->addStretch();
}