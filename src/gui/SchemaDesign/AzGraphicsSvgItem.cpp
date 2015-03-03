#include "AzGraphicsSvgItem.h"

#include <QDebug>
#include <QSvgGenerator>
#include <QBuffer>
#include <QPainter>
#include <QSvgRenderer>
#include <QStyleOptionGraphicsItem>

#include "AzChupa.h"

AzGraphicsSvgItem::AzGraphicsSvgItem() {
}

AzGraphicsSvgItem::AzGraphicsSvgItem(QGraphicsItem *parent):QGraphicsSvgItem(parent) {
    mInTransformChange = false;

}

AzGraphicsSvgItem::AzGraphicsSvgItem(const QString &fileName, QGraphicsItem * parent):QGraphicsSvgItem(fileName,parent) {
    mInTransformChange = false;
}

AzGraphicsSvgItem::~AzGraphicsSvgItem() {

}

/*!
 * \brief AzGraphicsSvgItem::mapTo
 * \return данные, с учетом transform
 * Возвращает изображение, полученое после трансформации.
 */
QByteArray AzGraphicsSvgItem::mapTo()  {
    QBuffer buffer;
    QSvgGenerator generator;
    generator.setOutputDevice(&buffer);
    QPainter painter;
    const QStyleOptionGraphicsItem style;    //!!!not define style (selected border?)
    qInstallMessageHandler(AzChupa::emptyMessageOutput);
    painter.begin(&generator);
        painter.setTransform(transform());
        paint(&painter,&style);
    painter.end();
    qInstallMessageHandler(0);
    return buffer.data();
}

/*!
 * \brief AzGraphicsSvgItem::clone
 * \param parent
 * \return
 * clone item with transform
 */
AzGraphicsSvgItem* AzGraphicsSvgItem::clone(QGraphicsItem* parent) { //!!!!!! убрать
    QSvgRenderer* renderer = new QSvgRenderer(mapTo()); ///!!!!!!!!!!!!!!!!!!!!! leak
    AzGraphicsSvgItem *item = new AzGraphicsSvgItem(parent);
    item->setFlags(flags());
    item->setSharedRenderer(renderer);
    return item;
}

/*!
 * \brief AzGraphicsSvgItem::fixTransofm
 * Переводит изражение после трансормации в "оригинальный" режим.
 */
void AzGraphicsSvgItem::fixTransofm() {
    if (transform().isIdentity())
        return;
    qDebug() << "Fix transform";
    QRectF rect = sceneBoundingRect();
    QSvgRenderer* renderer = new QSvgRenderer(mapTo()); ///!!!!!!!!!!!!!!!!!!!!! leak
    resetTransform();
    setSharedRenderer(renderer);
    QPointF pos(rect.center().x()-boundingRect().width()/2,rect.center().y()-boundingRect().height()/2); //if round - fix center
    setPos(pos);
}

void AzGraphicsSvgItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) {
//    bool isSelectable = flags() & QGraphicsItem::ItemIsSelectable;
//    if (isSelectable) {
//        setFlag(QGraphicsItem::ItemIsSelectable,false);
//        painter->drawRect(boundingRect());
//    }
    //painter->drawRect(boundingRect());
    QGraphicsSvgItem::paint(painter,option,widget);
    //setFlag(QGraphicsItem::ItemIsSelectable,isSelectable);
}

QVariant AzGraphicsSvgItem::itemChange(GraphicsItemChange change, const QVariant & value) {
//    if (change == ItemSelectedHasChanged) {
//            setFlag(ItemSendsGeometryChanges,isSelected()); //turn on geometry change
//    }

    //qDebug() << change << "selected" << isSelected();
    return value;
}
