#pragma once

#include <QWidget>

class BreedingModel;
class QLabel;

class BreedingView : public QWidget
{
    Q_OBJECT

public:
    explicit BreedingView(const BreedingModel& model, QWidget* parent = nullptr);
    void setModel(const BreedingModel& model);

signals:

private:
    QLabel* m_parent1Label = nullptr;
    QLabel* m_parent2Label = nullptr;
    QLabel* m_childLabel   = nullptr;
};
