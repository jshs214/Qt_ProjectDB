#include "chatserverform.h"
#include "qsqltablemodel.h"
#include "qstandarditemmodel.h"
#include "ui_chatserverform.h"
#include "logthread.h"

#include <QPushButton>
#include <QBoxLayout>
#include <QTcpServer>
#include <QTcpSocket>
#include <QApplication>
#include <QMessageBox>
#include <QScrollBar>
#include <QDateTime>
#include <QDebug>
#include <QMenu>
#include <QFile>
#include <QFileInfo>
#include <QProgressDialog>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
ChatServerForm::ChatServerForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatServerForm), totalSize(0), byteReceived(0)
{
    ui->setupUi(this);
    /* ui 설정 코드 들*/
    QList<int> sizes;
    sizes << 200 << 500;
    ui->splitter->setSizes(sizes);

    ui->svClientTableView->setContextMenuPolicy(Qt::CustomContextMenu);

    /*  Qt에서 지원하는 데이터베이스 드라이버 QSQLITE에 고객 DB 객체 선언  */
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "serverConnection");
    db.setDatabaseName("serverClient.db");    // DB명은 client.db
    if (db.open()) {
        QSqlQuery query(db);            // db 를 사용하여 QSqlQuery 객체를 생성
        /* SQL 쿼리문을 사용해 고객 테이블 생성 */
        query.exec("CREATE TABLE IF NOT EXISTS serverClientList(status VARCHAR(10),"
                   " name VARCHAR(30) NOT NULL,"
                   "id INTEGER Primary Key );");

        serverClientModel = new QSqlTableModel(this, db); // QSqlTableModel을 이용해 고객모델 객체 생성
        serverClientModel->setTable("serverClientList");        //모델이 작동하는 DB 테이블 설정

        serverClientModel->select();                      //모델의 데이터 조회

        /* 고객 모델의 헤더 명 설정 */
        serverClientModel->setHeaderData(0, Qt::Horizontal, QObject::tr("Status"));
        serverClientModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Name"));
        serverClientModel->setHeaderData(2, Qt::Horizontal, QObject::tr("id"));

        ui->svClientTableView->setModel(serverClientModel); //ui에 표시할 고객모델 설정

        chattingModel = new QStandardItemModel(0,3);
        chattingModel->setHeaderData(0, Qt::Horizontal, QObject::tr("Status"));
        chattingModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Name"));
        chattingModel->setHeaderData(2, Qt::Horizontal, QObject::tr("id"));

        ui->chattingTreeView->setModel(chattingModel); //ui에 표시할 고객모델 설정

    }

    /* 메시지 전송을 위한 서버 */
    chatServer = new QTcpServer(this);
    connect(chatServer, SIGNAL(newConnection()), SLOT(clientConnect( )));
    if (!chatServer->listen(QHostAddress::Any, PORT_NUMBER)) {
        QMessageBox::critical(this, tr("Chatting Server"), \
                              tr("Unable to start the server: %1.") \
                              .arg(chatServer->errorString( )));
        return;
    }
    /* 파일 전송을 위한 서버 */
    fileServer = new QTcpServer(this);
    connect(fileServer, SIGNAL(newConnection()), SLOT(acceptConnection()));
    if (!fileServer->listen(QHostAddress::Any, PORT_NUMBER+1)) {
        QMessageBox::critical(this, tr("Chatting Server"), \
                              tr("Unable to start the server: %1.") \
                              .arg(fileServer->errorString( )));
        return;
    }

    qDebug()<<tr("Start listening ...");

    /* Context 메뉴의 액션 설정 */
    QAction* inviteAction = new QAction(tr("&Invite"));
    inviteAction->setObjectName(tr("Invite"));
    connect(inviteAction, SIGNAL(triggered()), SLOT(inviteClient()));
    QAction* removeAction = new QAction(tr("&Kick out"));
    connect(removeAction, SIGNAL(triggered()), SLOT(kickOut()));
    menu = new QMenu;
    menu->addAction(inviteAction);
    menu->addAction(removeAction);

    /* 파일전송 시 ProgressDialog 설정 */
    progressDialog = new QProgressDialog(0);
    progressDialog->setAutoClose(true);
    progressDialog->reset();

    /* logThread 객체 생성 */
    logThread = new LogThread(this);
    logThread->start();

    connect(ui->savePushButton, SIGNAL(clicked()), logThread, SLOT(saveData()));

    qDebug() << tr("The server is running on port %1.").arg(chatServer->serverPort( ));
    ui->stateLineedit->setText(tr("The server is running on port %1.")
                               .arg(chatServer->serverPort()));

    connect(ui->inputLineEdit, SIGNAL(returnPressed( )), SLOT(on_sendButton_clicked( )));

}

/* 소멸자에서 스레드 실행을 종료, 서버를 닫음 DB close  */
ChatServerForm::~ChatServerForm()
{
    delete ui;
    /* 서버고객 DB close  */
    QSqlDatabase db = QSqlDatabase::database("serverConnection");
    if(db.isOpen()) {
        serverClientModel->submitAll();
        db.close();
    }
    logThread->terminate();

    chatServer->close( );
    fileServer->close( );
}
/* 고객의 데이터가 추가되면 서버에도 추가하는 슬롯 */
void ChatServerForm::addClient(int id, QString name)
{
    QSqlQuery query(serverClientModel->database());   //QSqlQuery 객체(서버고객모델)
    query.prepare("INSERT INTO serverClientList VALUES (?, ?, ?)");    //sql쿼리문 준비
    /* sql쿼리문에 값 바인딩 */
    query.bindValue(0, tr("Off"));
    query.bindValue(1, name);
    query.bindValue(2, id);
    query.exec();           //sql 쿼리 실행
    serverClientModel->select();  //모델의 데이터 조회
}
/* 고객의 데이터가 변경되면 서버에도 추가하는 슬롯 */
void ChatServerForm::modClient(int clientKey, QString name)
{
    QSqlQuery query(serverClientModel->database());   //QSqlQuery 객체(서버고객모델)
    query.prepare("UPDATE serverClientList SET status = ?, name = ?"
                  " WHERE id = ?"); //sql쿼리문 준비
    /* sql쿼리문에 값 바인딩 */
    query.bindValue(0, tr("Off"));
    query.bindValue(1, name);
    query.bindValue(2, clientKey);
    query.exec();           //sql 쿼리 실행
    serverClientModel->select();  //모델의 데이터 조회
}

/* 고객의 데이터가 삭제되면 서버에도 삭제하는 슬롯 */
void ChatServerForm::remClient(int id)
{   /* 사용자 제거 */
    QSqlQuery query(serverClientModel->database());   //QSqlQuery 객체(서버고객모델)
    query.prepare("DELETE FROM serverClientList WHERE id = ? "); //sql쿼리문 준비
    /* sql쿼리문에 값 바인딩 */
    query.bindValue(0, id);
    query.exec();           //sql 쿼리 실행
    serverClientModel->select();  //모델의 데이터 조회

}
/* 다음 소켓에 데이터가 오면 데이터를 받고 연결끊어지면 제거 */
void ChatServerForm::clientConnect( )
{
    QTcpSocket *clientConnection = chatServer->nextPendingConnection( );
    connect(clientConnection, SIGNAL(readyRead( )), SLOT(receiveData( )));
    connect(clientConnection, SIGNAL(disconnected( )), SLOT(removeClient()));
    qDebug()<<tr("new connection is established...");
}

/* 데이터를 받을 때 */
void ChatServerForm::receiveData( )
{
    QTcpSocket *clientConnection = dynamic_cast<QTcpSocket *>(sender( ));   //어느 소켓에서 데이터를 받아왔는지
    QByteArray bytearray = clientConnection->read(BLOCK_SIZE);  //블록 크기만큼 읽음

    Chat_Status type;       // 채팅 프로토콜 타입
    char data[1020];        // 전송되는 메시지/데이터
    memset(data, 0, 1020);

    QDataStream in(&bytearray, QIODevice::ReadOnly);
    in.device()->seek(0);
    in >> type;             // 채팅 프로토콜 타입
    in.readRawData(data, 1020);     //전송되는 데이터 / 메시지

    /* 소켓에 연결된 포트, ip주소 */
    QString ip = clientConnection->peerAddress().toString();
    quint16 port = clientConnection->peerPort();
    QString receiveData = QString::fromStdString(data);

    ui->ipLineEdit->setText(clientConnection->localAddress().toString());


    QSqlQuery query(serverClientModel->database());    //QSqlQuery 객체(서버고객모델)

    switch(type) {
    case Chat_Login:    // 로그인(서버 접속)   --> 채팅방 입장 위한 정보 저장
    {
        /* 로그인 시 입력받은 이름, id 파싱 */
        auto parts = receiveData.split(u',');
        QString name = QString(parts[0]);
        QString id = QString(parts[1]);

        /* 로그인 성공 시 사용자의 상태 변경*/
        QModelIndexList idIndex = serverClientModel->match(serverClientModel->index(0, 2), Qt::EditRole,
                                                           id, -1, Qt::MatchFlags(Qt::MatchFixedString));
        //로그인 한 id값의 인덱스를 찾음
        foreach(auto ix, idIndex) {
            if(ix.sibling(ix.row(), 1).data().toString() == name){

                //상태변경 및 설정
                if (ix.sibling(ix.row(), 0).data().toString() != tr("On")){
                    /* 해시에 데이터 저장 */
                    clientSocketHash[id] = clientConnection;    //해시에 소켓 연결
                    clientNameHash[port] = name;
                    clientPortIDHash[port]=id;

                    /* 사용자의 상태 변경 */
                    query.prepare("UPDATE serverClientList SET status = ? WHERE id = ?");  //sql쿼리문 준비
                    /* sql쿼리문에 값 바인딩 */
                    query.bindValue(0, tr("On"));
                    query.bindValue(1, id);
                    query.exec();           //sql 쿼리 실행

                    serverClientModel->select();   //모델의 데이터 조회
                    sendLogInOut(clientConnection, "true"); // 로그인 성공 유무를 전송
                    return;
                }
            }
        }
        /* 로그인 실패 시 */
        sendLogInOut(clientConnection, "false");    // 로그인 성공 유무를 전송
        clientConnection->disconnectFromHost();     // 소켓 연결 해제
        break;
    }

    case Chat_In:   // 채팅방 입장
    {
        /* 입력받은 이름, id 파싱해 채팅방에 저장 */
        auto parts = receiveData.split(u',');
        QString name = QString(parts[0]);
        QString id = QString(parts[1]);

        /* 로그인 성공 시 사용자의 상태 변경*/
        QModelIndexList idIndex = serverClientModel->match(serverClientModel->index(0, 2), Qt::EditRole,
                                                           id, -1, Qt::MatchFlags(Qt::MatchFixedString));
        //로그인 한 id값의 인덱스를 찾음
        foreach(auto ix, idIndex) {
            if (ix.sibling(ix.row(), 0).data().toString() != tr("Chat")){
                /* 사용자의 상태 변경 */
                query.prepare("UPDATE serverClientList SET status = ? WHERE id = ?");  //sql쿼리문 준비
                /* sql쿼리문에 값 바인딩 */
                query.bindValue(0, tr("Chat"));
                query.bindValue(1, id);
                query.exec();           //sql 쿼리 실행
                serverClientModel->select();   //모델의 데이터 조회

                /* 채팅방의 사용자 목록 상태 변경 */
                QStringList strings;
                strings << tr("Chat") << clientNameHash[port] << id;
                QList<QStandardItem*> items;
                for (int i = 0; i < 3; i++) {
                    items.append(new QStandardItem(strings.at(i)));
                }
                chattingModel->appendRow(items);
            }
        }

        /* Chat_List로 현재 채팅방 참여인원 전달 */
        sendChatList();
        break;
    }
    case Chat_Talk: {   // 채팅
        /* 연결되어 있는 모든 소켓 */
        foreach(QTcpSocket *sock, clientSocketHash.values()) {
            /* 발신자를 뺀 나머지에게 메시지 보내줌 */
            if(clientPortIDHash.contains(sock->peerPort()) && sock != clientConnection) {
                QModelIndexList index = serverClientModel->match(serverClientModel->index(0, 2), Qt::EditRole,
                                                                 clientPortIDHash[sock->peerPort()], -1, Qt::MatchFlags(Qt::MatchFixedString));
                //로그인 한 id값의 인덱스를 찾음
                foreach(auto ix, index) {
                    if (ix.sibling(ix.row(), 0).data().toString() == tr("Chat")){
                        /* 바이너리구조의 데이터스트림 생성, 전송 */
                        QByteArray sendArray;
                        sendArray.clear();
                        QDataStream out(&sendArray, QIODevice::WriteOnly);
                        out << Chat_Talk;
                        QString data;
                        data = QString("<font color=blue>") + clientNameHash[port]
                                + "(" + clientPortIDHash[port] + ")" +  "</font> : " + receiveData;
                        out.writeRawData(data.toStdString().data(), 1020);
                        sock->write(sendArray);
                    }
                }
            }
        }
        /* 채팅 로그 기록 */
        QTreeWidgetItem* item = new QTreeWidgetItem(ui->chatlogTreeWidget);
        item->setText(0, ip);
        item->setText(1, QString::number(port));
        item->setText(2, clientPortIDHash[port]);
        item->setText(3, clientNameHash[port]);
        item->setText(4, QString(data));
        item->setText(5, QDateTime::currentDateTime().toString());
        item->setToolTip(4, QString(data));

        ui->chatlogTreeWidget->addTopLevelItem(item);
        for(int i = 0; i < ui->chatlogTreeWidget->columnCount(); i++)
            ui->chatlogTreeWidget->resizeColumnToContents(i);

        /* 이름, id 값과 입력받은 메시지를 관리자 채팅창으로  */
        ui->message->append("<font color=blue>" + clientNameHash[port] +
                            "(" + clientPortIDHash[port] + ")" + "</font> : " + receiveData);

        /* logThread로 로그데이터 보냄 */
        logThread->appendData(item);
    }
        break;

    case Chat_Out:  // 채팅방 퇴장         --> 초대 가능
    {
        /* 퇴장 시 기존 이름, id 파싱*/
        auto parts = receiveData.split(u',');
        QString name = QString(parts[0]);
        QString id = QString(parts[1]);

        /* 사용자의 상태 변경 */
        QModelIndexList idIndex = serverClientModel->match(serverClientModel->index(0, 2), Qt::EditRole,
                                                           id, -1, Qt::MatchFlags(Qt::MatchFixedString));
        //로그인 한 id값의 인덱스를 찾음
        foreach(auto ix, idIndex) {
            if (ix.sibling(ix.row(), 0).data().toString() != tr("On")){
                /* 바이너리구조의 데이터스트림 생성, 전송 */

                query.prepare("UPDATE serverClientList SET status = ? WHERE id = ?");  //sql쿼리문 준비
                /* sql쿼리문에 값 바인딩 */
                query.bindValue(0, tr("On"));
                query.bindValue(1, id);
                query.exec();           //sql 쿼리 실행

                serverClientModel->select();   //모델의 데이터 조회
            }
        }
        /* 채팅방의 사용자 목록 상태 변경 */
        QModelIndexList chatIndex = chattingModel->match(chattingModel->index(0, 2), Qt::EditRole,
                                                         id, -1, Qt::MatchFlags(Qt::MatchFixedString));
        foreach(auto ix, chatIndex){
            if(id == ix.sibling(ix.row(), 2).data().toString()){
                chattingModel->removeRow(ix.row());        //모델의 현재 행 삭제
                ui->chattingTreeView->update();                //뷰 update
            }
        }

        /* Chat_List로 현재 채팅방 참여인원 전달 */
        sendChatList();
        break;
    }
    case Chat_LogOut:   // 로그 아웃(서버 단절) --> 초대 불가능
    {
        QString id = clientPortIDHash[port];    //해시에서 id를 가져옴
        /* 사용자의 상태 변경 */
        QModelIndexList idIndex = serverClientModel->match(serverClientModel->index(0, 2), Qt::EditRole,
                                                           id, -1, Qt::MatchFlags(Qt::MatchFixedString));
        //로그인 한 id값의 인덱스를 찾음
        foreach(auto ix, idIndex) {
            if (ix.sibling(ix.row(), 0).data().toString() != tr("Off")){
                /* 바이너리구조의 데이터스트림 생성, 전송 */
                query.prepare("UPDATE serverClientList SET status = ? WHERE id = ?");  //sql쿼리문 준비
                /* sql쿼리문에 값 바인딩 */
                query.bindValue(0, tr("Off"));
                query.bindValue(1, id);
                query.exec();           //sql 쿼리 실행
                serverClientModel->select();   //모델의 데이터 조회
                /* 서버 단절 시 저장되었던 해시에 저장된 데이터 제거 */
                clientSocketHash.remove(id);
                clientNameHash.remove(port);
                clientPortIDHash.remove(port);
            }
        }
        /* 채팅방의 사용자 목록 상태 변경 */
        QModelIndexList chatIndex = chattingModel->match(chattingModel->index(0, 2), Qt::EditRole,
                                                         id, -1, Qt::MatchFlags(Qt::MatchFixedString));
        foreach(auto ix, chatIndex){
            if(id == ix.sibling(ix.row(), 2).data().toString()){
                chattingModel->removeRow(ix.row());        //모델의 현재 행 삭제
                ui->chattingTreeView->update();                //뷰 update
            }
        }
        /* Chat_List로 현재 채팅방 참여인원 전달 */
        sendChatList();
        break;
    }
    case Chat_List: /* Chat_List로 현재 채팅방 참여인원 전달 */
        sendChatList();
        break;
    default:
        break;
    }
}
/* 연결이 끊어지면 데이터 제거 */
void ChatServerForm::removeClient()
{
    QTcpSocket *clientConnection = dynamic_cast<QTcpSocket *>(sender( ));   //어느 소켓에서 데이터를 받아왔는지
    QSqlQuery query(serverClientModel->database());    //QSqlQuery 객체(서버고객모델)

    if(clientConnection != nullptr){
        QString id =  clientPortIDHash[clientConnection->peerPort()];
        /* 사용자의 상태 변경 */
        QModelIndexList idIndex = serverClientModel->match(serverClientModel->index(0, 2), Qt::EditRole,
                                                           id, -1, Qt::MatchFlags(Qt::MatchFixedString));
        //off 상태이면 상태변경 및 설정
        foreach(auto v, idIndex) {
            Q_UNUSED(v);
            /* 바이너리구조의 데이터스트림 생성, 전송 */
            query.prepare("UPDATE serverClientList SET status = ? WHERE id = ?");  //sql쿼리문 준비
            /* sql쿼리문에 값 바인딩 */
            query.bindValue(0, tr("Off"));
            query.bindValue(1, id);
            query.exec();           //sql 쿼리 실행
            serverClientModel->select();   //모델의 데이터 조회
        }

        sendChatList();  /* Chat_List로 현재 채팅방 참여인원 전달 */

        clientSocketHash.remove(id);
        clientConnection->deleteLater();
    }
}

/* ContextMenu 슬롯 */
void ChatServerForm::on_svClientTableView_customContextMenuRequested(const QPoint &pos)
{
    QModelIndex index = ui->svClientTableView->currentIndex();    //변수에 뷰의 현재 index값 저장
    foreach(QAction *action, menu->actions()) {
        if(action->objectName() == tr("Invite"))
            action->setEnabled(index.sibling(index.row(), 0).data().toString() == tr("On"));
        else    // "Kick Out"
            action->setEnabled(index.sibling(index.row(), 0).data().toString() == tr("Chat"));
    }
    QPoint globalPos = ui->svClientTableView->mapToGlobal(pos);
    if(ui->svClientTableView->indexAt(pos).isValid())
        menu->exec(globalPos);
}



/* 클라이언트 초대하기 */
void ChatServerForm::inviteClient()
{
    /* 소켓으로 보낼 데이터를 채움 */
    QByteArray sendArray;
    QDataStream out(&sendArray, QIODevice::WriteOnly);
    out << Chat_Invite;
    out.writeRawData("", 1020);

    QModelIndex index = ui->svClientTableView->currentIndex();    //변수에 뷰의 현재 index값 저장
    QString name = index.sibling(index.row(), 1).data().toString();
    QString id = index.sibling(index.row(), 2).data().toString();

    /* 소켓은 현재 선택된 아이템에 표시된 이름과 id를 해시로 부터 가져옴 */
    /* 소켓에 데이터 전송 */
    QTcpSocket* sock = clientSocketHash[id];    //해시에 연결된 소켓가져옴
    sock->write(sendArray);
    sock->flush();

    QSqlQuery query(serverClientModel->database());    //QSqlQuery 객체(서버고객 모델)
    QModelIndexList idIndex = serverClientModel->match(serverClientModel->index(0, 2), Qt::EditRole,
                                                       id, -1, Qt::MatchFlags(Qt::MatchFixedString));
    /* 사용자의 상태 및 채팅방 목록 변경 */
    foreach(auto v, idIndex) {
        Q_UNUSED(v);
        /* 사용자의 상태 변경 */
        query.prepare("UPDATE serverClientList SET status = ? WHERE id = ?");  //sql쿼리문 준비
        /* sql쿼리문에 값 바인딩 */
        query.bindValue(0, tr("Chat"));
        query.bindValue(1, id);
        query.exec();           //sql 쿼리 실행
        serverClientModel->select();   //모델의 데이터 조회
    }
    // /* 채팅방의 사용자 목록 상태 변경 */
    QStringList strings;
    strings << tr("Chat") << name << id;
    QList<QStandardItem*> items;
    for (int i = 0; i < 3; i++) {
        items.append(new QStandardItem(strings.at(i)));
    }
    chattingModel->appendRow(items);
}

/* 클라이언트 강퇴하기 */
void ChatServerForm::kickOut()
{
    /* 소켓으로 보낼 데이터를 채움 */
    QByteArray sendArray;
    QDataStream out(&sendArray, QIODevice::WriteOnly);
    out << Chat_KickOut;
    out.writeRawData("", 1020);

    /* 소켓은 현재 선택된 아이템에 표시된 id를 해시로 부터 가져옴 */
    QModelIndex index = ui->svClientTableView->currentIndex();    //변수에 뷰의 현재 index값 저장
    QString id = index.sibling(index.row(), 2).data().toString();
    //    /* 소켓에 데이터 전송 */
    QTcpSocket* sock = clientSocketHash[id];    //해시에 연결된 소켓가져옴
    sock->write(sendArray);
    sock->flush();

    QSqlQuery query(serverClientModel->database());    //QSqlQuery 객체(서버고객모델)
    QModelIndexList idIndex = serverClientModel->match(serverClientModel->index(0, 2), Qt::EditRole,
                                                       id, -1, Qt::MatchFlags(Qt::MatchFixedString));
    /* 사용자의 상태  */
    foreach(auto v, idIndex) {
        Q_UNUSED(v);
        /* 사용자의 상태 변경 */
        query.prepare("UPDATE serverClientList SET status = ? WHERE id = ?");  //sql쿼리문 준비
        /* sql쿼리문에 값 바인딩 */
        query.bindValue(0, tr("On"));
        query.bindValue(1, id);
        query.exec();           //sql 쿼리 실행
        serverClientModel->select();   //모델의 데이터 조회
    }
    /* 채팅방 사용자 목록 변경 */
    QModelIndexList chatIndex = chattingModel->match(chattingModel->index(0, 2), Qt::EditRole,
                                                     id, -1, Qt::MatchFlags(Qt::MatchFixedString));
    foreach(auto ix, chatIndex){
        if(id == ix.sibling(ix.row(), 2).data().toString()){
            chattingModel->removeRow(ix.row());        //모델의 현재 행 삭제
            ui->chattingTreeView->update();                //뷰 update
        }
    }

}


/* 파일 전송을 위한 소켓 생성 */
void ChatServerForm::acceptConnection()
{
    qDebug() << tr("Connected, preparing to receive files!");
    QTcpSocket* receivedSocket = fileServer->nextPendingConnection();
    connect(receivedSocket, SIGNAL(readyRead()), this, SLOT(readClient()));
}

/* 파일 전송 */
void ChatServerForm::readClient()
{
    qDebug()<<tr("Receiving file ...");
    QTcpSocket* receivedSocket = dynamic_cast<QTcpSocket *>(sender( )); //어느 소켓에서 데이터를 받아왔는지

    QString filename, name, id;
    if (byteReceived == 0) {        // 파일 전송 시작 : 파일에 대한 정보를 이용해서 QFile 객체 생성
        progressDialog->reset();
        progressDialog->show();

        QString ip = receivedSocket->peerAddress().toString();
        quint16 port = receivedSocket->peerPort();

        QDataStream in(receivedSocket);
        in >> totalSize >> byteReceived >> filename >> name >> id;
        progressDialog->setMaximum(totalSize);

        /* 채팅 로그 기록 */
        QTreeWidgetItem* item = new QTreeWidgetItem(ui->chatlogTreeWidget);
        item->setText(0, ip);
        item->setText(1, QString::number(port));
        item->setText(2,  id);
        item->setText(3, name);
        item->setText(4, filename.split("/").last());
        item->setText(5, QDateTime::currentDateTime().toString());
        item->setToolTip(4, filename);

        /* 컨텐츠의 길이로 QTreeWidget의 헤더의 크기를 고정 */
        for(int i = 0; i < ui->chatlogTreeWidget->columnCount(); i++)
            ui->chatlogTreeWidget->resizeColumnToContents(i);

        ui->chatlogTreeWidget->addTopLevelItem(item);
        /* logThread 로그데이터 보냄 */
        logThread->appendData(item);

        QFileInfo info(filename);
        QString currentFileName = info.fileName();  //파일의 경로에서 이름만 뽑아옴
        file = new QFile("../data/file/"+currentFileName);
        /* 쓰기전용으로 open  */
        file->open(QFile::WriteOnly|QFile::Truncate);
    }
    else { /* 파일내용 읽기 */
        inBlock = receivedSocket->readAll();

        byteReceived += inBlock.size();
        file->write(inBlock);
        file->flush();
    }
    /* progressDialog 상태 변경 */
    progressDialog->setValue(byteReceived);

    /* 파일전송이 완료되면 progressDialog 종료, 파일 객체 삭제 */
    if (byteReceived == totalSize) {
        qDebug() << QString(tr("%1 receive completed")).arg(filename);

        inBlock.clear();
        byteReceived = 0;
        totalSize = 0;
        progressDialog->reset();
        progressDialog->hide();
        file->close();
        delete file;
    }
}

/* 서버 관리자의 채팅을 위한 슬롯 */
void ChatServerForm::on_sendButton_clicked()
{
    QString str = ui->inputLineEdit->text( );
    if(str.length( )) {
        ui->message->append("<font color=red>" + tr("Manager") +"</font> : " + str);
        /* 연결되어 있는 모든 소켓 */
        foreach(QTcpSocket *sock, clientSocketHash.values()) {
            QModelIndexList index = serverClientModel->match(serverClientModel->index(0, 2), Qt::EditRole,
                                                             clientPortIDHash[sock->peerPort()], -1, Qt::MatchFlags(Qt::MatchFixedString));
            //로그인 한 id값의 인덱스를 찾음
            foreach(auto ix, index) {
                // 채팅중인 클라이언트들에게 메시지 전송
                if (ix.sibling(ix.row(), 0).data().toString() == tr("Chat")){
                    /* 바이너리구조의 데이터스트림 생성, 전송 */
                    QByteArray sendArray;
                    sendArray.clear();
                    QDataStream out(&sendArray, QIODevice::WriteOnly);
                    out << Chat_Talk;
                    QString data;
                    data = QString("<font color=red>") + tr("Manager") + "</font> : " + str ;
                    out.writeRawData(data.toStdString().data(), 1020);
                    sock->write(sendArray);
                }
            }
        }
    }
    /* 채팅 로그 기록 */
    QTreeWidgetItem* item = new QTreeWidgetItem(ui->chatlogTreeWidget);
    item->setText(0, chatServer->serverAddress().toString());
    item->setText(1, QString::number(chatServer->serverPort()));
    item->setText(2, tr("Manager"));
    item->setText(3, tr("Manager"));
    item->setText(4, str);
    item->setText(5, QDateTime::currentDateTime().toString());
    item->setToolTip(4, str);
    ui->chatlogTreeWidget->addTopLevelItem(item);
    for(int i = 0; i < ui->chatlogTreeWidget->columnCount(); i++)
        ui->chatlogTreeWidget->resizeColumnToContents(i);

    /* logThread로 로그데이터 보냄 */
    logThread->appendData(item);

    ui->inputLineEdit->clear();
}

/* 로그인 성공 유무를 보내는 메서드 */
void ChatServerForm::sendLogInOut(QTcpSocket* sock , const char* data)
{
    /* 바이너리구조의 데이터스트림 생성, 소켓에 보낼 데이터를 채워 전송 */
    QByteArray sendArray;
    QDataStream out(&sendArray, QIODevice::WriteOnly);
    out << Chat_Login;
    out.writeRawData(data, 1020);
    sock->write(sendArray);
}


/* Chat_List로 현재 채팅방 참여인원 전달 */
void ChatServerForm::sendChatList()
{
    /* 바이너리구조의 데이터스트림 생성, 소켓에 보낼 데이터를 채워 전송 */
    QString data;
    QByteArray sendArray;
    QDataStream out(&sendArray, QIODevice::WriteOnly);
    out << Chat_List;
    /* 현재 채팅방 참여인원 전달 */
    QModelIndexList index = serverClientModel->match(serverClientModel->index(0, 0), Qt::EditRole,
                                                     tr("Chat"), -1, Qt::MatchFlags(Qt::MatchFixedString));
    foreach(auto ix, index){
        QString name = ix.sibling(ix.row(), 1).data().toString();
        QString id = ix.sibling(ix.row(), 2).data().toString();
        sendArray.append(",");
        sendArray.append(name.toStdString()+"("+id.toStdString()+")");
        data += "," + (name+"("+id+")") ;
    }
    foreach(QTcpSocket *sock, clientSocketHash.values()) {
        QModelIndexList index = serverClientModel->match(serverClientModel->index(0, 2), Qt::EditRole,
                                                         clientPortIDHash[sock->peerPort()], -1, Qt::MatchFlags(Qt::MatchFixedString));
        foreach(auto v, index){
            Q_UNUSED(v);
            sock->write(sendArray);
        }
    }
}
