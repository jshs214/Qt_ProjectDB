#ifndef ORDERMANAGERFORM_H
#define ORDERMANAGERFORM_H

#include <QWidget>
#include <QHash>

class QMenu;
class QTreeWidgetItem;

namespace Ui {
class OrderManagerForm;
}

/* 주문정보관리 클래스 */
class OrderManagerForm : public QWidget
{
    Q_OBJECT

public:
    explicit OrderManagerForm(QWidget *parent = nullptr);
    ~OrderManagerForm();
private:
    QMenu* menu;    //ContextMenu를 위한 메뉴
    Ui::OrderManagerForm *ui;

signals:


private slots:
    void showContextMenu(const QPoint &);   /* ContextMenu 슬롯 */
    void removeItem();  /* QAction을 위한 데이터 제거 슬롯 */
    void on_clearButton_clicked();/* 버튼 클릭 시 입력 값 초기화 하는 슬롯 */

    void updateClient(int,QString, QString, QString);   //추가 or loaddata 고객
    void delClient(int);    //고객 제거 반영
    void modClient(int, QString, QString, QString);    //고객 제거 반영

    void on_clientButton_clicked();
};

#endif // ORDERMANAGERFORM_H
