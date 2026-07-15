#pragma once

#include <QFrame>

class BreedingModel;

class BreedingListView : public QFrame
{
    Q_OBJECT

public:
    explicit BreedingListView(QWidget* parent = nullptr);
    ~BreedingListView();

public slots:
    void updateBreedingView(const QVector<BreedingModel>& breedingList);

private:
    QWidget* m_contentWidget = nullptr;

signals:
};
