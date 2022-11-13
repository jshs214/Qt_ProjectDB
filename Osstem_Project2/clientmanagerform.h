#ifndef CLIENTMANAGERFORM_H
#define CLIENTMANAGERFORM_H

#include <QWidget>
#include <QHash>

#include <QTcpSocket>

class QMenu;
class QSqlDatabase;
class QSqlTableModel;

namespace Ui {
class ClientManagerForm;
}

/* 고객정보관리 클래스 */
class ClientManagerForm : public QWidget
{
    Q_OBJECT

public:
    explicit ClientManagerForm(QWidget *parent = nullptr);
    ~ClientManagerForm();

    void loadData();    /* 고객정보 DB의 데이터를 불러오는 메서드 */

private:
    Ui::ClientManagerForm *ui;
    QMenu* menu;         //ContextMenu를 위한 메뉴
    int makeId();        /* 고객 키값 생성 메서드 */
    QSqlTableModel *clientModel;    //고객모델 객체

signals:
    /* 고객 정보 추가, 제거, 변경시 주문정보관리에 정보 전송 시그널 */
    void clientAddToOrder(int, QString, QString, QString);      // 추가
    void clientDelToOrder(int);                                 // 제거
    void clientModToOrder(int, QString, QString, QString);      // 변경

    /* 고객 정보 추가, 제거, 변경시 채팅서버에 정보 전송 시그널 */
    void clientAddToServer(int, QString);   // 추가
    void clientRevToServer(int);            // 제거
    void clientModToServer(int, QString);   // 변경

private slots:
    void showContextMenu(const QPoint &);   /* ContextMenu 슬롯 */
    void on_addPushButton_clicked();        /* 고객정보추가를 위한 슬롯 */
    void on_modifyPushButton_clicked();     /* 고객정보변경을 위한 슬롯 */
    void removeItem();                      /* 고객정보삭제를 위한 슬롯 */
    void on_searchPushButton_clicked();     /* 고객정보검색을 위한 슬롯 */
    void on_statePushButton_clicked();      /* 고객정보를 담고있는 모델의 모든 데이터 출력 슬롯 */
    void on_clientTableView_clicked(const QModelIndex &index);      /* 등록된 고객정보 클릭 시 관련정보 출력 슬롯 */
    void on_clearButton_clicked();          /* 버튼 클릭 시 입력 값 초기화 하는 슬롯 */
};

#endif // CLIENTMANAGERFORM_H
