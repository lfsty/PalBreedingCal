#pragma once

#include <QFrame>
#include <QVector>

class BreedingModel;
class BreedingView;
class QFrame;

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

    // widget 池：复用而非重建
    QVector<BreedingView*> m_itemPool;
    QVector<QFrame*> m_separatorPool;
};
