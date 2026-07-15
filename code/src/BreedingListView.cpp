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
}

void BreedingListView::updateBreedingView(const QVector<BreedingModel>& breedingList)
{
    // 清空布局
    QVBoxLayout* breedingLayout = qobject_cast<QVBoxLayout*>(m_contentWidget->layout());
    if (!breedingLayout->isEmpty())
    {
        while (QLayoutItem* child = breedingLayout->takeAt(0))
        {
            if (child->widget())
            {
                delete child->widget();  // 直接销毁控件，避免异步残留
            }

            delete child;  // 释放布局项占用的内存
        }
    }

    if (!breedingList.empty())
    {  // 添加分割线
        QFrame* line = new QFrame();
        line->setFrameShape(QFrame::HLine);    // 设置为水平线
        line->setFrameShadow(QFrame::Sunken);  // 设置为凹陷效果
        breedingLayout->addWidget(line);

        for (auto breedingData : breedingList)
        {
            breedingLayout->addWidget(new BreedingView(breedingData, m_contentWidget));

            // 添加分割线
            QFrame* line = new QFrame();
            line->setFrameShape(QFrame::HLine);    // 设置为水平线
            line->setFrameShadow(QFrame::Sunken);  // 设置为凹陷效果
            breedingLayout->addWidget(line);
        }
        breedingLayout->addStretch();
    }
}