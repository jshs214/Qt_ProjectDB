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

private slots:
    void on_clearButton_clicked();      /* 버튼 클릭 시 입력 값 초기화 하는 슬롯 */
    void showContextMenu(const QPoint &);   /* ContextMenu 슬롯 */
    void removeItem();      /* 제품정보의 데이터(트리위젯)의 리스트 제거 슬롯 */


    void on_addPushButton_clicked();
    void on_modifyPushButton_clicked();
    void on_statePushButton_clicked();
    void on_searchPushButton_clicked();
    void on_productTableView_clicked(const QModelIndex &index);
};

#endif // CLIENTMANAGERFORM_H
