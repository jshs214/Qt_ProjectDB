#ifndef PRODUCTMANAGERFORM_H
#define PRODUCTMANAGERFORM_H

#include <QWidget>
#include <QHash>

class QMenu;

class QSqlDatabase;
class QSqlTableModel;

namespace Ui {
class ProductManagerForm;
}

/* 제품정보관리 클래스 */
class ProductManagerForm : public QWidget
{
    Q_OBJECT

public:
    explicit ProductManagerForm(QWidget *parent = nullptr);
    ~ProductManagerForm();

    void loadData();
    int makeId();
private:
    QMenu* menu;
    Ui::ProductManagerForm *ui;

    QSqlTableModel *productModel;

signals:
    void productAddToOrder(int, QString, QString, QString);
    void productDelToOrder(int);
    void productModToOrder(int, QString, QString, QString);

private slots:
    void on_clearButton_clicked();      /* 버튼 클릭 시 입력 값 초기화 하는 슬롯 */
    void showContextMenu(const QPoint &);   /* ContextMenu 슬롯 */
    void removeItem();      /* 제품정보의 데이터(트리위젯)의 리스트 제거 슬롯 */

    void on_addPushButton_clicked();
    void on_modifyPushButton_clicked();
    void on_statePushButton_clicked();
    void on_searchPushButton_clicked();
    void on_productTableView_clicked(const QModelIndex &index);

    void receiveAddStock(int, QString);     /* Order에서 주문추가 시, 재고반영을 위한 슬롯 */
    void receiveModStock(int, QString, QString);    /* Order에서 주문변경 시, 재고반영을 위한 슬롯 */
    void receiveDelStock(int, QString);     /* Order에서 주문내역제거 시, 재고반영을 위한 슬롯 */

};

#endif // CLIENTMANAGERFORM_H
