#ifndef ORDERMANAGERFORM_H
#define ORDERMANAGERFORM_H

#include <QWidget>
#include <QHash>

class QMenu;
class QTreeWidgetItem;
class QSqlTableModel;
class QStandardItemModel;
class orderDelegate;

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

    void loadData();    /* 주문정보 DB의 데이터를 불러오는 메서드 */

private:
    Ui::OrderManagerForm *ui;
    int makeId();       /* 주문 키값 생성 메서드 */
    QMenu* menu;    //ContextMenu를 위한 메뉴

    QSqlTableModel *orderModel;     //주문모델 객체

    QStandardItemModel *clientItemModel;    //고객Item모델
    QStandardItemModel *productItemModel;   //제품Item모델

    QStandardItemModel *searchClientModel;  //고객검색Item모델
    QStandardItemModel *searchProductModel; //제품검색Item모델

    orderDelegate *m_delegate = nullptr;
    QList<int> redList;    //delegate로 배경색을 변경할 행을 저장하는 멤버변수

signals:
    /* 재고반영을 위한 시그널  */
    void productAddKeySent(int, QString);           // 추가. 제품id, 수량
    void productModKeySent(int, QString, QString);  // 변경. 제품id, 변경할 수량, 기존 재고량
    void removedataSent(int, QString);              // 제거. 제품 ID와 수량


private slots:
    void showContextMenu(const QPoint &);   /* ContextMenu 슬롯 */
    void on_clearButton_clicked();/* 버튼 클릭 시 입력 값 초기화 하는 슬롯 */

    /* 고객정보 추가, 제거, 변경 시 데이터 반영 */
    void updateClient(int,QString, QString, QString);   //데이터 추가
    void delClient(int);                                //데이터 제거
    void modClient(int, QString, QString, QString);     //데이터 변경

    /* 제품정보 추가, 제거, 변경 시 데이터 반영 */
    void updateProduct(int,QString, QString, QString);  //데이터 추가
    void delProduct(int);                               //데이터 제거
    void modProduct(int, QString, QString, QString);    //데이터 변경

    /* 고객, 제품 검색 시, 콤보박스index에 따른 모델 설정 및 버튼상태 변경 */
    void on_clientComboBox_activated(int index);        //고객
    void on_productComboBox_activated(int index);       //제품

    /*고객, 제품 id나 이름 검색하는 슬롯 */
    void on_clientButton_clicked();     //고객
    void on_productButton_clicked();    //제품

    /* 고객, 제품 데이터 클릭 시 LineEdit에 입력하는 슬롯 */
    void on_clientItemTreeView_clicked(const QModelIndex &index);
    void on_productItemTreeView_clicked(const QModelIndex &index);

    /* 주문정보 추가, 변경, 삭제, 검색 */
    void on_addPushButton_clicked();    //추가
    void on_modifyPushButton_clicked(); //변경
    void removeItem();                  //삭제
    void on_searchPushButton_clicked(); //검색

    void on_stockLineEdit_textChanged(const QString &arg1);     /* 총합 계산 슬롯 */
    void on_orderTableView_clicked(const QModelIndex &index);   /* 등록된 주문정보 클릭 시 관련정보 출력 슬롯 */
    void on_statePushButton_clicked();                          /* 주문정보를 담고있는 모델의 모든 데이터 출력 슬롯 */

};

#endif // ORDERMANAGERFORM_H
