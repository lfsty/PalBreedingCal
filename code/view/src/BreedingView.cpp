#include "BreedingView.h"

#include <QHBoxLayout>
#include <QPainter>

#include <BreedingModel.h>
#include <PalModel.h>

BreedingView::BreedingView(const BreedingModel& model, QWidget* parent) : QWidget(parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    setFixedHeight(30);

    if (model.parent1)
    {
        setModel(model);
    }
}

void BreedingView::setModel(const BreedingModel& model)
{
    m_model = model;
    update();
}

void BreedingView::paintEvent(QPaintEvent* event)
{
    if (!m_model.parent1 || !m_model.parent2 || !m_model.child)
    {
        return;
    }

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QPen pen;
    pen.setWidth(2);
    pen.setColor(Qt::black);
    painter.setPen(pen);

    int palContentHeight = rect().height();
    int plusWidth        = palContentHeight;
    int palContentWidth  = (rect().width() - 2 * plusWidth) / 3;

    // 绘制帕鲁信息
    QRect paintPalRect = QRect(0, 0, palContentWidth, palContentHeight);
    paintSinglePal(&painter, paintPalRect, m_model.parent1);
    paintSinglePal(&painter, paintPalRect.translated(palContentWidth + palContentHeight, 0), m_model.parent2);
    paintSinglePal(&painter, paintPalRect.translated((palContentWidth + palContentHeight) * 2, 0), m_model.child);

    // 绘制加号
    painter.setPen(Qt::black);
    QFont font = painter.font();
    font.setPointSize(20);
    painter.setFont(font);

    QRect paintPlusRect = QRect(palContentWidth, 0, plusWidth, palContentHeight);
    painter.drawText(paintPlusRect, Qt::AlignCenter, "+");
    painter.drawText(paintPlusRect.translated(palContentWidth + palContentHeight, 0), Qt::AlignCenter, "=");
}

void BreedingView::paintSinglePal(QPainter* painter, const QRect& rect, const PalModel* pal) const
{
    if (!pal)
    {
        return;
    }

    painter->save();

    QRect textRect = painter->boundingRect(rect, Qt::AlignCenter, pal->getLocalizedName());
    painter->drawText(textRect, Qt::AlignCenter, pal->getLocalizedName());

    constexpr int ownedRadius = 5;
    QRect ownedRect           = QRect(textRect.left() - 2 * ownedRadius, rect.top(), ownedRadius, rect.height());
    if (pal->isOwned())
    {
        painter->setBrush(Qt::green);
    }
    else
    {
        painter->setBrush(Qt::red);
    }
    painter->setPen(Qt::NoPen);
    painter->drawEllipse(ownedRect.center(), ownedRadius, ownedRadius);

    painter->restore();
}
