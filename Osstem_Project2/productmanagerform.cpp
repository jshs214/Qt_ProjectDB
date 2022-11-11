#include "productmanagerform.h"
#include "qsqlerror.h"
#include "ui_productmanagerform.h"

#include <QFile>
#include <QMenu>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QMessageBox>
#include <QSqlError>

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

    connect(ui->productTableView, SIGNAL(customContextMenuRequested(QPoint)), this,
            SLOT(showContextMenu(QPoint)));

    connect(ui->searchLineEdit, SIGNAL(returnPressed()),
            this, SLOT(on_searchPushButton_clicked()));

    /* 가격 입력 시 0~99999999, 재고 입력 시 0~9999 숫자만 받도록 */
    ui->priceLineEdit->setValidator( new QIntValidator(0, 99999999, this) );    //숫자만 받도록
    ui->stockLineEdit->setValidator( new QIntValidator(0, 9999, this) );        //숫자만 받도록


}



void ProductManagerForm::loadData()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "productConnection");
    db.setDatabaseName("product.db");
    if (db.open( )) {
        QSqlQuery query(db);
        query.exec("CREATE TABLE IF NOT EXISTS productList(id INTEGER Primary Key, name VARCHAR(30) NOT NULL,"
                   " price VARCHAR(20) NOT NULL, stock VARCHAR(50));");

        productModel = new QSqlTableModel(this,db);
        productModel->setTable("productList");
        productModel->select();
        productModel->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
        productModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Name"));
        productModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Price"));
        productModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Stock"));

        ui->productTableView->setModel(productModel);


    }

    for(int i = 0; i < productModel->rowCount(); i++) {
        int id = productModel->data(productModel->index(i, 0)).toInt();
        QString name = productModel->data(productModel->index(i, 1)).toString();
        QString price = productModel->data(productModel->index(i, 2)).toString();
        QString stock = productModel->data(productModel->index(i, 3)).toString();

        emit productAddToOrder(id, name, price, stock);
    }
}


/* 소멸자에서 파일 출력 */
ProductManagerForm::~ProductManagerForm()
{
    delete ui;

    QSqlDatabase db = QSqlDatabase::database("productConnection");
    if(db.isOpen()) {
        productModel->submitAll();
        db.close();
    }
}

int ProductManagerForm::makeId( )
{
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

/* 제품정보의 데이터(트리위젯)의 리스트 제거 슬롯 */
void ProductManagerForm::removeItem()
{
    QModelIndex index = ui->productTableView->currentIndex();
    int sendId =index.sibling(index.row(), 0).data().toString().toInt();
    emit productDelToOrder(sendId);

    if(index.isValid()) {
        productModel->removeRow(index.row());
        productModel->select();
        ui->productTableView->update();
    }
}



/* 버튼 클릭 시 입력 값 초기화 하는 슬롯 */
void ProductManagerForm::on_clearButton_clicked()
{
    ui->idLineEdit->clear();
    ui->nameLineEdit->clear();
    ui->priceLineEdit->clear();
    ui->stockLineEdit->clear();
    ui->searchLineEdit->clear();
}

void ProductManagerForm::on_addPushButton_clicked()
{
    QString name, price, stock;
    int id = makeId( );
    ui->idLineEdit->setText(QString::number(id));
    name = ui->nameLineEdit->text();
    price = ui->priceLineEdit->text();
    stock = ui->stockLineEdit->text();
    if(name.length()) {
        QSqlQuery query(productModel->database());
        query.prepare("INSERT INTO productList VALUES (?, ?, ?, ?)");
        query.bindValue(0, id);
        query.bindValue(1, name);
        query.bindValue(2, price);
        query.bindValue(3, stock);
        query.exec();
        productModel->select();
        emit productAddToOrder(id, name, price, stock);
    }

    ui->idLineEdit->clear();
    ui->nameLineEdit->clear();
    ui->priceLineEdit->clear();
    ui->stockLineEdit->clear();
}

// 변경
void ProductManagerForm::on_modifyPushButton_clicked()
{
    QModelIndex index = ui->productTableView->currentIndex();
    if(index.isValid()) {
        int id = productModel->data(index.siblingAtColumn(0)).toInt();
        QString name, price, stock;
        name = ui->nameLineEdit->text();
        price = ui->priceLineEdit->text();
        stock = ui->stockLineEdit->text();

        QSqlQuery query(productModel->database());
        query.prepare("UPDATE productList SET name = ?, price = ?, stock = ? WHERE id = ?");
        query.bindValue(0, name);
        query.bindValue(1, price);
        query.bindValue(2, stock);
        query.bindValue(3, id);
        query.exec();
        productModel->select();
        emit productModToOrder(id, name, price, stock);
    }
}

void ProductManagerForm::on_statePushButton_clicked()
{
    productModel->setFilter("");
    productModel->select();
}


void ProductManagerForm::on_searchPushButton_clicked()
{
    QString searchValue = ui->searchLineEdit->text();

    int i = ui->searchComboBox->currentIndex(); //무엇으로 검색할지 콤보박스의 인덱스를 가져옴
    switch (i){
    case 0:
        productModel->setFilter(QString("id = '%1'").arg(searchValue));
        productModel->select();
        QMessageBox::information(this, tr("Search Info"),
                                 QString( tr("%1 search results were found") ).arg(productModel->rowCount()));
        break;
    case 1:
        productModel->setFilter(QString("name LIKE '%%1%'").arg(searchValue));
        productModel->select();
        QMessageBox::information(this, tr("Search Info"),
                                 QString( tr("%1 search results were found") ).arg(productModel->rowCount()));
        break;
    case 2:
        productModel->setFilter(QString("price LIKE '%%1%'").arg(searchValue));
        productModel->select();
        QMessageBox::information(this, tr("Search Info"),
                                 QString( tr("%1 search results were found") ).arg(productModel->rowCount()));
        break;
    case 3:
        productModel->setFilter(QString("stock LIKE '%%1%'").arg(searchValue));
        productModel->select();
        QMessageBox::information(this, tr("Search Info"),
                                 QString( tr("%1 search results were found") ).arg(productModel->rowCount()));
        break;
    default:
        break;
    }
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


void ProductManagerForm::on_productTableView_clicked(const QModelIndex &index)
{
    ui->idLineEdit->setText( index.sibling(index.row(), 0).data().toString() );
    ui->nameLineEdit->setText( index.sibling(index.row(), 1).data().toString() );
    ui->priceLineEdit->setText( index.sibling(index.row(), 2).data().toString() );
    ui->stockLineEdit->setText( index.sibling(index.row(), 3).data().toString() );
}

/* Order에서 주문추가 시, 재고반영을 위한 슬롯 */
void ProductManagerForm::receiveAddStock(int key, QString inStock) //추가 시 재고 변경
{
    QSqlQuery query(productModel->database());
    int stock, result;

    // 주문 전 기존 재고 뽑음
    query.prepare("SELECT stock FROM productList WHERE id = ?");
    query.bindValue(0, key);
    query.exec();

    while (query.next()) {
        stock = query.value(0).toInt();
    }

    result = stock - inStock.toInt();
    /* 주문 시, 재고부족을 위한 메시지 박스 */
    if(stock < inStock.toInt()) {
        QMessageBox::information(this, tr("Error"),
                                 QString( tr("out of stock\nYou can order up to %0.") )
                                                                    .arg(stock));
        return;
    }

    query.prepare("UPDATE productList SET stock = ? WHERE id = ?");

    query.bindValue(0, result);
    query.bindValue(1, key);
    query.exec();
    productModel->select();

}

// 키값, 수정할 수량, 주문 되어있는 수량
void ProductManagerForm::receiveModStock(int key, QString updateStock, QString orderStock)    /* Order에서 주문변경 시, 재고반영을 위한 슬롯 */
{
    QSqlQuery query(productModel->database());
    int stock, result;

    // 주문 전 기존 재고 뽑음
    query.prepare("SELECT stock FROM productList WHERE id = ?");
    query.bindValue(0, key);
    query.exec();

    while (query.next()) {
        stock = query.value(0).toInt();
    }

    result = stock + orderStock.toInt() - updateStock.toInt();

    query.prepare("UPDATE productList SET stock = ? WHERE id = ?");

    query.bindValue(0, result);
    query.bindValue(1, key);
    query.exec();
    productModel->select();
}

void ProductManagerForm::receiveDelStock(int key, QString delStock)     /* Order에서 주문내역제거 시, 재고반영을 위한 슬롯 */
{
    QSqlQuery query(productModel->database());
    int stock, result;

    // 주문 전 기존 재고 뽑음
    query.prepare("SELECT stock FROM productList WHERE id = ?");
    query.bindValue(0, key);
    query.exec();

    while (query.next()) {
        stock = query.value(0).toInt();
    }

    result = stock + delStock.toInt();

    query.prepare("UPDATE productList SET stock = ? WHERE id = ?");

    query.bindValue(0, result);
    query.bindValue(1, key);
    query.exec();
    productModel->select();
}
