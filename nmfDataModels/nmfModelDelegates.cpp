

//============================================================================
// Name        : nmfModelDelegates.cpp
// Author      : Ron Klasky
// Version     :
// Copyright   : NOAA - National Marine Fisheries Service
// Description :
//============================================================================

#include "nmfModelDelegates.h"



nmfTableItemDelegate::nmfTableItemDelegate(QObject *parent) :
        QStyledItemDelegate(parent)
{

}

QString nmfTableItemDelegate::displayText(const QVariant & value,
        const QLocale & locale) const
{
    QString str = QString::number(value.toDouble(), 'f', 3);
    return str;
}

void nmfTableItemDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    QStyleOptionViewItem myOption = option;

    (myOption.displayAlignment ^= Qt::AlignLeft) |= Qt::AlignRight;

    QStyledItemDelegate::paint(painter, myOption, index);
}
