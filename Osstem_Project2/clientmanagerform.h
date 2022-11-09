#ifndef CLIENTMANAGERFORM_H
#define CLIENTMANAGERFORM_H

#include <QWidget>
#include <QHash>

#include <QTcpSocket>

class QMenu;
class QSqlDatabase;
class QSqlTableModel;

class QMenu;

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

    void loadData();
    int makeId();
private:
    QMenu* menu;    //ContextMenu를 위한 메뉴
    Ui::ClientManagerForm *ui;
    QSqlTableModel *clientModel;

signals:
    void clientAddToServer(int, QString);
    void clientAddToOrder(int, QString, QString, QString);
    void clientDelToOrder(int);
    void clientModToOrder(int, QString, QString, QString);

private slots:

    void on_clearButton_clicked();       /* 버튼 클릭 시 입력 값 초기화 하는 슬롯 */
    void showContextMenu(const QPoint &);   /* ContextMenu 슬롯 */
    void on_addPushButton_clicked();
    void on_modifyPushButton_clicked();
    void removeItem();
    void on_searchPushButton_clicked();
    void on_statePushButton_clicked();

    void on_clientTableView_clicked(const QModelIndex &index);
};

#endif // CLIENTMANAGERFORM_H
