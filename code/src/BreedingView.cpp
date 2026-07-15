#include "BreedingView.h"

#include <QHBoxLayout>
#include <QLabel>

#include <BreedingModel.h>
#include <PalModel.h>

BreedingView::BreedingView(const BreedingModel& model, QWidget* parent) : QWidget(parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    m_parent1Label = new QLabel(this);
    m_parent2Label = new QLabel(this);
    m_childLabel   = new QLabel(this);

    layout->addWidget(m_parent1Label);
    layout->addWidget(new QLabel("+", this));
    layout->addWidget(m_parent2Label);
    layout->addWidget(new QLabel("=", this));
    layout->addWidget(m_childLabel);

    if (model.parent1)
    {
        setModel(model);
    }
}

void BreedingView::setModel(const BreedingModel& model)
{
    if (model.parent1)
    {
        m_parent1Label->setText(model.parent1->getLocalizedName());
    }
    if (model.parent2)
    {
        m_parent2Label->setText(model.parent2->getLocalizedName());
    }
    if (model.child)
    {
        m_childLabel->setText(model.child->getLocalizedName());
    }
}
