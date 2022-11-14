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
#include <QStandardItemModel>

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

    /* 버튼의 상태변경 */
    ui->clientButton->setEnabled(false);
    ui->productButton->setEnabled(false);

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

        /* 고객Item모델 객체 생성 및 헤더 명 설정*/
        clientItemModel = new QStandardItemModel(0, 3);
        clientItemModel->setHeaderData(0, Qt::Horizontal, tr("ID"));
        clientItemModel->setHeaderData(1, Qt::Horizontal, tr("Name"));
        clientItemModel->setHeaderData(2, Qt::Horizontal, tr("Address"));
        ui->clientItemTreeView->setModel(clientItemModel);      //ui에 표시할 고객Item모델 설정
        ui->clientItemTreeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);

        /* 제품Item모델 객체 생성 및 헤더 명 설정*/
        productItemModel = new QStandardItemModel(0, 4);
        productItemModel->setHeaderData(0, Qt::Horizontal, tr("ID"));
        productItemModel->setHeaderData(1, Qt::Horizontal, tr("Name"));
        productItemModel->setHeaderData(2, Qt::Horizontal, tr("price"));
        productItemModel->setHeaderData(3, Qt::Horizontal, tr("stock"));
        ui->productItemTreeView->setModel(productItemModel);    //ui에 표시할 제품Item모델 설정
        ui->productItemTreeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);

        /* 고객Item 검색 모델 객체 생성 및 헤더 명 설정*/
        searchClientModel = new QStandardItemModel(0, 3);
        searchClientModel->setHeaderData(0, Qt::Horizontal, tr("ID"));
        searchClientModel->setHeaderData(1, Qt::Horizontal, tr("Name"));
        searchClientModel->setHeaderData(2, Qt::Horizontal, tr("Address"));

        /* 고객Item검색 모델 객체 생성 및 헤더 명 설정*/
        searchProductModel = new QStandardItemModel(0, 4);
        searchProductModel->setHeaderData(0, Qt::Horizontal, tr("ID"));
        searchProductModel->setHeaderData(1, Qt::Horizontal, tr("Name"));
        searchProductModel->setHeaderData(2, Qt::Horizontal, tr("price"));
        searchProductModel->setHeaderData(3, Qt::Horizontal, tr("stock"));


        /* 주문 모델 헤더 명 설정 */
        orderModel->setHeaderData(0, Qt::Horizontal, QObject::tr("OrderNum"));
        orderModel->setHeaderData(1, Qt::Horizontal, QObject::tr("고객ID"));
        orderModel->setHeaderData(2, Qt::Horizontal, QObject::tr("ProductID"));
        orderModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Stock"));
        orderModel->setHeaderData(4, Qt::Horizontal, QObject::tr("Price"));
        orderModel->setHeaderData(5, Qt::Horizontal, QObject::tr("Sum"));
        orderModel->setHeaderData(6, Qt::Horizontal, QObject::tr("Address"));

        ui->orderTableView->setModel(orderModel);       //ui에 표시할 주문모델 설정
        ui->orderTableView->horizontalHeader()->setStyleSheet(
                    "QHeaderView { font-weight: bold; };");
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
    Q_UNUSED(phoneNumber);
    /* 고객Item모델에 데이터 추가 */
    QStringList strings;
    strings << QString::number(id) << name << address;
    QList<QStandardItem*> items;
    for (int i = 0; i < 3; i++) {
        items.append(new QStandardItem(strings.at(i)));
    }
    clientItemModel->appendRow(items);
}

/* 고객정보 데이터 제거 */
void OrderManagerForm::delClient(int id)    //고객 id
{
    /* 고객Item모델에서 id와 일치하는 index를 찾아 해당 index의 데이터 제거  */
    QModelIndexList index = clientItemModel->match(clientItemModel->index(0, 0), Qt::EditRole,
                                                   id, -1, Qt::MatchFlags(Qt::MatchFixedString));
    foreach(auto ix, index) {
        clientItemModel->removeRow(ix.row());        //모델의 현재 행 삭제
        ui->clientItemTreeView->update();                //뷰 update
    }
}

/* 고객정보 데이터 변경 */
void OrderManagerForm::modClient(int id, QString name,
                                 QString phoneNumber, QString address)  //고객id, 이름, 전화번호, 주소
{
    /* 고객Item모델에서 id와 일치하는 index를 찾아 해당 index의 데이터 변경  */
    QModelIndexList index = clientItemModel->match(clientItemModel->index(0, 0), Qt::EditRole,
                                                   id, -1, Qt::MatchFlags(Qt::MatchFixedString));
    foreach(auto ix, index) {
        clientItemModel->setItem(ix.row(), 1, new QStandardItem(name));
        clientItemModel->setItem(ix.row(), 2, new QStandardItem(address));
    }
}

/* 제품정보 데이터 추가 */
void OrderManagerForm::updateProduct(int id, QString name,
                                     QString price, QString stock)  //제품id, 이름, 가격, 수량
{
    /* 제품Item모델에 데이터 추가 */
    QStringList strings;
    strings << QString::number(id) << name << price << stock;
    QList<QStandardItem*> items;
    for (int i = 0; i < 4; i++) {
        items.append(new QStandardItem(strings.at(i)));
    }
    productItemModel->appendRow(items);
}

/* 제품정보 데이터 제거 */
void OrderManagerForm::delProduct(int id)
{
    /* 제품아이템모델에서 id와 일치하는 index를 찾아 해당 index의 데이터 제거  */
    QModelIndexList index = productItemModel->match(productItemModel->index(0, 0), Qt::EditRole,
                                                    id, -1, Qt::MatchFlags(Qt::MatchFixedString));
    foreach(auto ix, index) {
        productItemModel->removeRow(ix.row());        //모델의 현재 행 삭제
        ui->productItemTreeView->update();                //뷰 update
    }
}

/* 제품정보 데이터 변경 */
void OrderManagerForm::modProduct(int id, QString name, QString price, QString stock)
{
    /* 제품아이템모델에서 id와 일치하는 index를 찾아 해당 index의 데이터 변경  */
    QModelIndexList index = productItemModel->match(productItemModel->index(0, 0), Qt::EditRole,
                                                    id, -1, Qt::MatchFlags(Qt::MatchFixedString));
    foreach(auto ix, index) {
        productItemModel->setItem(ix.row(), 1, new QStandardItem(name));
        productItemModel->setItem(ix.row(), 2, new QStandardItem(price));
        productItemModel->setItem(ix.row(), 3, new QStandardItem(stock));
    }
}

/* 고객 검색 시, 콤보박스index에 따른 모델 설정 및 버튼상태 변경 */
void OrderManagerForm::on_clientComboBox_activated(int index)
{
    if(index == 0){
        ui->clientItemTreeView->setModel(clientItemModel);
        ui->clientButton->setEnabled(false);
    }
    else ui->clientButton->setEnabled(true);
}

/* 제품 검색 시, 콤보박스index에 따른 모델 설정 및 버튼상태 변경 */
void OrderManagerForm::on_productComboBox_activated(int index)
{
    if(index == 0){
        ui->productItemTreeView->setModel(productItemModel);
        ui->productButton->setEnabled(false);
    }
    else ui->productButton->setEnabled(true);
}

/*고객 id나 이름 검색하는 슬롯 */
void OrderManagerForm::on_clientButton_clicked()
{
    searchClientModel->removeRows(0,searchClientModel->rowCount());

    QString str = ui->clientLineEdit->text();   // 검색할 값은 ui에서 입력한 QString 데이터
    int i = ui->clientComboBox->currentIndex(); //무엇으로 검색할지 콤보박스의 인덱스를 가져옴
    auto flag = (i)? Qt::MatchCaseSensitive|Qt::MatchContains
                   : Qt::MatchCaseSensitive;

    /* id 검색. 고객Item 모델에서 검색하는 데이터와 일치하거나 포함되는 데이터의 인덱스 */
    QModelIndexList idIndex = clientItemModel->match(clientItemModel->index(0, 0), Qt::EditRole,
                                                     str, -1, Qt::MatchFlags(flag));
    /* 이름검색. 고객Item 모델에서 검색하는 데이터와 일치하거나 포함되는 데이터의 인덱스 */
    QModelIndexList nameIndex = clientItemModel->match(clientItemModel->index(0, 1), Qt::EditRole,
                                                       str, -1, Qt::MatchFlags(flag));
    /* 주소검색. 고객Item 모델에서 검색하는 데이터와 일치하거나 포함되는 데이터의 인덱스 */
    QModelIndexList addressIndex = clientItemModel->match(clientItemModel->index(0, 2), Qt::EditRole,
                                                          str, -1, Qt::MatchFlags(flag));
    ui->clientItemTreeView->setModel(searchClientModel);    // 검색 시, 뷰의 세팅은 고객Item모델 -> 고객Item검색모델

    switch (i){
    case 1: //id 검색
        /* 검색한 데이터에 이름이 포함되면 뷰에 검색결과 출력 */
        foreach(auto ix, idIndex) {
            int id = clientItemModel->data(ix.siblingAtColumn(0)).toInt();
            QString name = clientItemModel->data(ix.siblingAtColumn(1)).toString();
            QString address = clientItemModel->data(ix.siblingAtColumn(2)).toString();
            QStringList strings;
            strings << QString::number(id) << name  << address;
            /* 고객Item모델에서 검색값을 받아 고객Item검색 모델로 데이터를 추가 */
            QList<QStandardItem *> items;
            for (int i = 0; i < 3; ++i) {
                items.append(new QStandardItem(strings.at(i)));
            }
            searchClientModel->appendRow(items);
            ui->clientItemTreeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
        }
        break;
    case 2: //이름 검색
        /* 검색한 데이터에 id가 포함되면 뷰에 검색결과 출력*/
        foreach(auto ix, nameIndex) {
            int id = clientItemModel->data(ix.siblingAtColumn(0)).toInt();
            QString name = clientItemModel->data(ix.siblingAtColumn(1)).toString();
            QString address = clientItemModel->data(ix.siblingAtColumn(2)).toString();
            QStringList strings;
            strings << QString::number(id) << name  << address;
            /* 고객Item모델에서 검색값을 받아 고객Item검색 모델로 데이터를 추가 */
            QList<QStandardItem *> items;
            for (int i = 0; i < 3; ++i) {
                items.append(new QStandardItem(strings.at(i)));
            }
            searchClientModel->appendRow(items);
            ui->clientItemTreeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
        }
        break;
    case 3: //주소 검색
        foreach(auto ix, addressIndex) {
            int id = clientItemModel->data(ix.siblingAtColumn(0)).toInt();
            QString name = clientItemModel->data(ix.siblingAtColumn(1)).toString();
            QString address = clientItemModel->data(ix.siblingAtColumn(2)).toString();
            QStringList strings;
            strings << QString::number(id) << name  << address;
            /* 고객Item모델에서 검색값을 받아 고객Item검색 모델로 데이터를 추가 */
            QList<QStandardItem *> items;
            for (int i = 0; i < 3; ++i) {
                items.append(new QStandardItem(strings.at(i)));
            }
            searchClientModel->appendRow(items);
            ui->clientItemTreeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
        }
        break;

    default:
        break;
    }

}

/* 제품 id나 이름 검색하는 슬롯 */
void OrderManagerForm::on_productButton_clicked()
{
    searchProductModel->removeRows(0,searchProductModel->rowCount()); //중복 방지

    QString str = ui->productLineEdit->text();   // 검색할 값은 ui에서 입력한 QString 데이터
    int i = ui->productComboBox->currentIndex(); //무엇으로 검색할지 콤보박스의 인덱스를 가져옴
    auto flag = (i)? Qt::MatchCaseSensitive|Qt::MatchContains
                   : Qt::MatchCaseSensitive;
    /* 제품Item 모델에서 검색하는 데이터와 일치하거나 포함되는 데이터의 인덱스 */
    QModelIndexList idindex = productItemModel->match(productItemModel->index(0, 0), Qt::EditRole,
                                                      str, -1, Qt::MatchFlags(flag));
    /* 제품Item 모델에서 검색하는 데이터가 일치하거나 포함되는 데이터의 인덱스 */
    QModelIndexList nameindex = productItemModel->match(productItemModel->index(0, 1), Qt::EditRole,
                                                        str, -1, Qt::MatchFlags(flag));

    ui->productItemTreeView->setModel(searchProductModel);  // 검색 시, 뷰의 세팅은 제품 Item모델 -> 제품 Item검색모델
    switch (i){
    case 1: //id 검색
        /* 검색한 데이터와 id가 일치하면 뷰에 검색결과 출력 */
        foreach(auto ix, idindex) {
            int id = productItemModel->data(ix.siblingAtColumn(0)).toInt();
            QString name = productItemModel->data(ix.siblingAtColumn(1)).toString();
            QString price = productItemModel->data(ix.siblingAtColumn(2)).toString();
            QString stock = productItemModel->data(ix.siblingAtColumn(3)).toString();
            /* 제품Item모델에서 검색값을 받아 제품Item검색 모델로 데이터를 추가 */
            QStringList strings;
            strings << QString::number(id) << name  << price << stock;
            QList<QStandardItem *> items;
            for (int i = 0; i < 4; ++i) {
                items.append(new QStandardItem(strings.at(i)));
            }

            searchProductModel->appendRow(items);
            ui->productItemTreeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
        }
        break;
    case 2: //이름 검색
        /* 검색한 데이터에 id가 포함되면 뷰에 검색결과 출력 */
        foreach(auto ix, nameindex) {
            int id = productItemModel->data(ix.siblingAtColumn(0)).toInt();
            QString name = productItemModel->data(ix.siblingAtColumn(1)).toString();
            QString price = productItemModel->data(ix.siblingAtColumn(2)).toString();
            QString stock = productItemModel->data(ix.siblingAtColumn(3)).toString();
            /* 제품Item모델에서 검색값을 받아 제품Item검색 모델로 데이터를 추가 */
            QStringList strings;
            strings << QString::number(id) << name  << price << stock;
            QList<QStandardItem *> items;
            for (int i = 0; i < 4; ++i) {
                items.append(new QStandardItem(strings.at(i)));
            }

            searchProductModel->appendRow(items);
            ui->productItemTreeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
        }
        break;

    default:
        break;
    }
}


/* 고객 데이터 클릭 시 LineEdit에 입력하는 슬롯 */
void OrderManagerForm::on_clientItemTreeView_clicked(const QModelIndex &index)
{
    /* 클릭한 Index의 데이터를 가져옴 */
    QString client;
    client = index.sibling(index.row(), 0).data().toString();
    ui->cIdLineEdit->setText(client);
    ui->addressLineEdit->setText(index.sibling(index.row(), 2).data().toString() );
}

/* 제품 데이터 클릭 시 LineEdit에 입력하는 슬롯 */
void OrderManagerForm::on_productItemTreeView_clicked(const QModelIndex &index)
{
    /* 클릭한 Index의 데이터를 가져옴 */
    QString product;
    product = index.sibling(index.row(), 0).data().toString();

    ui->pIdLineEdit->setText(product);
    ui->priceLineEdit->setText(index.sibling(index.row(), 2).data().toString() );
}

/* 주문 정보 추가 */
void OrderManagerForm::on_addPushButton_clicked()
{
    auto flag = Qt::MatchFixedString;   //완전일치
    /* 제품id index. 제품Item 모델에서 검색하는 데이터와 일치하는 데이터의 인덱스 */
    QModelIndexList index = productItemModel->match(productItemModel->index(0, 0), Qt::EditRole,
                                                    ui->pIdLineEdit->text(), -1, Qt::MatchFlags(flag));
    /* 제품id index. 제품Item 검색모델에서 검색하는 데이터와 일치하는 데이터의 인덱스 */
    QModelIndexList srcIndex = searchProductModel->match(searchProductModel->index(0, 0), Qt::EditRole,
                                                         ui->pIdLineEdit->text(), -1, Qt::MatchFlags(flag));
    QString client, product, stock, price, address, sum;    //고객, 제품, 수량, 가격, 주소, 총합
    QString productKey, productTree_Stock;                  //제품id, 주문 수량

    /* 추가 시 LineEdit에 있는 고객, 제품, 수량, 가격, 주소, 총 합의 데이터 저장 */
    int id = makeId( );
    ui->idLineEdit->setText(QString::number(id));
    client = ui->cIdLineEdit->text();
    product = ui->pIdLineEdit->text();
    stock = ui->stockLineEdit->text();
    price = ui->priceLineEdit->text();
    address = ui->addressLineEdit->text();
    sum = ui->sumLineEdit->text();

    /* 제품의 재고를 가져오기 위한 foreach 문 */
    foreach(auto ix, index) {
        productTree_Stock = productItemModel->data(ix.siblingAtColumn(3)).toString();
    }
    productKey =  ui->pIdLineEdit->text();   //제품 id
    /* 재고반영을 위한 시그널 */
    emit productAddKeySent(productKey.toInt(), stock ); //제품id, 주문수량

    /* 주문 수량이 재고보다 많을 경우 예외 */
    if(productTree_Stock.toInt() < ui->stockLineEdit->text().toInt()) return;

    /* 주문이 되면 제품리스트의 제품Item모델의 재고를 변경 */
    QString result_stock = QString::number(productTree_Stock.toInt() - stock.toInt());
    foreach(auto ix, index) {
        productItemModel->setItem(ix.row(), 3, new QStandardItem(result_stock));
    }
    /* 주문이 되면 제품리스트의 제품Item검색모델의 재고를 변경 */
    foreach(auto ix, srcIndex){
        searchProductModel->setItem(ix.row(), 3, new QStandardItem(result_stock));
    }

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
    QModelIndex index = ui->orderTableView->currentIndex();     //변경하고자 하는 주문정보데이터의 index
    if(index.isValid()) {
        QString client, product, stock, price, address, sum;    //고객, 제품, 수량, 가격, 주소, 총합
        QString productKey, orderStock; //제품 id, 주문수량
        int result, beInStock;          //변경가능 재고량, 제품리스트의 재고 + 주문되어있는 재고

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
            productKey = query.value(0).toString(); // 제품id
            orderStock = query.value(1).toString();         // 입력된 수량 (주문되어있는 재고)
        }
        /* 제품id index. 제품Item 모델에서 검색하는 데이터와 일치하는 데이터의 인덱스 */
        QModelIndexList index = productItemModel->match(productItemModel->index(0, 0), Qt::EditRole,
                                                        ui->pIdLineEdit->text(), -1, Qt::MatchFlags(Qt::MatchFixedString));
        /* 제품id index. 제품Item 검색모델에서 검색하는 데이터와 일치하는 데이터의 인덱스 */
        QModelIndexList srcIndex = searchProductModel->match(searchProductModel->index(0, 0), Qt::EditRole,
                                                             ui->pIdLineEdit->text(), -1, Qt::MatchFlags(Qt::MatchFixedString));
        /* 주문 변경 시, 제품id가 일치하는 제품Item모델의 재고 변경 */
        foreach(auto ix, index) {
            QString productStock = productItemModel->data(ix.siblingAtColumn(3)).toString();            //제품리스트의 재고
            result = productStock.toInt() +orderStock.toInt() - ui->stockLineEdit->text().toInt();  //변경 가능한 재고량
            beInStock = productStock.toInt() +orderStock.toInt();   //제품리스트의 재고 + 주문되어있는 재고
            /* 주문변경 가능 수량에 대한 예외처리 */
            if( beInStock< ui->stockLineEdit->text().toInt()){
                QMessageBox::information(this, tr("Error"),
                                         QString(tr("out of stock\nYou can change up to %0."))
                                         .arg(beInStock));
                return;
            }
            productItemModel->setItem(ix.row(), 3, new QStandardItem(QString::number(result)));
        }
        /* 주문 변경 시, 제품id가 일치하는 제품Item검색모델의 재고 변경 */
        foreach(auto ix, srcIndex) {
            QString productStock = searchProductModel->data(ix.siblingAtColumn(3)).toString();            //제품리스트의 재고
            result = productStock.toInt() +orderStock.toInt() - ui->stockLineEdit->text().toInt();  //변경 가능한 재고량
            beInStock = productStock.toInt() +orderStock.toInt();   //제품리스트의 재고 + 주문되어있는 재고
            /* 주문변경 가능 수량에 대한 예외처리 */
            if( beInStock< ui->stockLineEdit->text().toInt()){
                QMessageBox::information(this, tr("Error"),
                                         QString(tr("out of stock\nYou can change up to %0."))
                                         .arg(beInStock));
                return;
            }
            searchProductModel->setItem(ix.row(), 3, new QStandardItem(QString::number(result)));
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
    QString productKey, productStock ,delStock;             //제품id, 주문제거한 수량
    int result; //반영할 재고량

    QModelIndex index = ui->orderTableView->currentIndex(); //주문정보에서 제거하고자 하는 데이터의 index

    productKey = index.sibling(index.row(), 2).data().toString();   //제거하고자 하는 제품 id
    delStock = index.sibling(index.row(), 3).data().toString();             //제거하는 제품의 주문수량

    /* 제품id index. 제품Item 모델에서 검색하는 데이터와 일치하는 데이터의 인덱스 */
    QModelIndexList indexes = productItemModel->match(productItemModel->index(0, 0), Qt::EditRole,
                                                      productKey, -1, Qt::MatchFlags(Qt::MatchFixedString));
    foreach(auto ix, indexes) {
        productStock = productItemModel->data(ix.siblingAtColumn(3)).toString();    //제품리스트의 재고
        result = productStock.toInt() + delStock.toInt();                           //주문취소 후 재고량
        productItemModel->setItem(ix.row(), 3, new QStandardItem(QString::number(result)));
    }
    /* 제품id index. 제품Item 검색모델에서 검색하는 데이터와 일치하는 데이터의 인덱스 */
    QModelIndexList srcIndex = searchProductModel->match(searchProductModel->index(0, 0), Qt::EditRole,
                                                         productKey, -1, Qt::MatchFlags(Qt::MatchFixedString));
    foreach(auto ix, srcIndex) {
        productStock = searchProductModel->data(ix.siblingAtColumn(3)).toString();  //제품리스트의 재고
        result = productStock.toInt() + delStock.toInt();                           //주문취소 후 재고량
        searchProductModel->setItem(ix.row(), 3, new QStandardItem(QString::number(result)));
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
        /* 검색한 데이터와 id가 포함되거나 일치하면 뷰에 검색결과 출력 후 메시지박스 */
        orderModel->setFilter(QString("id LIKE '%%1%'").arg(searchValue));
        orderModel->select();
        QMessageBox::information(this, tr("Search Info"),
                                 QString( tr("%1 search results were found") ).arg(orderModel->rowCount()));
        break;
    case 1: //client 검색
        /* 검색한 데이터에 client가 포함되거나 일치하면 뷰에 검색결과 출력 후 메시지박스 */
        orderModel->setFilter(QString("client LIKE '%%1%'").arg(searchValue));
        orderModel->select();
        QMessageBox::information(this, tr("Search Info"),
                                 QString( tr("%1 search results were found") ).arg(orderModel->rowCount()));
        break;
    case 2: //product 검색
        /* 검색한 데이터에 product이 포함되거나 일치하면 뷰에 검색결과 출력 후 메시지박스 */
        orderModel->setFilter(QString("product LIKE '%%1%'").arg(searchValue));
        orderModel->select();
        QMessageBox::information(this, tr("Search Info"),
                                 QString( tr("%1 search results were found") ).arg(orderModel->rowCount()));
        break;

    default:
        break;
    }

    /* 검색 후 id 생성 시, 주문모델에 저장된 마지막 id 값보다 +1 */
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
    searchClientModel->removeRows(0,searchClientModel->rowCount());         //중복 방지
    searchProductModel->removeRows(0,searchProductModel->rowCount());       //중복 방지

    ui->clientComboBox->setCurrentIndex(4);     //콤보박스의 상태 변경
    ui->productComboBox->setCurrentIndex(3);

    QString clientID = index.sibling(index.row(), 1).data().toString();
    QString productID = index.sibling(index.row(), 2).data().toString();

    /* id 검색. 고객Item 모델에서 검색하는 데이터와 일치하거나 포함되는 데이터의 인덱스 */
    QModelIndexList cIDIndex = clientItemModel->match(clientItemModel->index(0, 0), Qt::EditRole,
                                                     clientID, -1, Qt::MatchFlags(Qt::MatchCaseSensitive
                                                                             |Qt::MatchContains));

    /* id 검색. 고객Item 모델에서 검색하는 데이터와 일치하거나 포함되는 데이터의 인덱스 */
    QModelIndexList pIDIndex = productItemModel->match(productItemModel->index(0, 0), Qt::EditRole,
                                                     productID, -1, Qt::MatchFlags(Qt::MatchCaseSensitive
                                                                             |Qt::MatchContains));
    ui->clientItemTreeView->setModel(searchClientModel);    // 클릭 시, 뷰의 세팅은 고객Item모델 -> 고객Item검색모델
    ui->productItemTreeView->setModel(searchProductModel);    // 클릭 시, 뷰의 세팅은 고객Item모델 -> 고객Item검색모델

    foreach(auto ix, cIDIndex) {
        int id = clientItemModel->data(ix.siblingAtColumn(0)).toInt();
        QString name = clientItemModel->data(ix.siblingAtColumn(1)).toString();
        QString address = clientItemModel->data(ix.siblingAtColumn(2)).toString();
        QStringList strings;
        strings << QString::number(id) << name  << address;
        /* 고객Item모델에서 검색값을 받아 고객Item검색 모델로 데이터를 추가 */
        QList<QStandardItem *> items;
        for (int i = 0; i < 3; ++i) {
            items.append(new QStandardItem(strings.at(i)));
        }
        searchClientModel->appendRow(items);
        ui->clientItemTreeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    }

    foreach(auto ix, pIDIndex) {
        int id = productItemModel->data(ix.siblingAtColumn(0)).toInt();
        QString name = productItemModel->data(ix.siblingAtColumn(1)).toString();
        QString price = productItemModel->data(ix.siblingAtColumn(2)).toString();
        QString stock = productItemModel->data(ix.siblingAtColumn(3)).toString();
        /* 제품Item모델에서 검색값을 받아 제품Item검색 모델로 데이터를 추가 */
        QStringList strings;
        strings << QString::number(id) << name  << price << stock;
        QList<QStandardItem *> items;
        for (int i = 0; i < 4; ++i) {
            items.append(new QStandardItem(strings.at(i)));
        }

        searchProductModel->appendRow(items);
        ui->productItemTreeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    }

    ui->idLineEdit->setText(index.sibling(index.row(), 0).data().toString());
    ui->cIdLineEdit->setText(clientID);
    ui->pIdLineEdit->setText(productID);
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

