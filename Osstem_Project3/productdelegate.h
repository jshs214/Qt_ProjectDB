#ifndef PRODUCTDELEGATE_H
#define PRODUCTDELEGATE_H

#include <QStyledItemDelegate>
#include <QDateTime>
#include <QVector>

/* 뷰를 사용할 때 상태에 따른 배경 설정을 해주는 델리게이트 클래스  */
class productDelegate : public QStyledItemDelegate
{
public:
    using QStyledItemDelegate::QStyledItemDelegate;

    /* 벡터에 index 값을 받아오는 set함수 */
    void setRedRows(const QVector<int> &rows);


protected:
    void initStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const override;
private:
    QVector<int> m_redRows;       /* 배경 설정을 위한 행의 index를 저장하는 벡터 */

};

#endif // PRODUCTDELEGATE_H
