#pragma once

#include <QWidget>

class BreedingModel;

class BreedingView : public QWidget
{
    Q_OBJECT

public:
    explicit BreedingView(const BreedingModel& model, QWidget* parent = nullptr);

signals:
};
