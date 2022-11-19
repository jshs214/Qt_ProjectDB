#include "orderdelegate.h"

#include <QVector>

/* 벡터에 index 값을 받아오는 set함수 */
void orderDelegate::setRedRows(const QVector<int> &rows)
{
    m_redRows=(rows);
}

void orderDelegate::initStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const {
    QStyledItemDelegate::initStyleOption(option, index);

    /* 행에 index에 따라 배경색 설정 */
    if(m_redRows.contains(index.row())){
        option->backgroundBrush = QBrush(QColor(255,162,162));
    }


}
