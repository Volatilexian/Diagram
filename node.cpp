#include <QSet>
#include <QPainter>
#include <QStyle>
#include <QGraphicsItem>
#include <QInputDialog>
#include <QGraphicsSceneMouseEvent>
#include <QApplication>
#include <QStyleOptionGraphicsItem>
#include <QFontMetricsF>

#include "node.h"
#include "link.h"

Node::Node()
{
    myTextColor = Qt::darkGreen;
    myOutlineColor = Qt::darkBlue;
    myBackgroundColor = Qt::white;

    setFlags(ItemIsSelectable | ItemIsMovable);
}

Node::~Node()
{
    foreach(Link *link, myLinks)
        delete link;
}

void Node::addLink(Link *link)
{
    myLinks.insert(link);
}

QColor Node::backgroundColor() const
{
    return myBackgroundColor;
}

QRectF Node::boundingRect() const
{
    const int Margin = 1;
    return outlineRect().adjusted(-Margin, -Margin, +Margin, +Margin);
}

QColor Node::outlineColor() const
{
    return myOutlineColor;
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    QPen pen(myOutlineColor);
    if(option->state & QStyle::State_Selected)
    {
        pen.setStyle(Qt::DotLine);
        pen.setWidth(2);
    }
    painter->setPen(pen);
    painter->setBrush(myBackgroundColor);

    QRectF rect = outlineRect();
    painter->drawRoundRect(rect, roundness(rect.width()), roundness(rect.height()));

    painter->setPen(myTextColor);
    painter->drawText(rect, Qt::AlignCenter, myText);
}

void Node::removeLink(Link *link)
{
    myLinks.remove(link);
}

void Node::setBackgroundColor(const QColor &color)
{
    myBackgroundColor = color;
    update();
}

void Node::setOutlineColor(const QColor &color)
{
    myOutlineColor = color;
    update();
}

void Node::setText(const QString &text)
{
    prepareGeometryChange();
    myText = text;
    update();
}

void Node::setTextColor(const QColor &color)
{
    myTextColor = color;
    update();
}

QPainterPath Node::shape() const
{
    QRectF rect = outlineRect();

    QPainterPath path;
    path.addRoundedRect(rect, roundness(rect.width()), roundness(rect.height()));

    return path;
}

QString Node::text() const
{
    return myText;
}

QColor Node::textColor() const
{
    return myTextColor;
}

QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if(change == ItemPositionHasChanged)
    {
        foreach(Link *link, myLinks)
            link->trackNodes();
    }
    return QGraphicsItem::itemChange(change, value);
}

void Node::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    QString text = QInputDialog::getText(event->widget(),
                                         tr("Edit Text"), tr("Enter new text"),
                                         QLineEdit::Normal, myText);
    if(!text.isEmpty())
        setText(text);
}

QRectF Node::outlineRect() const
{
    const int padding = 8;
    QFontMetricsF metrics = qApp->fontMetrics();
    QRectF rect = metrics.boundingRect(myText);
    rect.adjust(-padding, -padding, +padding, +padding);
    rect.translate(-rect.center());
    return rect;
}

int Node::roundness(double size) const
{
    const int diameter = 12;
    return 100 * diameter / int(size);
}
