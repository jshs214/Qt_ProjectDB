#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "clientmanagerform.h"
#include "productmanagerform.h"
#include "ordermanagerform.h"
#include "chatserverform.h"
#include "chattingform.h"

#include <QSqlDatabase>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    /* ui 툴바 및 액션 설정 */
    ui->toolBar->setAllowedAreas(Qt::RightToolBarArea);
    ui->actionClient->setIcon(QIcon(":/images/client.png"));
    ui->actionProduct->setIcon(QIcon(":/images/product.png"));
    ui->actionOrder->setIcon(QIcon(":/images/order.png"));
    ui->actionServer->setIcon(QIcon(":/images/server.png"));
    ui->actionChat->setIcon(QIcon(":/images/chat.png"));
    ui->actionQuit->setIcon(QIcon(":/images/quit.png"));
    ui->toolBar->setIconSize(QSize(70, 70));
    setContextMenuPolicy (Qt::NoContextMenu);

    clientForm = new ClientManagerForm(this);
    connect(clientForm, SIGNAL(destroyed()),
            clientForm, SLOT(deleteLater()));
    productForm = new ProductManagerForm(this);
    connect(productForm, SIGNAL(destroyed()),
            productForm, SLOT(deleteLater()));
    orderForm = new OrderManagerForm(this);
    connect(orderForm, SIGNAL(destroyed()),
            orderForm, SLOT(deleteLater()));
    serverForm = new ChatServerForm(this);
    connect(serverForm, SIGNAL(destroyed()),
            serverForm, SLOT(deleteLater()));

    /* 고객 정보 추가, 제거, 변경시 주문정보관리에 정보 전송 시그널 */
    connect(clientForm, SIGNAL(clientAddToOrder(int, QString, QString, QString)),
            orderForm, SLOT(updateClient(int, QString, QString, QString)));
    connect(clientForm, SIGNAL(clientDelToOrder(int)),
            orderForm, SLOT(delClient(int)));
    connect(clientForm, SIGNAL(clientModToOrder(int, QString, QString, QString)),
            orderForm, SLOT(modClient(int, QString, QString, QString)));

    /* 제품 정보 추가, 제거, 변경시 주문정보관리에 정보 전송 시그널 */
    connect(productForm, SIGNAL(productAddToOrder(int, QString, QString, QString)),
            orderForm, SLOT(updateProduct(int, QString, QString, QString)));
    connect(productForm, SIGNAL(productDelToOrder(int)),
            orderForm, SLOT(delProduct(int)));
    connect(productForm, SIGNAL(productModToOrder(int, QString, QString, QString)),
            orderForm, SLOT(modProduct(int, QString, QString, QString)));

    /*키값(재고)를 위한 시그널 슬롯 */
    connect(orderForm, SIGNAL(productAddKeySent(int, QString)),       //주문 추가
            productForm, SLOT(receiveAddStock(int, QString)));
    connect(orderForm, SIGNAL(productModKeySent(int,QString,QString)),//주문 변경
            productForm, SLOT(receiveModStock(int, QString, QString)));
    connect(orderForm, SIGNAL(removedataSent(int, QString)),          //주문 삭제
            productForm, SLOT(receiveDelStock(int, QString)));

    /* 고객 정보 추가, 제거, 변경시 채팅서버에 정보 전송 시그널 */
    connect(clientForm, SIGNAL(clientAddToServer(int, QString)),
            serverForm, SLOT(addClient(int, QString)));
    connect(clientForm, SIGNAL(clientModToServer(int, QString)),
            serverForm, SLOT(modClient(int, QString)));
    connect(clientForm, SIGNAL(clientRevToServer(int)),
            serverForm, SLOT(remClient(int)));

    /* ui 설정 */
    ui->stackedWidget->insertWidget(0, clientForm);
    ui->stackedWidget->insertWidget(1, productForm);
    ui->stackedWidget->insertWidget(2, orderForm);
    ui->stackedWidget->insertWidget(3, serverForm);

    clientForm->showMaximized();
    ui->stackedWidget->setCurrentIndex(0);

    orderForm->loadData();
    clientForm->loadData();
    productForm->loadData();

    this->setWindowTitle("Qt_Project");
}

/* 소멸자에서 메모리 해제 */
MainWindow::~MainWindow()
{
    delete clientForm;
    delete productForm;
    delete orderForm;
    delete serverForm;
    delete ui;
}

/* 고객정보 슬롯 */
void MainWindow::on_actionClient_triggered()
{
    ui->stackedWidget->setCurrentIndex(0);
}

/* 제품정보 슬롯 */
void MainWindow::on_actionProduct_triggered()
{
    ui->stackedWidget->setCurrentIndex(1);
}


/* 주문정보 슬롯 */
void MainWindow::on_actionOrder_triggered()
{
    ui->stackedWidget->setCurrentIndex(2);
}

/* 채팅서버 슬롯 */
void MainWindow::on_actionServer_triggered()
{
    ui->stackedWidget->setCurrentIndex(3);
}

/* 채팅창 open하는 슬롯 */
void MainWindow::on_actionChat_triggered()
{
    chattingForm = new ChattingForm;
    chattingForm->show();
}



