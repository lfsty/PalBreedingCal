#pragma once

#include <BreedingModel.h>
#include <QWidget>

class DisplayPalModel;

class BreedingView : public QWidget
{
    Q_OBJECT

public:
    explicit BreedingView(const BreedingModel& model, QWidget* parent = nullptr);
    void setModel(const BreedingModel& model);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    void paintSinglePal(QPainter* painter, const QRect& rect, const DisplayPalModel* pal) const;

private:
    BreedingModel m_model;
};
