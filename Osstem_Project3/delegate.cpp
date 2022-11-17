#include "delegate.h"

#include <QVector>


QVector<int> Delegate::greenRows() const
{
    return m_greenRows;
}
QVector<int> Delegate::grayRows() const
{
    return m_grayRows;
}


void Delegate::setGreenRows(const QVector<int> &rows)
{
    m_greenRows = (rows);
}
void Delegate::setGrayRows(const QVector<int> &rows)
{
    m_grayRows=(rows);
}


void Delegate::initStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const {
    QStyledItemDelegate::initStyleOption(option, index);
    //QVariant value = index.data();

    if(m_greenRows.contains(index.row())){
        option->backgroundBrush = QBrush(QColor("lightgreen"));
    }
    if(m_grayRows.contains(index.row())){
        option->backgroundBrush = QBrush(QColor("lightgray"));
    }

}
