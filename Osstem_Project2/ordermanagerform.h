#ifndef ORDERMANAGERFORM_H
#define ORDERMANAGERFORM_H

#include <QWidget>
#include <QHash>

class QMenu;
class QTreeWidgetItem;
class QSqlTableModel;

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

    void loadData();
    int makeId();
private:
    QMenu* menu;    //ContextMenu를 위한 메뉴
    Ui::OrderManagerForm *ui;
    QSqlTableModel *orderModel;

signals:
    void productAddKeySent(int, QString);  /* 재고반영을 위해 제품키값과, 수량을 시그널로 보냄 */
    void productModKeySent(int, QString, QString); /* 재고반영을 위해 키값, 수정할 수량, 입력된 수량을 시그널로 보냄 */
    void removedataSent(int, QString);     /* 재고반영을 위해 데이터 제거 시, 제품 ID와 수량을 시그널로 보냄 */


private slots:
    void showContextMenu(const QPoint &);   /* ContextMenu 슬롯 */
    void removeItem();  /* QAction을 위한 데이터 제거 슬롯 */
    void on_clearButton_clicked();/* 버튼 클릭 시 입력 값 초기화 하는 슬롯 */

    void updateClient(int,QString, QString, QString);   //추가 or loaddata 고객
    void delClient(int);    //고객 제거 반영
    void modClient(int, QString, QString, QString);    //고객 제거 반영

    void on_clientButton_clicked();
    void on_clientTreeWidget_itemClicked(QTreeWidgetItem *item, int column);

    void updateProduct(int,QString, QString, QString);
    void delProduct(int);    //고객 제거 반영
    void modProduct(int, QString, QString, QString);    //고객 제거 반영

    void on_productButton_clicked();
    void on_productTreeWidget_itemClicked(QTreeWidgetItem *item, int column);
    void on_addPushButton_clicked();
    void on_stockLineEdit_textChanged(const QString &arg1);
    void on_modifyPushButton_clicked();
    void on_orderTableView_clicked(const QModelIndex &index);
    void on_searchPushButton_clicked();
    void on_statePushButton_clicked();
};

#endif // ORDERMANAGERFORM_H
