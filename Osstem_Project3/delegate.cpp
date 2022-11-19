#include "delegate.h"

#include <QVector>

/* 벡터에 index 값을 받아오는 set함수 */
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

    /* 행에 index에 따라 배경색 회색, 초록색 설정 */
    if(m_greenRows.contains(index.row())){
        option->backgroundBrush = QBrush(QColor("lightgreen"));
    }
    if(m_grayRows.contains(index.row())){
        option->backgroundBrush = QBrush(QColor("lightgray"));
    }

}
