
/**
 @file nmfModelCatchAtAge.h
 @author rklasky
 @copyright 2017 NOAA - National Marine Fisheries Service
 @brief
 @date Dec 9, 2016
*/

#ifndef NMFMODELDELEGATES_H
#define NMFMODELDELEGATES_H


#include <QStyledItemDelegate>
#include <QStyleOptionViewItem>

class /*NMFMODELDELEGATESSHARED_EXPORT*/ nmfTableItemDelegate : public QStyledItemDelegate
{

   Q_OBJECT

public:

   nmfTableItemDelegate(QObject *parent = 0);

   QString displayText(const QVariant & value, const QLocale & locale) const;


   void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;
};



#endif // NMFMODELDELEGATES_H
