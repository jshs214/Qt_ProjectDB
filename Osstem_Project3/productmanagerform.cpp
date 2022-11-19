#include "productmanagerform.h"
#include "qsqltablemodel.h"
#include "ui_productmanagerform.h"

#include <QFile>
#include <QMenu>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QMessageBox>

ProductManagerForm::ProductManagerForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProductManagerForm)
{
    ui->setupUi(this);
    /* ContextMenu의 remove 액션, 리스트의 데이터 삭제 */
    QAction* removeAction = new QAction(tr("&Remove"));
    removeAction->setIcon(QIcon(":/images/eraser.png"));
    connect(removeAction, SIGNAL(triggered()), SLOT(removeItem()));
    menu = new QMenu;
    menu->addAction(removeAction);
    ui->productTableView->setContextMenuPolicy(Qt::CustomContextMenu);

    /* 테이블 뷰에 컨텍스트메뉴 사용한 시그널, 슬롯 */
    connect(ui->productTableView, SIGNAL(customContextMenuRequested(QPoint)), this,
            SLOT(showContextMenu(QPoint)));

    /* 엔터키로 버튼 클릭 가능하게 슬롯 연결 */
    connect(ui->searchLineEdit, SIGNAL(returnPressed()),
            this, SLOT(on_searchPushButton_clicked()));

    /* 가격 입력 시 0~99999999, 재고 입력 시 0~9999 숫자만 받도록 */
    ui->priceLineEdit->setValidator( new QIntValidator(0, 99999999, this) );    //숫자만 받도록
    ui->stockLineEdit->setValidator( new QIntValidator(0, 9999, this) );        //숫자만 받도록

}

/* 소멸자에서 DB close */
ProductManagerForm::~ProductManagerForm()
{
    delete ui;
    /* 제품 DB close  */
    QSqlDatabase db = QSqlDatabase::database("productConnection");
    if(db.isOpen()) {
        productModel->submitAll();
        db.close();
    }
}

/* 제품정보 DB의 데이터를 불러오는 메서드 */
void ProductManagerForm::loadData()
{
    /*  Qt에서 지원하는 데이터베이스 드라이버 QSQLITE에 제품 DB 객체 선언  */
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "productConnection");
    db.setDatabaseName("product.db");   // DB명은 product.db
    if (db.open( )) {
        QSqlQuery query(db);            // db 를 사용하여 QSqlQuery 객체를 생성
        /* SQL 쿼리문을 사용해 제품 테이블 생성 */
        query.exec("CREATE TABLE IF NOT EXISTS productList(id INTEGER Primary Key, name VARCHAR(30) NOT NULL,"
                   " price VARCHAR(20) NOT NULL, stock VARCHAR(20), type VARCHAR(20));");

        productModel = new QSqlTableModel(this,db); //QSqlTableModel을 이용해 고객모델 객체 생성
        productModel->setTable("productList");      //모델이 작동하는 DB 테이블 설정
        productModel->select();                     //모델의 데이터 조회

        /* 제품모델의 헤더 명 설정 */
        productModel->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
        productModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Name"));
        productModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Price"));
        productModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Stock"));
        productModel->setHeaderData(4, Qt::Horizontal, QObject::tr("Type"));

        ui->productTableView->setModel(productModel);   //ui에 표시할 제품모델 설정
        ui->productTableView->horizontalHeader()->setStyleSheet(
                    "QHeaderView { font-weight: bold; };");
    }
    /* 제품정보 데이터를 불러 올 때, 주문정보에 불러온 데이터를 시그널을 통해 전송 */
    for(int i = 0; i < productModel->rowCount(); i++) {
        int id = productModel->data(productModel->index(i, 0)).toInt();
        QString name = productModel->data(productModel->index(i, 1)).toString();
        QString price = productModel->data(productModel->index(i, 2)).toString();
        QString stock = productModel->data(productModel->index(i, 3)).toString();

        /* 제품정보데이터를 주문정보관리에 정보 전송 시그널 */
        emit productAddToOrder(id, name, price, stock); //id, 이름, 가격, 재고
    }
}

/* 고객 키값 생성 메서드 */
int ProductManagerForm::makeId( )
{
    /* 키값 생성 시, 중복 되지 않게 모델에 존재하는 키값의 마지막 +1 하여 생성  */
    if(productModel->rowCount() == 0) {
        return 50000;
    } else {
        auto id = productModel->data(productModel->index(productModel->rowCount()-1, 0)).toInt();
        return ++id;
    }
}

/* ContextMenu 슬롯 */
void ProductManagerForm::showContextMenu(const QPoint &pos)
{
    QPoint globalPos = ui->productTableView->mapToGlobal(pos);
    if(ui->productTableView->indexAt(pos).isValid())
        menu->exec(globalPos);
}


/* 버튼 클릭 시 입력 값 초기화 하는 슬롯 */
void ProductManagerForm::on_clearButton_clicked()
{
    ui->idLineEdit->clear();
    ui->nameLineEdit->clear();
    ui->priceLineEdit->clear();
    ui->stockLineEdit->clear();
    ui->searchLineEdit->clear();
    ui->typeLineEdit->clear();
}

/* 제품정보추가를 위한 슬롯 */
void ProductManagerForm::on_addPushButton_clicked()
{
    /* 추가 시 변수에 입력했던 id, 이름, 전화번호, 주소의 데이터 저장 */
    QString name, price, stock, type;
    int id = makeId( );
    ui->idLineEdit->setText(QString::number(id));
    name = ui->nameLineEdit->text();
    price = ui->priceLineEdit->text();
    stock = ui->stockLineEdit->text();
    type = ui->typeLineEdit->text();
    /* 추가할 제품의 정보를 다 입력하면 제품 정보추가 */
    if(name.length() && price.length() && stock.length()) {
        QSqlQuery query(productModel->database());  //QSqlQuery 객체(제품모델)
        query.prepare("INSERT INTO productList VALUES (?, ?, ?, ?, ?)");       //sql쿼리문 준비
        /* sql쿼리문에 값 바인딩 */
        query.bindValue(0, id);
        query.bindValue(1, name);
        query.bindValue(2, price);
        query.bindValue(3, stock);
        query.bindValue(4, type);

        query.exec();              //sql쿼리문 실행
        productModel->select();    //모델의 데이터 조회

        /* 추가한 제품정보데이터를 주문정보관리에 정보 전송 시그널 */
        emit productAddToOrder(id, name, price, stock); //id, 이름, 가격, 수량
    }

    /* ui의 LineEdit 값 clear */
    ui->idLineEdit->clear();
    ui->nameLineEdit->clear();
    ui->priceLineEdit->clear();
    ui->stockLineEdit->clear();
    ui->typeLineEdit->clear();
}

/* 제품정보변경을 위한 슬롯 */
void ProductManagerForm::on_modifyPushButton_clicked()
{
    QModelIndex index = ui->productTableView->currentIndex();   //변수에 뷰의 현재 index값 저장

    if(index.isValid()) {
        /* id는 모델의 현재 데이터값, QString 변수는 변경 할 이름, 가격, 수량의 데이터 저장 */
        int id = productModel->data(index.siblingAtColumn(0)).toInt();
        QString name, price, stock, type;
        name = ui->nameLineEdit->text();
        price = ui->priceLineEdit->text();
        stock = ui->stockLineEdit->text();
        type = ui->typeLineEdit->text();

        QSqlQuery query(productModel->database());  //QSqlQuery 객체(제품모델)
        query.prepare("UPDATE productList SET name = ?,"
                      " price = ?, stock = ?, type = ? WHERE id = ?");     //sql쿼리문 준비
        /* sql쿼리문에 값 바인딩 */
        query.bindValue(0, name);
        query.bindValue(1, price);
        query.bindValue(2, stock);
        query.bindValue(3, type);
        query.bindValue(4, id);

        query.exec();               //sql 쿼리 실행
        productModel->select();     //모델의 데이터 조회

        /* 변경한 제품정보데이터를 주문정보관리에 정보 전송 시그널 */
        emit productModToOrder(id, name, price, stock); //id, 이름, 가격, 수량
    }
}

/* 제품정보의 데이터(트리위젯)의 리스트 제거 슬롯 */
void ProductManagerForm::removeItem()
{
    QModelIndex index = ui->productTableView->currentIndex();       //변수에 뷰의 현재 index값 저장

    int sendId =index.sibling(index.row(), 0).data().toString().toInt();    //현재 index의 id

    /* 삭제한 제품ID를 주문정보관리에 전송 시그널 */
    emit productDelToOrder(sendId);

    QSqlQuery query(productModel->database());   //QSqlQuery 객체(고객모델)
    query.prepare("DELETE FROM productList WHERE id = ?"); //sql쿼리문 준비
    /* sql쿼리문에 값 바인딩 */
    query.bindValue(0, QString::number(sendId));
    query.exec();           //sql 쿼리 실행
    productModel->select();  //모델의 데이터 조회
}

/* 고객정보검색을 위한 슬롯 */
void ProductManagerForm::on_searchPushButton_clicked()
{
    QString searchValue = ui->searchLineEdit->text();   //검색할 데이터

    int i = ui->searchComboBox->currentIndex();         //무엇으로 검색할지 콤보박스의 인덱스를 가져옴
    switch (i){
    case 0: //id 검색
        /* 검색한 데이터와 id가 일치하거나 포함되면 뷰에 검색결과 출력 후 메시지박스 */
        productModel->setFilter(QString("id LIKE '%%1%'").arg(searchValue));
        productModel->select();
        QMessageBox::information(this, tr("Search Info"),
                                 QString( tr("%1 search results were found") ).arg(productModel->rowCount()));
        break;
    case 1: //이름 검색
        /* 검색한 데이터에 이름이 일치하거나 포함되면 뷰에 검색결과 출력 후 메시지박스 */
        productModel->setFilter(QString("name LIKE '%%1%'").arg(searchValue));
        productModel->select();
        QMessageBox::information(this, tr("Search Info"),
                                 QString( tr("%1 search results were found") ).arg(productModel->rowCount()));
        break;
    case 2: //가격 검색
        /* 검색한 데이터에 가격이 일치하거나 포함되면 뷰에 검색결과 출력 후 메시지박스 */
        productModel->setFilter(QString("price LIKE '%%1%'").arg(searchValue));
        productModel->select();
        QMessageBox::information(this, tr("Search Info"),
                                 QString( tr("%1 search results were found") ).arg(productModel->rowCount()));
        break;
    case 3: //수량 검색
        /* 검색한 데이터에 수량이 일치하거나 포함되면 뷰에 검색결과 출력 후 메시지박스 */
        productModel->setFilter(QString("stock LIKE '%%1%'").arg(searchValue));
        productModel->select();
        QMessageBox::information(this, tr("Search Info"),
                                 QString( tr("%1 search results were found") ).arg(productModel->rowCount()));
        break;
    case 4: //타입 검색
        /* 검색한 데이터에 수량이 포함되면 뷰에 검색결과 출력 후 메시지박스 */
        productModel->setFilter(QString("type LIKE '%%1%'").arg(searchValue));
        productModel->select();
        QMessageBox::information(this, tr("Search Info"),
                                 QString( tr("%1 search results were found") ).arg(productModel->rowCount()));
        break;
    default:
        break;
    }
    /* 검색 후 id 생성 시, DB에 저장된 마지막 id 값보다 +1 */
    QString filterStr = "id in (";
    for(int i = 0; i < productModel->rowCount(); i++) {
        int id = productModel->data(productModel->index(i, 0)).toInt();
        if(i != productModel->rowCount()-1)
            filterStr += QString("%1, ").arg(id);
        else
            filterStr += QString("%1);").arg(id);
    }
    productModel->setFilter(filterStr);
}


/* 등록된 제품정보 클릭 시 관련정보 출력 슬롯 */
void ProductManagerForm::on_productTableView_clicked(const QModelIndex &index)
{
    /* 뷰의 데이터 클릭 시, ui->LineEdit에 고객정보 출력 */
    ui->idLineEdit->setText( index.sibling(index.row(), 0).data().toString() );
    ui->nameLineEdit->setText( index.sibling(index.row(), 1).data().toString() );
    ui->priceLineEdit->setText( index.sibling(index.row(), 2).data().toString() );
    ui->stockLineEdit->setText( index.sibling(index.row(), 3).data().toString() );
    ui->typeLineEdit->setText( index.sibling(index.row(), 4).data().toString() );
}

/* 제품정보를 담고있는 모델의 모든 데이터 출력 슬롯 */
void ProductManagerForm::on_statePushButton_clicked()
{
    productModel->setFilter("");
    productModel->select();     //모델의 데이터 조회
}

/* Order에서 주문추가 시, 재고반영을 위한 슬롯 */
void ProductManagerForm::receiveAddStock(int key, QString inStock) //id, 주문수량
{
    QSqlQuery query(productModel->database());  //QSqlQuery 객체(제품모델)
    int stock, result;  //기존수량, 반영할 재고수량

    query.prepare("SELECT stock FROM productList WHERE id = ?");    //sql쿼리문 준비
    query.bindValue(0, key);    //sql쿼리문에 값 바인딩
    query.exec();               //sql 쿼리 실행
    while (query.next()) {
        stock = query.value(0).toInt();
    }

    result = stock - inStock.toInt();   //반영할 재고 수량 = 기존수량 - 주문수량

    /* 주문 시, 재고부족을 위한 메시지 박스 */
    if(stock < inStock.toInt()) {
        QMessageBox::information(this, tr("Error"),
                                 QString( tr("out of stock\nYou can order up to %0.") )
                                 .arg(stock));
        return;
    }

    query.prepare("UPDATE productList SET stock = ? WHERE id = ?"); //sql쿼리문 준비
    /* sql쿼리문에 값 바인딩 */
    query.bindValue(0, result);
    query.bindValue(1, key);

    query.exec();               //sql 쿼리 실행
    productModel->select();     //모델의 데이터 조회
}

/* Order에서 주문변경 시, 재고반영을 위한 슬롯 */
void ProductManagerForm::receiveModStock(int key, QString updateStock,
                                         QString orderStock)   // 키값, 수정할 수량, 주문 되어있는 수량
{
    QSqlQuery query(productModel->database());  //QSqlQuery 객체(제품모델)
    int stock, result;  //기존수량, 반영할 재고수량

    // 주문 전 기존 재고 조회
    query.prepare("SELECT stock FROM productList WHERE id = ?");    //sql쿼리문 준비
    query.bindValue(0, key);    //sql쿼리문에 값 바인딩
    query.exec();               //sql 쿼리 실행
    while (query.next()) {
        stock = query.value(0).toInt();
    }

    result = stock + orderStock.toInt() - updateStock.toInt();  //반영할 재고 수량 = 현재재고 + 주문수량 - 변경할 주문수량

    query.prepare("UPDATE productList SET stock = ? WHERE id = ?");     //sql쿼리문 준비
    /* sql쿼리문에 값 바인딩 */
    query.bindValue(0, result);
    query.bindValue(1, key);
    query.exec();   //sql 쿼리 실행
    productModel->select();     //모델의 데이터 조회
}

/* Order에서 주문내역제거 시, 재고반영을 위한 슬롯 */
void ProductManagerForm::receiveDelStock(int key, QString delStock) //제품 id, 주문취소수량
{
    QSqlQuery query(productModel->database());  //QSqlQuery 객체(제품모델)
    int stock, result;  //기존수량, 반영할 재고수량

    // 주문 전 기존 재고 조회
    query.prepare("SELECT stock FROM productList WHERE id = ?");    //sql쿼리문 준비
    query.bindValue(0, key);    //sql쿼리문에 값 바인딩
    query.exec();               //sql 쿼리 실행
    while (query.next()) {
        stock = query.value(0).toInt();
    }

    result = stock + delStock.toInt();  //반영할 재고 수량 = 현재재고 + 주문취소 수량

    query.prepare("UPDATE productList SET stock = ? WHERE id = ?"); //sql쿼리문 준비
    /* sql쿼리문에 값 바인딩 */
    query.bindValue(0, result);
    query.bindValue(1, key);
    query.exec();   //sql 쿼리 실행
    productModel->select();     //모델의 데이터 조회
}
