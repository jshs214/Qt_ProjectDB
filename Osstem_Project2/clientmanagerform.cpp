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

    /* 테이블 뷰에 컨텍스트메뉴 사용한 시그널, 슬롯 */
    connect(ui->clientTableView, SIGNAL(customContextMenuRequested(QPoint)), this,
            SLOT(showContextMenu(QPoint)));

    /* 엔터키로 버튼 클릭 가능하게 슬롯 연결 */
    connect(ui->searchLineEdit, SIGNAL(returnPressed()),
            this, SLOT(on_searchPushButton_clicked()));

    /* 전화번호 입력 시 00(0)-000(0)-000(0)만 입력 받을 수 있도록 */
    QRegularExpressionValidator* regExpValidator = new QRegularExpressionValidator(this);
    regExpValidator->setRegularExpression(QRegularExpression("^\\d{2,3}-\\d{3,4}-\\d{4}$"));
    ui->phoneNumberLineEdit->setValidator(regExpValidator);


}

/* 소멸자에서 DB close */
ClientManagerForm::~ClientManagerForm()
{
    delete ui;
    /* 고객 DB close  */
    QSqlDatabase db = QSqlDatabase::database("clientConnection");
    if(db.isOpen()) {
        clientModel->submitAll();
        db.close();
    }
}

/* 고객정보 DB의 데이터를 불러오는 메서드 */
void ClientManagerForm::loadData()
{
    /*  Qt에서 지원하는 데이터베이스 드라이버 QSQLITE에 고객 DB 객체 선언  */
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "clientConnection");
    db.setDatabaseName("client.db");    // DB명은 client.db
    if (db.open()) {
        QSqlQuery query(db);            // db 를 사용하여 QSqlQuery 객체를 생성

        /* SQL 쿼리문을 사용해 고객 테이블 생성 */
        query.exec("CREATE TABLE IF NOT EXISTS clientList(id INTEGER Primary Key, name VARCHAR(30) NOT NULL,"
                   " phoneNumber VARCHAR(20) NOT NULL, address VARCHAR(50));");

        clientModel = new QSqlTableModel(this, db); // QSqlTableModel을 이용해 고객모델 객체 생성
        clientModel->setTable("clientList");        //모델이 작동하는 DB 테이블 설정
        clientModel->select();                      //모델의 데이터 조회

        /* 고객 모델의 헤더 명 설정 */
        clientModel->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
        clientModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Name"));
        clientModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Phone Number"));
        clientModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Address"));

        ui->clientTableView->setModel(clientModel); //ui에 표시할 모델 설정
    }
    /* 고객정보 데이터를 불러 올 때, 주문정보, 채팅서버에 불러온 데이터를 시그널을 통해 전송 */
    for(int i = 0; i < clientModel->rowCount(); i++) {
        int id = clientModel->data(clientModel->index(i, 0)).toInt();
        QString name = clientModel->data(clientModel->index(i, 1)).toString();
        QString phoneNumber = clientModel->data(clientModel->index(i, 2)).toString();
        QString address = clientModel->data(clientModel->index(i, 3)).toString();

        /* 고객정보데이터를 주문정보관리, 채팅서버에 정보 전송 시그널 */
        emit clientAddToOrder(id, name, phoneNumber,address);   //id, 이름, 전화번호, 주소
        emit clientAddToServer(id, name);                       //id, 이름
    }
}

/* 고객 키값 생성 메서드 */
int ClientManagerForm::makeId( )
{
    /* 키값 생성 시, 중복 되지 않게 모델에 존재하는 키값의 마지막 +1 하여 생성  */
    if(clientModel->rowCount() == 0) {
        return 10000;
    } else {
        auto id = clientModel->data(clientModel->index(clientModel->rowCount()-1, 0)).toInt();
        return ++id;
    }
}

/* ContextMenu 슬롯 */
void ClientManagerForm::showContextMenu(const QPoint &pos)
{
    QPoint globalPos = ui->clientTableView->mapToGlobal(pos);
    if(ui->clientTableView->indexAt(pos).isValid())
        menu->exec(globalPos);
}

/* 고객정보추가를 위한 슬롯 */
void ClientManagerForm::on_addPushButton_clicked()
{
    /* 추가 시 변수에 입력했던 id, 이름, 전화번호, 주소의 데이터 저장 */
    QString name, phoneNumber, address;
    int id = makeId( );
    ui->idLineEdit->setText(QString::number(id));
    name = ui->nameLineEdit->text();
    phoneNumber = ui->phoneNumberLineEdit->text();
    address = ui->addressLineEdit->text();

    /* 추가할 고객의 정보를 다 입력하면 고객 정보추가 */
    if(name.length() && phoneNumber.length() && address.length()) {
        QSqlQuery query(clientModel->database());   //QSqlQuery 객체(고객모델)
        query.prepare("INSERT INTO clientList VALUES (?, ?, ?, ?)");    //sql쿼리문 준비
        /* sql쿼리문에 값 바인딩 */
        query.bindValue(0, id);
        query.bindValue(1, name);
        query.bindValue(2, phoneNumber);
        query.bindValue(3, address);
        query.exec();           //sql 쿼리 실행
        clientModel->select();  //모델의 데이터 조회

        /* 추가한 고객정보데이터를 주문정보관리, 채팅서버에 정보 전송 시그널 */
        emit clientAddToOrder(id, name, phoneNumber,address);   //id, 이름, 전화번호, 주소
        emit clientAddToServer(id, name);                       //id, 이름
    }

    /* ui의 LineEdit 값 clear */
    ui->idLineEdit->clear();
    ui->nameLineEdit->clear();
    ui->phoneNumberLineEdit->clear();
    ui->addressLineEdit->clear();
}

/* 고객정보변경을 위한 슬롯 */
void ClientManagerForm::on_modifyPushButton_clicked()
{
    QModelIndex index = ui->clientTableView->currentIndex();    //변수에 뷰의 현재 index값 저장

    if(index.isValid()) {
        /* id는 모델의 현재 데이터값, QString 변수는 변경 할 이름, 전화번호, 주소의 데이터 저장 */
        int id = clientModel->data(index.siblingAtColumn(0)).toInt();
        QString name, phoneNumber, address;
        name = ui->nameLineEdit->text();
        phoneNumber = ui->phoneNumberLineEdit->text();
        address = ui->addressLineEdit->text();

        QSqlQuery query(clientModel->database());   //QSqlQuery 객체(고객모델)
        query.prepare("UPDATE clientList SET name = ?, phoneNumber = ?,"
                      " address = ? WHERE id = ?"); //sql쿼리문 준비
        /* sql쿼리문에 값 바인딩 */
        query.bindValue(0, name);
        query.bindValue(1, phoneNumber);
        query.bindValue(2, address);
        query.bindValue(3, id);
        query.exec();           //sql 쿼리 실행
        clientModel->select();  //모델의 데이터 조회

        /* 변경한 고객정보데이터를 주문정보관리, 채팅서버에 정보 전송 시그널 */
        emit clientModToOrder(id, name, phoneNumber, address);  //id, 이름, 전화번호, 주소
        emit clientModToServer(id, name);                       //id, 이름
    }

}

/* 고객정보삭제를 위한 슬롯 */
void ClientManagerForm::removeItem()
{
    QModelIndex index = ui->clientTableView->currentIndex();    //변수에 뷰의 현재 index값 저장

    int sendId =index.sibling(index.row(), 0).data().toString().toInt();    //현재 index의 id

    /* 삭제한 고객ID를 주문정보관리, 채팅서버에 전송 시그널 */
    emit clientDelToOrder(sendId);  //삭제한 고객 id
    emit clientRevToServer(sendId); //삭제한 고객 id

    if(index.isValid()) {
        clientModel->removeRow(index.row());        //모델의 현재 행 삭제
        clientModel->select();                      //모델의 데이터 조회
        ui->clientTableView->update();              //뷰 update
    }
}

/* 고객정보검색을 위한 슬롯 */
void ClientManagerForm::on_searchPushButton_clicked()
{
    QString searchValue = ui->searchLineEdit->text();   //검색할 데이터 저장

    int i = ui->searchComboBox->currentIndex();         //무엇으로 검색할지 콤보박스의 인덱스를 가져옴
    switch (i){
    case 0: //id 검색
        /* 검색한 데이터와 id가 일치하거나 포함되면 뷰에 검색결과 출력 후 메시지박스 */
        clientModel->setFilter(QString("id LIKE '%%1%'").arg(searchValue));
        clientModel->select();
        QMessageBox::information(this, tr("Search Info"),
                                 QString( tr("%1 search results were found") ).arg(clientModel->rowCount()));
        break;
    case 1: //이름 검색
        /* 검색한 데이터에 이름이 일치하거나 포함되면 뷰에 검색결과 출력 후 메시지박스 */
        clientModel->setFilter(QString("name LIKE '%%1%'").arg(searchValue));
        clientModel->select();
        QMessageBox::information(this, tr("Search Info"),
                                 QString( tr("%1 search results were found") ).arg(clientModel->rowCount()));
        break;
    case 2: //전화번호 검색
        /* 검색한 데이터에 id가 일치하거나 포함되면 뷰에 검색결과 출력 후 메시지박스 */
        clientModel->setFilter(QString("phoneNumber LIKE '%%1%'").arg(searchValue));
        clientModel->select();
        QMessageBox::information(this, tr("Search Info"),
                                 QString( tr("%1 search results were found") ).arg(clientModel->rowCount()));
        break;
    case 3: //주소 검색
        /* 검색한 데이터에 id가 일치하거나 포함되면 뷰에 검색결과 출력 후 메시지박스 */
        clientModel->setFilter(QString("address LIKE '%%1%'").arg(searchValue));
        clientModel->select();
        QMessageBox::information(this, tr("Search Info"),
                                 QString( tr("%1 search results were found") ).arg(clientModel->rowCount()));
        break;
    default:
        break;
    }

    /* 검색 후 id 생성 시, DB에 저장된 마지막 id 값보다 +1 */
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

/* 고객정보를 담고있는 모델의 모든 데이터 출력 슬롯 */
void ClientManagerForm::on_statePushButton_clicked()
{
    clientModel->setFilter("");
    clientModel->select();
}

/* 등록된 고객정보 클릭 시 관련정보 출력 슬롯 */
void ClientManagerForm::on_clientTableView_clicked(const QModelIndex &index)
{
    /* 뷰의 데이터 클릭 시, ui->LineEdit에 고객정보 출력 */
    ui->idLineEdit->setText( index.sibling(index.row(), 0).data().toString() );
    ui->nameLineEdit->setText( index.sibling(index.row(), 1).data().toString() );
    ui->phoneNumberLineEdit->setText( index.sibling(index.row(), 2).data().toString() );
    ui->addressLineEdit->setText( index.sibling(index.row(), 3).data().toString() );
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
