#include "BreedingView.h"

#include <QHBoxLayout>
#include <QLabel>

#include <BreedingModel.h>
#include <PalModel.h>

BreedingView::BreedingView(const BreedingModel& model, QWidget* parent) : QWidget(parent)
{
    QString dispString = +" + " + model.parent2->getLocalizedName() + " = " + model.child->getLocalizedName();

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    layout->addWidget(new QLabel(model.parent1->getLocalizedName(), this));
    layout->addWidget(new QLabel("+", this));
    layout->addWidget(new QLabel(model.parent2->getLocalizedName(), this));
    layout->addWidget(new QLabel("+", this));
    layout->addWidget(new QLabel(model.child->getLocalizedName(), this));

    // setStyleSheet("border: 1px solid black;");
}
