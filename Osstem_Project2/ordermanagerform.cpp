#include "ordermanagerform.h"
#include "qsqltablemodel.h"
#include "ui_ordermanagerform.h"

#include "ordermanagerform.h"

#include <QFile>
#include <QMenu>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

OrderManagerForm::OrderManagerForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OrderManagerForm)
{
    ui->setupUi(this);

    /* ContextMenu의 remove 액션, 리스트의 데이터 삭제 */
    QAction* removeAction = new QAction(tr("&Remove"));
    removeAction->setIcon(QIcon(":/images/eraser.png"));
    connect(removeAction, SIGNAL(triggered()), SLOT(removeItem()));
    menu = new QMenu;
    menu->addAction(removeAction);
    ui->orderTableView->setContextMenuPolicy(Qt::CustomContextMenu);

    /* 테이블 뷰에 컨텍스트메뉴 사용한 시그널, 슬롯 */
    connect(ui->orderTableView, SIGNAL(customContextMenuRequested(QPoint)), this,
            SLOT(showContextMenu(QPoint)));

    ui->stockLineEdit->setValidator(new QIntValidator(0, 9999, this) ); //수량에 0~9999 범위의 숫자만 받도록

    /* 엔터키로 버튼 클릭 가능하게 슬롯 연결 */
    connect(ui->searchLineEdit, SIGNAL(returnPressed()), this, SLOT(on_searchPushButton_clicked()));
    connect(ui->clientLineEdit, SIGNAL(returnPressed()), this, SLOT(on_clientButton_clicked()));
    connect(ui->productLineEdit, SIGNAL(returnPressed()), this, SLOT(on_productButton_clicked()));

    /* ui의 트리위젯들의 열 너비 설정 */
    ui->clientTreeWidget->QTreeView::setColumnWidth(0,40);
    ui->clientTreeWidget->QTreeView::setColumnWidth(1,70);
    ui->clientTreeWidget->QTreeView::setColumnWidth(2,95);
    ui->productTreeWidget->QTreeView::setColumnWidth(0,40);
    ui->productTreeWidget->QTreeView::setColumnWidth(1,70);
    ui->productTreeWidget->QTreeView::setColumnWidth(2,95);
}


/* 소멸자에서 DB close */
OrderManagerForm::~OrderManagerForm()
{
    delete ui;
    /* 주문 DB close  */
    QSqlDatabase db = QSqlDatabase::database("orderConnection");
    if(db.isOpen()) {
        orderModel->submitAll();
        db.close();
    }
}

/* 주문정보 DB의 데이터를 불러오는 메서드 */
void OrderManagerForm::loadData()
{
    /*  Qt에서 지원하는 데이터베이스 드라이버 QSQLITE에 주문 DB 객체 선언  */
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "orderConnection");
    db.setDatabaseName("order.db");     // DB명은 order.db
    if (db.open()) {
        QSqlQuery query(db);            // db 를 사용하여 QSqlQuery 객체를 생성

        /* SQL 쿼리문을 사용해 고객 테이블 생성 */
        query.exec("CREATE TABLE IF NOT EXISTS orderList(id INTEGER Primary Key, client VARCHAR(30) NOT NULL,"
                   "product VARCHAR(30) NOT NULL, stock VARCHAR(10), "
                   "price VARCHAR(30), sum VARCHAR(30), address VARCHAR(30));");

        orderModel = new QSqlTableModel(this, db);  // QSqlTableModel을 이용해 주문모델 객체 생성
        orderModel->setTable("orderList");          //모델이 작동하는 DB 테이블 설정
        orderModel->select();                       //모델의 데이터 조회

        /* DB 테이블 헤더 명 설정 */
        orderModel->setHeaderData(0, Qt::Horizontal, QObject::tr("OrderNum"));
        orderModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Client"));
        orderModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Product"));
        orderModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Stock"));
        orderModel->setHeaderData(4, Qt::Horizontal, QObject::tr("Price"));
        orderModel->setHeaderData(5, Qt::Horizontal, QObject::tr("Sum"));
        orderModel->setHeaderData(6, Qt::Horizontal, QObject::tr("Address"));

        ui->orderTableView->setModel(orderModel);       //ui에 표시할 모델 설정

        ui->orderTableView->resizeColumnsToContents();  //컬럼 사이즈를 데이터에 맞게 조절
    }
}

/* 주문 키값 생성 메서드 */
int OrderManagerForm::makeId( )
{
    /* 키값 생성 시, 중복 되지 않게 모델에 존재하는 키값의 마지막 +1 하여 생성  */
    if(orderModel->rowCount() == 0) {
        return 230000;
    } else {
        auto id = orderModel->data(orderModel->index(orderModel->rowCount()-1, 0)).toInt();
        return ++id;
    }
}

/* ContextMenu 슬롯 */
void OrderManagerForm::showContextMenu(const QPoint &pos)
{
    QPoint globalPos = ui->orderTableView->mapToGlobal(pos);
    if(ui->orderTableView->indexAt(pos).isValid())
        menu->exec(globalPos);
}

/* 버튼 클릭 시 LineEdit값 초기화 하는 슬롯 */
void OrderManagerForm::on_clearButton_clicked()
{
    ui->cIdLineEdit->clear();
    ui->pIdLineEdit->clear();
    ui->stockLineEdit->clear();
    ui->addressLineEdit->clear();
    ui->priceLineEdit->clear();
    ui->searchLineEdit->clear();
    ui->clientLineEdit->clear();
    ui->productLineEdit->clear();
}

/* 고객정보 데이터 추가 */
void OrderManagerForm::updateClient(int id, QString name,
                                    QString phoneNumber, QString address)   //고객id, 이름, 전화번호, 주소
{
    QTreeWidgetItem* cItem = new QTreeWidgetItem(ui->clientTreeWidget);
    cItem->setText(0,QString::number(id));
    cItem->setText(1,name);
    cItem->setText(2,phoneNumber);
    cItem->setText(3,address);
}

/* 고객정보 데이터 제거 */
void OrderManagerForm::delClient(int id)    //고객 id
{
    /* 고객트리위젯에서 id와 일치하는지 찾음  */
    auto item = ui->clientTreeWidget->findItems(QString::number(id), Qt::MatchFixedString, 0);
    /* id 가일치한다면 트리위젯의 지정된 인덱스 에서 항목 제거 */
    foreach(auto i, item) {
        if(QString::number(id) == i->text(0))
            ui->clientTreeWidget->takeTopLevelItem(ui->clientTreeWidget
                                                   ->indexOfTopLevelItem(i));
        delete i;
    }


}

/* 고객정보 데이터 변경 */
void OrderManagerForm::modClient(int id, QString name,
                                 QString phoneNumber, QString address)  //고객id, 이름, 전화번호, 주소
{
    /* 고객트리위젯에서 id와 일치하는지 찾음  */
    auto item = ui->clientTreeWidget->findItems(QString::number(id), Qt::MatchFixedString, 0);        //키값비교, 0
    /* id 가일치한다면 지정된 인덱스 의 데이터 변경 */
    foreach(auto i, item) {
        if(QString::number(id) == i->text(0)){
            i->setText(0, QString::number(id));
            i->setText(1, name);
            i->setText(2, phoneNumber);
            i->setText(3, address);
        }
    }
}

/* 제품정보 데이터 추가 */
void OrderManagerForm::updateProduct(int id, QString name,
                                     QString price, QString stock)  //제품id, 이름, 가격, 수량
{
    QTreeWidgetItem* pItem = new QTreeWidgetItem(ui->productTreeWidget);
    pItem->setText(0,QString::number(id));
    pItem->setText(1,name);
    pItem->setText(2,price);
    pItem->setText(3,stock);
}

/* 제품정보 데이터 제거 */
void OrderManagerForm::delProduct(int id)
{
    /* 제품트리위젯에서 id와 일치하는지 찾음  */
    auto item = ui->productTreeWidget->findItems(QString::number(id), Qt::MatchFixedString, 0);
    /* id 가일치한다면 트리위젯의 지정된 인덱스 에서 항목 제거 */
    foreach(auto i, item) {
        if(QString::number(id) == i->text(0))
            ui->productTreeWidget->takeTopLevelItem(ui->productTreeWidget->indexOfTopLevelItem(i));
        delete i;
    }

}

/* 제품정보 데이터 변경 */
void OrderManagerForm::modProduct(int id, QString name, QString price, QString stock)
{
    /* 제품트리위젯에서 id와 일치하는지 찾음  */
    auto item = ui->productTreeWidget->findItems(QString::number(id), Qt::MatchFixedString, 0);        //키값비교, 0
    /* id 가일치한다면 지정된 인덱스 의 데이터 변경 */
    foreach(auto i, item) {
        if(QString::number(id) == i->text(0)){
            i->setText(0, QString::number(id));
            i->setText(1, name);
            i->setText(2, price);
            i->setText(3, stock);
        }
    }
}

/*고객 id나 이름 검색하는 슬롯 */
void OrderManagerForm::on_clientButton_clicked()
{
    QString str = ui->clientLineEdit->text();
    auto flag =  Qt::MatchCaseSensitive|Qt::MatchContains;

    /* 검색 시, 고객 트리위젯의 모든 데이터 hidden */
    for(int i =0; i < ui->clientTreeWidget->topLevelItemCount() ; i++)
    {
        ui->clientTreeWidget->topLevelItem(i)->setHidden(true);
    }

    /* id 검색 시, 검색값과 id가 완전일치 하면 위젯에 출력 */
    auto cItem = ui->clientTreeWidget->findItems(str, Qt::MatchFixedString, 0); //id 는 완전일치
    foreach(auto i, cItem) {
        i->setHidden(false);
    }
    /* 이름 검색 시, 검색값과 이름이 부분일치 하면 위젯에 출력 */
    cItem = ui->clientTreeWidget->findItems(str, flag, 1);
    foreach(auto i, cItem) {
        i->setHidden(false);
    }

}

/* 제품 id나 이름 검색하는 슬롯 */
void OrderManagerForm::on_productButton_clicked()
{
    QString str = ui->productLineEdit->text();
    auto flag =  Qt::MatchCaseSensitive|Qt::MatchContains;

    /* 검색 시, 제품 트리위젯의 모든 데이터 hidden */
    for(int i =0; i < ui->productTreeWidget->topLevelItemCount() ; i++)
    {
        ui->productTreeWidget->topLevelItem(i)->setHidden(true);
    }

    /* id 검색 시, 검색값과 id가 완전일치 하면 위젯에 출력 */
    auto pItem = ui->productTreeWidget->findItems(str, Qt::MatchFixedString, 0); //id 는 완전일치
    foreach(auto i, pItem) {
        i->setHidden(false);
    }

    /* 이름 검색 시, 검색값과 제품명이 부분일치 하면 위젯에 출력 */
    pItem = ui->productTreeWidget->findItems(str, flag, 1);
    foreach(auto i, pItem) {
        i->setHidden(false);
    }
}

/* 고객 데이터 클릭 시 LineEdit에 입력하는 슬롯 */
void OrderManagerForm::on_clientTreeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);
    ui->idLineEdit->clear();
    QString client;
    client = item->text(0)+" ("+item->text(1)+")";

    ui->cIdLineEdit->setText(client);
    ui->addressLineEdit->setText(item->text(3));
}

/* 제품 데이터 클릭 시 LineEdit에 입력하는 슬롯 */
void OrderManagerForm::on_productTreeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);
    ui->idLineEdit->clear();
    ui->stockLineEdit->clear();
    QString product;
    product = item->text(0)+" ("+item->text(1)+")";

    ui->pIdLineEdit->setText(product);
    ui->priceLineEdit->setText(item->text(2));
}

/* 주문 정보 추가 */
void OrderManagerForm::on_addPushButton_clicked()
{
    QTreeWidgetItem* pItem = ui->productTreeWidget->currentItem();

    QString  client, product, stock, price, address, sum;
    QString productKey, productTree_Stock;  //제품id, 주문 수량

    /* 추가 시 LineEdit에 입력했던 고객, 제품, 수량, 가격, 주소, 총 합의 데이터 저장 */
    int id = makeId( );
    ui->idLineEdit->setText(QString::number(id));
    client = ui->cIdLineEdit->text();
    product = ui->pIdLineEdit->text();
    stock = ui->stockLineEdit->text();
    price = ui->priceLineEdit->text();
    address = ui->addressLineEdit->text();
    sum = ui->sumLineEdit->text();

    productTree_Stock = pItem->text(3);              //제품리스트의 재고
    productKey =  ui->pIdLineEdit->text().left(5);   //제품 id

    /* 재고반영을 위한 시그널 */
    emit productAddKeySent(productKey.toInt(), stock ); //제품id, 주문수량

    if(pItem == nullptr) return; // 예외

    /* 주문 수량이 재고보다 많을 경우 예외 */
    if(productTree_Stock.toInt() < ui->stockLineEdit->text().toInt()) return;

    /* 주문이 되면 제품리스트의 재고를 변경 */
    QString result_stock = QString::number(productTree_Stock.toInt() - stock.toInt());
    pItem->setText((3), result_stock);

    /* 주문 할 데이터를 다 입력, 재고>=주문수량이면 주문정보 추가 */
    if( client.length() && product.length() && stock.length() &&price.length() && address.length()
            && productTree_Stock.toInt() >= stock.toInt())
    {
        QSqlQuery query(orderModel->database());    //QSqlQuery 객체(주문모델)
        query.prepare("INSERT INTO orderList VALUES (?, ?, ?, ?, ?, ?, ?)");    //sql쿼리문 준비
        /* sql쿼리문에 값 바인딩 */
        query.bindValue(0, id);
        query.bindValue(1, client);
        query.bindValue(2, product);
        query.bindValue(3, stock);
        query.bindValue(4, price);
        query.bindValue(5, sum);
        query.bindValue(6, address);

        query.exec();             //sql쿼리문 실행
        orderModel->select();     //모델의 데이터 조회

        /* ui의 LineEdit 값 clear */
        ui->cIdLineEdit->clear();
        ui->pIdLineEdit->clear();
        ui->stockLineEdit->clear();
        ui->addressLineEdit->clear();
        ui->priceLineEdit->clear();
    }

    ui->orderTableView->resizeColumnsToContents();  //컬럼 사이즈를 데이터에 맞게 조절
}

/* 주문 정보 변경 */
void OrderManagerForm::on_modifyPushButton_clicked()
{

    QModelIndex index = ui->orderTableView->currentIndex();
    if(index.isValid()) {
        QString client, product, stock, price, address, sum;
        QString productKey, orderStock; //제품 id, 주문수량

        /* id는 모델의 현재 데이터 값 */
        /* 추가 시 LineEdit에 입력했던 고객, 제품, 수량, 가격, 주소, 총 합의 데이터 저장 */
        int id = orderModel->data(index.siblingAtColumn(0)).toInt();
        client = ui->cIdLineEdit->text();
        product = ui->pIdLineEdit->text();
        stock = ui->stockLineEdit->text();
        price = ui->priceLineEdit->text();
        address = ui->addressLineEdit->text();
        sum = ui->sumLineEdit->text();

        QSqlQuery query(orderModel->database());    //QSqlQuery 객체(주문모델)

        // 주문정보 변경 전 기존 주문수량 조회
        query.prepare("SELECT product, stock FROM orderList WHERE id = ?"); //sql쿼리문 준비
        query.bindValue(0, id);     //sql쿼리문에 값 바인딩
        query.exec();               //sql 쿼리 실행

        while (query.next()) {
            productKey = query.value(0).toString().left(5); // 제품id
            orderStock = query.value(1).toString();         // 입력된 수량 (주문되어있는 재고)
        }


        /* 제품 수량 변경 시, 제품리스트에 있는 재고 반영*/
        {
            /* 주문 변경 시, 제품id가 일치하는 제품리스트의 재고 변경 */
            auto pitem = ui->productTreeWidget->findItems(productKey, Qt::MatchFixedString, 0);
            foreach(auto i, pitem) {
                if(productKey == i->text(0)){
                    int result, beInStock;
                    QString productStock = i->text(3);
                    result = productStock.toInt() +orderStock.toInt() - ui->stockLineEdit->text().toInt();  //변경 가능한 재고량
                    beInStock = productStock.toInt() +orderStock.toInt();   //제품리스트의 재고 + 주문되어있는 재고

                    /* 주문변경 가능 수량에 대한 예외처리 */
                    if( beInStock< ui->stockLineEdit->text().toInt()){
                        QMessageBox::information(this, tr("Error"),
                                                 QString(tr("out of stock\nYou can change up to %0."))
                                                 .arg(beInStock));
                        return;
                    }
                    i->setText(3, QString::number(result));
                }
            }
        }

        /* 주문변경 시 재고반영을 위한 시그널 */
        emit productModKeySent(productKey.toInt(), stock , orderStock); //제품id, 수정할 수량, 입력된 수량 (주문되어있는 재고)

        query.prepare("UPDATE orderList SET client = ?, product = ?, stock = ?,"
                      "price = ?, sum = ?, address = ? WHERE id = ?");  //sql쿼리문 준비
        /* sql쿼리문에 값 바인딩 */
        query.bindValue(0, client);
        query.bindValue(1, product);
        query.bindValue(2, stock);
        query.bindValue(3, price);
        query.bindValue(4, sum);
        query.bindValue(5, address);
        query.bindValue(6, id);

        query.exec();           //sql 쿼리 실행
        orderModel->select();   //모델의 데이터 조회
    }
    ui->orderTableView->resizeColumnsToContents();  //컬럼 사이즈를 데이터에 맞게 조절
}

/* 주문 정보 제거 */
void OrderManagerForm::removeItem()
{
    QString productKey, delStock;   //제품id, 주문제거한 수량

    QModelIndex index = ui->orderTableView->currentIndex();
    productKey = index.sibling(index.row(), 2).data().toString().left(5);
    delStock = index.sibling(index.row(), 3).data().toString();

    /* 제품 수량 변경 시, 제품리스트에 있는 재고 반영*/
    {
        /* 주문 삭제 시, 제품id가 일치하는 제품리스트의 재고에 추가 */
        auto pitem = ui->productTreeWidget->findItems(productKey, Qt::MatchFixedString, 0);
        foreach(auto i, pitem) {
            if(productKey == i->text(0)){
                QString productStock = i->text(3);
                int result = productStock.toInt() + delStock.toInt();
                i->setText(3, QString::number(result));
            }
        }
    }

    if(index.isValid()) {
        orderModel->removeRow(index.row()); //모델의 현재 행 삭제
        orderModel->select();               //모델의 데이터 조회
        ui->orderTableView->update();       //뷰 update
    }

    /* 주문삭제 시 재고반영을 위한 시그널 */
    emit removedataSent(productKey.toInt() , delStock); //제품id, 주문제거한 수량
}

/* 주문 정보 검색 */
void OrderManagerForm::on_searchPushButton_clicked()
{
    QString searchValue = ui->searchLineEdit->text();   //검색할 데이터

    int i = ui->searchComboBox->currentIndex(); //무엇으로 검색할지 콤보박스의 인덱스를 가져옴
    switch (i){
    case 0: //주문번호 검색
        /* 검색한 데이터와 id가 일치하면 뷰에 검색결과 출력 후 메시지박스 */
        orderModel->setFilter(QString("id = '%1'").arg(searchValue));
        orderModel->select();
        QMessageBox::information(this, tr("Search Info"),
                                 QString( tr("%1 search results were found") ).arg(orderModel->rowCount()));
        break;
    case 1: //client 검색
        /* 검색한 데이터에 client이 포함되면 뷰에 검색결과 출력 후 메시지박스 */
        orderModel->setFilter(QString("client LIKE '%%1%'").arg(searchValue));
        orderModel->select();
        QMessageBox::information(this, tr("Search Info"),
                                 QString( tr("%1 search results were found") ).arg(orderModel->rowCount()));
        break;
    case 2: //product 검색
        /* 검색한 데이터에 product이 포함되면 뷰에 검색결과 출력 후 메시지박스 */
        orderModel->setFilter(QString("product LIKE '%%1%'").arg(searchValue));
        orderModel->select();
        QMessageBox::information(this, tr("Search Info"),
                                 QString( tr("%1 search results were found") ).arg(orderModel->rowCount()));
        break;

    default:
        break;
    }

    /* 검색 후 id 생성 시, DB에 저장된 마지막 id 값보다 +1 */
    QString filterStr = "id in (";
    for(int i = 0; i < orderModel->rowCount(); i++) {
        int id = orderModel->data(orderModel->index(i, 0)).toInt();
        if(i != orderModel->rowCount()-1)
            filterStr += QString("%1, ").arg(id);
        else
            filterStr += QString("%1);").arg(id);
    }
    orderModel->setFilter(filterStr);
}

/* 총합 계산 슬롯 */
void OrderManagerForm::on_stockLineEdit_textChanged(const QString &stock)
{
    int sum;
    sum = stock.toInt() * ui->priceLineEdit->text().toInt();    //주문수량 * 가격
    ui->sumLineEdit->setText(QString::number(sum));
}

/* 등록된 주문정보 클릭 시 관련정보 출력 슬롯 */
void OrderManagerForm::on_orderTableView_clicked(const QModelIndex &index)
{
    ui->idLineEdit->setText( index.sibling(index.row(), 0).data().toString() );
    ui->cIdLineEdit->setText( index.sibling(index.row(), 1).data().toString() );
    ui->pIdLineEdit->setText( index.sibling(index.row(), 2).data().toString() );
    ui->stockLineEdit->setText( index.sibling(index.row(), 3).data().toString() );
    ui->priceLineEdit->setText( index.sibling(index.row(), 4).data().toString() );
    ui->sumLineEdit->setText( index.sibling(index.row(), 5).data().toString() );
    ui->addressLineEdit->setText( index.sibling(index.row(), 6).data().toString() );
}

/* 주문정보를 담고있는 모델의 모든 데이터 출력 슬롯 */
void OrderManagerForm::on_statePushButton_clicked()
{
    orderModel->setFilter("");
    orderModel->select();
}

