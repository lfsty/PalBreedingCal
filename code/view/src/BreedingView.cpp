#include "BreedingView.h"

#include <QHBoxLayout>
#include <QPainter>

#include <BreedingModel.h>
#include <DisplayPalModel.h>
#include <PalDef.h>

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

void BreedingView::paintSinglePal(QPainter* painter, const QRect& rect, const DisplayPalModel* pal) const
{
    if (!pal)
    {
        return;
    }

    painter->save();

    QRect textRect = painter->boundingRect(rect, Qt::AlignCenter, pal->getLocalizedName());
    painter->drawText(textRect, Qt::AlignCenter, pal->getLocalizedName());

    QRect ownedStatusRect = QRect(textRect.left(), rect.top(), 0, rect.height());

    if (pal->isOwned())
    {
        // 绘制性别符号
        Genders gender = pal->getOwnedGender();

        QFontMetrics fm  = painter->fontMetrics();
        QRect symbolRect = fm.boundingRect(QString::fromUtf8("\u2642"));
        ownedStatusRect.setWidth(symbolRect.width());

        if (gender & Gender::Female)
        {
            ownedStatusRect.translate(-ownedStatusRect.width(), 0);
            painter->setPen(QColor("#FF69B4"));
            painter->drawText(ownedStatusRect, Qt::AlignCenter, QString::fromUtf8("\u2640"));
        }

        if (gender & Gender::Male)
        {
            ownedStatusRect.translate(-ownedStatusRect.width(), 0);
            painter->setPen(QColor("#1E90FF"));
            painter->drawText(ownedStatusRect, Qt::AlignCenter, QString::fromUtf8("\u2642"));
        }
    }

    {
        // 绘制是否拥有
        constexpr int ownedRadius = 5;
        ownedStatusRect.setWidth(ownedRadius);
        ownedStatusRect.translate(-ownedStatusRect.width() * 2, 0);

        if (pal->isOwned())
        {
            painter->setBrush(Qt::green);
        }
        else
        {
            painter->setBrush(Qt::red);
        }
        painter->setPen(Qt::NoPen);
        painter->drawEllipse(ownedStatusRect.center(), ownedRadius, ownedRadius);
    }

    painter->restore();
}
