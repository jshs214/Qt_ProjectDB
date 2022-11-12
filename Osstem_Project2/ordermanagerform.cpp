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
    connect(ui->orderTableView, SIGNAL(customContextMenuRequested(QPoint)), this,
            SLOT(showContextMenu(QPoint)));

    ui->stockLineEdit->setValidator(new QIntValidator(0, 9999, this) ); //수량에 0~9999 범위의 숫자만 받도록

    connect(ui->searchLineEdit, SIGNAL(returnPressed()), this, SLOT(on_searchPushButton_clicked()));
    connect(ui->clientLineEdit, SIGNAL(returnPressed()), this, SLOT(on_clientButton_clicked()));
    connect(ui->productLineEdit, SIGNAL(returnPressed()), this, SLOT(on_productButton_clicked()));

    ui->clientTreeWidget->QTreeView::setColumnWidth(0,40);
    ui->clientTreeWidget->QTreeView::setColumnWidth(1,70);
    ui->clientTreeWidget->QTreeView::setColumnWidth(2,95);
    ui->productTreeWidget->QTreeView::setColumnWidth(0,40);
    ui->productTreeWidget->QTreeView::setColumnWidth(1,70);
    ui->productTreeWidget->QTreeView::setColumnWidth(2,95);
}


void OrderManagerForm::loadData()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "orderConnection");
    db.setDatabaseName("order.db");
    if (db.open()) {
        QSqlQuery query(db);

        query.exec("CREATE TABLE IF NOT EXISTS orderList(id INTEGER Primary Key, client VARCHAR(30) NOT NULL,"
                   "product VARCHAR(30) NOT NULL, stock VARCHAR(10), "
                   "price VARCHAR(30), sum VARCHAR(30), address VARCHAR(30));");

        orderModel = new QSqlTableModel(this, db);
        orderModel->setTable("orderList");
        orderModel->select();
        orderModel->setHeaderData(0, Qt::Horizontal, QObject::tr("OrderNum"));
        orderModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Client"));
        orderModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Product"));
        orderModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Stock"));
        orderModel->setHeaderData(4, Qt::Horizontal, QObject::tr("Price"));
        orderModel->setHeaderData(5, Qt::Horizontal, QObject::tr("Sum"));
        orderModel->setHeaderData(6, Qt::Horizontal, QObject::tr("Address"));

        ui->orderTableView->setModel(orderModel);

        ui->orderTableView->resizeColumnsToContents();  //컬럼 사이즈를 데이터에 맞게 조절
    }
}

/* 소멸자에서 파일 출력 */
OrderManagerForm::~OrderManagerForm()
{
    delete ui;
    QSqlDatabase db = QSqlDatabase::database("orderConnection");
    if(db.isOpen()) {
        orderModel->submitAll();
        db.close();
    }
}


int OrderManagerForm::makeId( )
{
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
    //if(ui->orderTableView->currentItem() == nullptr)    return; //에외처리
    QPoint globalPos = ui->orderTableView->mapToGlobal(pos);
    menu->exec(globalPos);
}

/* 주문정보의 데이터(트리위젯)의 리스트 제거 슬롯 */
void OrderManagerForm::removeItem()
{
    QString productKey, delStock;
    QModelIndex index = ui->orderTableView->currentIndex();
    productKey = index.sibling(index.row(), 2).data().toString().left(5);
    delStock = index.sibling(index.row(), 3).data().toString();

    /* 제품 수량 변경 시, 제품리스트에 있는 재고 반영*/
    {
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
        orderModel->removeRow(index.row());
        orderModel->select();
        ui->orderTableView->update();
    }

    /* 데이터 제거 시, 제품 ID와 수량을 시그널로 보냄 */
    emit removedataSent(productKey.toInt() , delStock);
}


/* 버튼 클릭 시 입력 값 초기화 하는 슬롯 */
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

//고객 정보 추가 or loaddata
void OrderManagerForm::updateClient(int id,QString name, QString phoneNumber, QString address)
{
    QTreeWidgetItem* cItem = new QTreeWidgetItem(ui->clientTreeWidget);
    cItem->setText(0,QString::number(id));
    cItem->setText(1,name);
    cItem->setText(2,phoneNumber);
    cItem->setText(3,address);
}

// 고객 정보 제거 시 반영
void OrderManagerForm::delClient(int id)
{
    {
        auto item = ui->clientTreeWidget->findItems(QString::number(id), Qt::MatchFixedString, 0);
        foreach(auto i, item) {
            if(QString::number(id) == i->text(0))
                ui->clientTreeWidget->takeTopLevelItem(ui->clientTreeWidget->indexOfTopLevelItem(i));
            delete i;
        }
    }

}
// 고객 정보 변경 시 반영
void OrderManagerForm::modClient(int id, QString name, QString phoneNumber, QString address)
{
    auto item = ui->clientTreeWidget->findItems(QString::number(id), Qt::MatchFixedString, 0);        //키값비교, 0
    foreach(auto i, item) {
        if(QString::number(id) == i->text(0)){
            i->setText(0, QString::number(id));
            i->setText(1, name);
            i->setText(2, phoneNumber);
            i->setText(3, address);
        }
    }
}

//고객 검사
void OrderManagerForm::on_clientButton_clicked()
{
    QString str = ui->clientLineEdit->text();
    auto flag =  Qt::MatchCaseSensitive|Qt::MatchContains;

    for(int i =0; i < ui->clientTreeWidget->topLevelItemCount() ; i++)
    {
        ui->clientTreeWidget->topLevelItem(i)->setHidden(true);
    }

    auto cItem = ui->clientTreeWidget->findItems(str, Qt::MatchFixedString, 0); //id 는 완전일치
    foreach(auto i, cItem) {
        i->setHidden(false);
    }

    cItem = ui->clientTreeWidget->findItems(str, flag, 1);
    foreach(auto i, cItem) {
        i->setHidden(false);
    }

}


void OrderManagerForm::on_clientTreeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    ui->idLineEdit->clear();
    Q_UNUSED(column);
    QString client;
    client = item->text(0)+" ("+item->text(1)+")";

    ui->cIdLineEdit->setText(client);
    ui->addressLineEdit->setText(item->text(3));
}

//제품 정보 추가 or loaddata
void OrderManagerForm::updateProduct(int id, QString name, QString price, QString stock)
{
    QTreeWidgetItem* pItem = new QTreeWidgetItem(ui->productTreeWidget);
    pItem->setText(0,QString::number(id));
    pItem->setText(1,name);
    pItem->setText(2,price);
    pItem->setText(3,stock);
}

// 제품 정보 제거 시 반영
void OrderManagerForm::delProduct(int id)
{
    {
        auto item = ui->productTreeWidget->findItems(QString::number(id), Qt::MatchFixedString, 0);
        foreach(auto i, item) {
            if(QString::number(id) == i->text(0))
                ui->productTreeWidget->takeTopLevelItem(ui->productTreeWidget->indexOfTopLevelItem(i));
            delete i;
        }
    }

}

// 제품 정보 변경 시 반영
void OrderManagerForm::modProduct(int id, QString name, QString price, QString stock)
{
    auto item = ui->productTreeWidget->findItems(QString::number(id), Qt::MatchFixedString, 0);        //키값비교, 0
    foreach(auto i, item) {
        if(QString::number(id) == i->text(0)){
            i->setText(0, QString::number(id));
            i->setText(1, name);
            i->setText(2, price);
            i->setText(3, stock);
        }
    }
}


void OrderManagerForm::on_productButton_clicked()
{
    QString str = ui->productLineEdit->text();
    auto flag =  Qt::MatchCaseSensitive|Qt::MatchContains;

    for(int i =0; i < ui->productTreeWidget->topLevelItemCount() ; i++)
    {
        ui->productTreeWidget->topLevelItem(i)->setHidden(true);
    }

    auto pItem = ui->productTreeWidget->findItems(str, Qt::MatchFixedString, 0); //id 는 완전일치
    foreach(auto i, pItem) {
        i->setHidden(false);
    }

    pItem = ui->productTreeWidget->findItems(str, flag, 1);
    foreach(auto i, pItem) {
        i->setHidden(false);
    }
}


void OrderManagerForm::on_productTreeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    ui->idLineEdit->clear();
    ui->stockLineEdit->clear();
    Q_UNUSED(column);
    QString product;
    product = item->text(0)+" ("+item->text(1)+")";

    ui->pIdLineEdit->setText(product);
    ui->priceLineEdit->setText(item->text(2));
}

/* 총합 계산 슬롯 */
void OrderManagerForm::on_stockLineEdit_textChanged(const QString &stock)
{
    int sum;
    sum = stock.toInt() * ui->priceLineEdit->text().toInt();
    ui->sumLineEdit->setText(QString::number(sum));
}

/* 주문 정보 추가 */
void OrderManagerForm::on_addPushButton_clicked()
{
    QTreeWidgetItem* pItem = ui->productTreeWidget->currentItem();
    QString  client, product, stock, price, address,sum;
    QString productKey, productTree_Stock;


    int id = makeId( );
    ui->idLineEdit->setText(QString::number(id));
    client = ui->cIdLineEdit->text();
    product = ui->pIdLineEdit->text();
    stock = ui->stockLineEdit->text();
    price = ui->priceLineEdit->text();
    address = ui->addressLineEdit->text();
    sum = ui->sumLineEdit->text();

    productTree_Stock = pItem->text(3); //제품리스트의 재고
    productKey =  ui->pIdLineEdit->text().left(5);   //제품 키값

    /* 총합을 위한 연산 */
    int result = price.toInt() * stock.toInt();
    sum = QString::number(result);

    /* 재고반영을 위해 제품키값과, 수량을 시그널로 보냄 */
    emit productAddKeySent(productKey.toInt(), stock );

    if(pItem == nullptr) return; // 예외

    /* 주문 수량이 재고보다 많을 경우 예외*/
    if(productTree_Stock.toInt() < ui->stockLineEdit->text().toInt()) return;

    /* 주문이 되면 제품리스트의 재고를 변경 */
    QString result_stock = QString::number(productTree_Stock.toInt() - stock.toInt());
    pItem->setText((3), result_stock);

    /* 주문 할 데이터를 다 입력, 재고>=주문수량이면 주문정보 추가 */
    if( client.length() && product.length() && stock.length() &&price.length() && address.length()
            && productTree_Stock.toInt() >= stock.toInt())
    {
        QSqlQuery query(orderModel->database());
        query.prepare("INSERT INTO orderList VALUES (?, ?, ?, ?, ?, ?, ?)");
        query.bindValue(0, id);
        query.bindValue(1, client);
        query.bindValue(2, product);
        query.bindValue(3, stock);
        query.bindValue(4, price);
        query.bindValue(5, sum);
        query.bindValue(6, address);

        query.exec();
        orderModel->select();

        ui->cIdLineEdit->clear();
        ui->pIdLineEdit->clear();
        ui->stockLineEdit->clear();
        ui->addressLineEdit->clear();
        ui->priceLineEdit->clear();
    }

    ui->orderTableView->resizeColumnsToContents();  //컬럼 사이즈를 데이터에 맞게 조절
}



void OrderManagerForm::on_modifyPushButton_clicked()
{

    QModelIndex index = ui->orderTableView->currentIndex();
    if(index.isValid()) {
        // 프로덕트 트리위젯 재고 반영 아직 안함
        //        QTreeWidgetItem* pItem = ui->productTreeWidget->currentItem();

        int id = orderModel->data(index.siblingAtColumn(0)).toInt();
        QString client, product, stock, price, address, sum;
        QString productKey, orderStock;

        client = ui->cIdLineEdit->text();
        product = ui->pIdLineEdit->text();
        stock = ui->stockLineEdit->text();
        price = ui->priceLineEdit->text();
        address = ui->addressLineEdit->text();
        sum = ui->sumLineEdit->text();

        QSqlQuery query(orderModel->database());

        // 주문 테이블의 주문 해놓은 재고 뽑음
        query.prepare("SELECT product, stock FROM orderList WHERE id = ?");
        query.bindValue(0, id);
        query.exec();

        while (query.next()) {
            productKey = query.value(0).toString().left(5); //재고 키값
            orderStock = query.value(1).toString();         // 입력된 수량 (주문되어있는 재고)
        }


        /* 제품 수량 변경 시, 제품리스트에 있는 재고 반영*/
        {
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

        /* 재고반영을 위해 키값, 수정할 수량, 입력된 수량을 시그널로 보냄 */
        emit productModKeySent(productKey.toInt(), stock , orderStock);

        query.prepare("UPDATE orderList SET client = ?, product = ?, stock = ?,"
                      "price = ?, sum = ?, address = ? WHERE id = ?");

        query.bindValue(0, client);
        query.bindValue(1, product);
        query.bindValue(2, stock);
        query.bindValue(3, price);
        query.bindValue(4, sum);
        query.bindValue(5, address);
        query.bindValue(6, id);

        query.exec();
        orderModel->select();
    }
    ui->orderTableView->resizeColumnsToContents();  //컬럼 사이즈를 데이터에 맞게 조절
}


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


void OrderManagerForm::on_searchPushButton_clicked()
{
    QString searchValue = ui->searchLineEdit->text();

    int i = ui->searchComboBox->currentIndex(); //무엇으로 검색할지 콤보박스의 인덱스를 가져옴
    switch (i){
    case 0:
        orderModel->setFilter(QString("id = '%1'").arg(searchValue));
        orderModel->select();
        QMessageBox::information(this, tr("Search Info"),
                                 QString( tr("%1 search results were found") ).arg(orderModel->rowCount()));
        break;
    case 1:
        orderModel->setFilter(QString("client LIKE '%%1%'").arg(searchValue));
        orderModel->select();
        QMessageBox::information(this, tr("Search Info"),
                                 QString( tr("%1 search results were found") ).arg(orderModel->rowCount()));
        break;
    case 2:
        orderModel->setFilter(QString("product LIKE '%%1%'").arg(searchValue));
        orderModel->select();
        QMessageBox::information(this, tr("Search Info"),
                                 QString( tr("%1 search results were found") ).arg(orderModel->rowCount()));
        break;

    default:
        break;
    }
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


void OrderManagerForm::on_statePushButton_clicked()
{
    orderModel->setFilter("");
    orderModel->select();
}

