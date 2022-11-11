#include "clientmanagerform.h"
#include "qsqltablemodel.h"
#include "ui_clientmanagerform.h"

#include <QFile>
#include <QMenu>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>

ClientManagerForm::ClientManagerForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClientManagerForm)
{
    ui->setupUi(this);

    ui->clientTableView->setContextMenuPolicy(Qt::CustomContextMenu);
    /* ContextMenu의 remove 액션, 리스트의 데이터 삭제 */
    QAction* removeAction = new QAction(tr("&Remove"));
    removeAction->setIcon(QIcon(":/images/eraser.png"));
    connect(removeAction, SIGNAL(triggered()), SLOT(removeItem()));
    menu = new QMenu;
    menu->addAction(removeAction);


    connect(ui->clientTableView, SIGNAL(customContextMenuRequested(QPoint)), this,
            SLOT(showContextMenu(QPoint)));

    connect(ui->searchLineEdit, SIGNAL(returnPressed()),
            this, SLOT(on_searchPushButton_clicked()));

    /* 전화번호 입력 시 00(0)-000(0)-000(0)만 입력 받을 수 있도록 */
    QRegularExpressionValidator* regExpValidator = new QRegularExpressionValidator(this);
    regExpValidator->setRegularExpression(QRegularExpression("^\\d{2,3}-\\d{3,4}-\\d{4}$"));
    ui->phoneNumberLineEdit->setValidator(regExpValidator);


}

void ClientManagerForm::loadData()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "clientConnection");
    db.setDatabaseName("client.db");
    if (db.open()) {
        QSqlQuery query(db);
        query.exec("CREATE TABLE IF NOT EXISTS clientList(id INTEGER Primary Key, name VARCHAR(30) NOT NULL,"
                   " phoneNumber VARCHAR(20) NOT NULL, address VARCHAR(50));");

        clientModel = new QSqlTableModel(this, db);
        clientModel->setTable("clientList");
        clientModel->select();
        clientModel->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
        clientModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Name"));
        clientModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Phone Number"));
        clientModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Address"));

        ui->clientTableView->setModel(clientModel);
    }
    for(int i = 0; i < clientModel->rowCount(); i++) {
        int id = clientModel->data(clientModel->index(i, 0)).toInt();
        QString name = clientModel->data(clientModel->index(i, 1)).toString();
        QString phoneNumber = clientModel->data(clientModel->index(i, 2)).toString();
        QString address = clientModel->data(clientModel->index(i, 3)).toString();

        emit clientAddToOrder(id, name, phoneNumber,address);
        emit clientAddToServer(id, name);
    }
}

/* 소멸자에서 파일 출력 */
ClientManagerForm::~ClientManagerForm()
{
    delete ui;
    QSqlDatabase db = QSqlDatabase::database("clientConnection");
    if(db.isOpen()) {
        clientModel->submitAll();
        db.close();
    }
}

int ClientManagerForm::makeId( )
{
    if(clientModel->rowCount() == 0) {
        return 10000;
    } else {
        auto id = clientModel->data(clientModel->index(clientModel->rowCount()-1, 0)).toInt();
        return ++id;
    }
}

/* 버튼 클릭 시 입력 값 초기화 하는 슬롯 */
void ClientManagerForm::on_clearButton_clicked()
{
    ui->idLineEdit->clear();
    ui->nameLineEdit->clear();
    ui->phoneNumberLineEdit->clear();
    ui->addressLineEdit->clear();
    ui->searchLineEdit->clear();
}

/* ContextMenu 슬롯 */
void ClientManagerForm::showContextMenu(const QPoint &pos)
{
    QPoint globalPos = ui->clientTableView->mapToGlobal(pos);
    if(ui->clientTableView->indexAt(pos).isValid())
        menu->exec(globalPos);
}


void ClientManagerForm::on_addPushButton_clicked()
{
    QString name, phoneNumber, address;
    int id = makeId( );
    ui->idLineEdit->setText(QString::number(id));
    name = ui->nameLineEdit->text();
    phoneNumber = ui->phoneNumberLineEdit->text();
    address = ui->addressLineEdit->text();

    if(name.length()) {
        QSqlQuery query(clientModel->database());
        query.prepare("INSERT INTO clientList VALUES (?, ?, ?, ?)");
        query.bindValue(0, id);
        query.bindValue(1, name);
        query.bindValue(2, phoneNumber);
        query.bindValue(3, address);
        query.exec();
        clientModel->select();

        emit clientAddToOrder(id, name, phoneNumber,address);
        emit clientAddToServer(id, name);
    }

    ui->idLineEdit->clear();
    ui->nameLineEdit->clear();
    ui->phoneNumberLineEdit->clear();
    ui->addressLineEdit->clear();
}


void ClientManagerForm::on_modifyPushButton_clicked()
{

    QModelIndex index = ui->clientTableView->currentIndex();
    if(index.isValid()) {
        int id = clientModel->data(index.siblingAtColumn(0)).toInt();
        QString name, phoneNumber, address;
        name = ui->nameLineEdit->text();
        phoneNumber = ui->phoneNumberLineEdit->text();
        address = ui->addressLineEdit->text();

        QSqlQuery query(clientModel->database());
        query.prepare("UPDATE clientList SET name = ?, phoneNumber = ?, address = ? WHERE id = ?");
        query.bindValue(0, name);
        query.bindValue(1, phoneNumber);
        query.bindValue(2, address);
        query.bindValue(3, id);
        query.exec();
        clientModel->select();
        emit clientModToOrder(id, name, phoneNumber, address);
        emit clientModToServer(id, name);
    }

}

/* 고객정보의 데이터(트리위젯)의 리스트 제거 슬롯 */
void ClientManagerForm::removeItem()
{
    QModelIndex index = ui->clientTableView->currentIndex();
    int sendId =index.sibling(index.row(), 0).data().toString().toInt();
    emit clientDelToOrder(sendId);
    emit clientRevToServer(sendId);

    if(index.isValid()) {
        clientModel->removeRow(index.row());
        clientModel->select();
        ui->clientTableView->update();
    }
}

void ClientManagerForm::on_searchPushButton_clicked()
{
    QString searchValue = ui->searchLineEdit->text();

    int i = ui->searchComboBox->currentIndex(); //무엇으로 검색할지 콤보박스의 인덱스를 가져옴
    switch (i){
    case 0:
        clientModel->setFilter(QString("id = '%1'").arg(searchValue));
        clientModel->select();
        QMessageBox::information(this, tr("Search Info"),
                                 QString( tr("%1 search results were found") ).arg(clientModel->rowCount()));
        break;
    case 1:
        clientModel->setFilter(QString("name LIKE '%%1%'").arg(searchValue));
        clientModel->select();
        QMessageBox::information(this, tr("Search Info"),
                                 QString( tr("%1 search results were found") ).arg(clientModel->rowCount()));
        break;
    case 2:
        clientModel->setFilter(QString("phoneNumber LIKE '%%1%'").arg(searchValue));
        clientModel->select();
        QMessageBox::information(this, tr("Search Info"),
                                 QString( tr("%1 search results were found") ).arg(clientModel->rowCount()));
        break;
    case 3:
        clientModel->setFilter(QString("address LIKE '%%1%'").arg(searchValue));
        clientModel->select();
        QMessageBox::information(this, tr("Search Info"),
                                 QString( tr("%1 search results were found") ).arg(clientModel->rowCount()));
        break;
    default:
        break;
    }
    QString filterStr = "id in (";
    for(int i = 0; i < clientModel->rowCount(); i++) {
        int id = clientModel->data(clientModel->index(i, 0)).toInt();
        if(i != clientModel->rowCount()-1)
            filterStr += QString("%1, ").arg(id);
        else
            filterStr += QString("%1);").arg(id);
    }
    clientModel->setFilter(filterStr);

}

void ClientManagerForm::on_statePushButton_clicked()
{
    clientModel->setFilter("");
    clientModel->select();
}

void ClientManagerForm::on_clientTableView_clicked(const QModelIndex &index)
{
    ui->idLineEdit->setText( index.sibling(index.row(), 0).data().toString() );
    ui->nameLineEdit->setText( index.sibling(index.row(), 1).data().toString() );
    ui->phoneNumberLineEdit->setText( index.sibling(index.row(), 2).data().toString() );
    ui->addressLineEdit->setText( index.sibling(index.row(), 3).data().toString() );
}

