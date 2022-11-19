#ifndef PRODUCTMANAGERFORM_H
#define PRODUCTMANAGERFORM_H

#include <QWidget>
#include <QHash>

class QMenu;
class QSqlDatabase;
class QSqlTableModel;
class productDelegate;

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

    void loadData();    /* 고객정보 DB의 데이터를 불러오는 메서드 */

private:
    Ui::ProductManagerForm *ui;
    QMenu* menu;    //ContextMenu를 위한 메뉴
    int makeId();       /* 고객 키값 생성 메서드 */
    QSqlTableModel *productModel;   //제품모델 객체

    productDelegate *m_delegate = nullptr;
    QList<int> redList;    //delegate로 배경색을 변경할 행을 저장하는 멤버변수

signals:
    /* 고객 정보 추가, 제거, 변경시 주문정보관리에 정보 전송 시그널 */
    void productAddToOrder(int, QString, QString, QString);     //추가
    void productDelToOrder(int);                                //제거
    void productModToOrder(int, QString, QString, QString);     //변경

private slots:
    void showContextMenu(const QPoint &);   /* ContextMenu 슬롯 */
    void on_clearButton_clicked();          /* 버튼 클릭 시 입력 값 초기화 하는 슬롯 */

    void on_addPushButton_clicked();        /* 제품정보추가를 위한 슬롯 */
    void on_modifyPushButton_clicked();     /* 제품정보변경을 위한 슬롯 */
    void removeItem();                      /* 제품정보삭제를 위한 슬롯 */
    void on_searchPushButton_clicked();     /* 제품정보검색을 위한 슬롯 */

    void on_productTableView_clicked(const QModelIndex &index);  /* 등록된 제품정보 클릭 시 관련정보 출력 슬롯 */
    void on_statePushButton_clicked();              /* 제품정보를 담고있는 모델의 모든 데이터 출력 슬롯 */

    void receiveAddStock(int, QString);             /* Order에서 주문추가 시, 재고반영을 위한 슬롯 */
    void receiveModStock(int, QString, QString);    /* Order에서 주문변경 시, 재고반영을 위한 슬롯 */
    void receiveDelStock(int, QString);             /* Order에서 주문내역제거 시, 재고반영을 위한 슬롯 */

};

#endif // CLIENTMANAGERFORM_H
