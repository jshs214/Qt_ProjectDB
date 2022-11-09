#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMdiSubWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class ClientManagerForm;
class ProductManagerForm;
class OrderManagerForm;
class ChatServerForm;
class ChattingForm;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    ClientManagerForm *clientForm;      /* 고객정보관리form */
    ProductManagerForm *productForm;    /* 제품정보관리form */
    OrderManagerForm *orderForm;        /* 주문정보관리form */
    ChatServerForm *serverForm;         /* 채팅서버form */
    ChattingForm *chattingForm;         /* 채팅창form */

private slots:
    void on_actionClient_triggered();   /* 고객정보 슬롯 */
    void on_actionProduct_triggered();  /* 제품정보 슬롯 */
    void on_actionOrder_triggered();    /* 주문정보 슬롯 */
    void on_actionServer_triggered();   /* 채팅서버 슬롯 */
    void on_actionChat_triggered();     /* 채팅창 open하는 슬롯 */

};
#endif // MAINWINDOW_H
