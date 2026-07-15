#include "BreedingListView.h"

#include <QLabel>
#include <QScrollArea>
#include <QVBoxLayout>

#include <BreedingModel.h>
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
                child->widget()->deleteLater();  // 彻底销毁控件，释放内存
            }

            delete child;  // 释放布局项占用的内存
        }
    }

    for (auto breedingData : breedingList)
    {
        QString dispString = breedingData.parent1->getLocalizedName() + " + " + breedingData.parent2->getLocalizedName() + " = " + breedingData.child->getLocalizedName();

        breedingLayout->addWidget(new QLabel(dispString));
    }
    breedingLayout->addStretch();
}