#pragma once

#include <BreedingModel.h>
#include <QWidget>

class PalModel;

class BreedingView : public QWidget
{
    Q_OBJECT

public:
    explicit BreedingView(const BreedingModel& model, QWidget* parent = nullptr);
    void setModel(const BreedingModel& model) { m_model = model; }

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    void paintSinglePal(QPainter* painter, const QRect& rect, const PalModel* pal) const;
    void paintPlus(QPainter* painter, const QRect& rect) const;

private:
    BreedingModel m_model;
};
