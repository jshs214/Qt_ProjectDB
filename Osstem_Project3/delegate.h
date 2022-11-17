#ifndef DELEGATE_H
#define DELEGATE_H

#include <QStyledItemDelegate>
#include <QDateTime>
#include <QVector>

class Delegate : public QStyledItemDelegate
{
public:
    using QStyledItemDelegate::QStyledItemDelegate;

    QVector<int> greenRows() const;
    QVector<int> grayRows() const;

    void setGreenRows(const QVector<int> &rows);
    void setGrayRows(const QVector<int> &rows);

protected:
    void initStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const override;
private:
    QVector<int> m_greenRows;
    QVector<int> m_grayRows;
};

#endif // DELEGATE_H
