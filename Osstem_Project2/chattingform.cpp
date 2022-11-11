#include "chattingform.h"
#include "ui_chattingform.h"

#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QBoxLayout>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QDataStream>
#include <QTcpSocket>
#include <QApplication>
#include <QThread>
#include <QMessageBox>
#include <QSettings>
#include <QFileDialog>
#include <QFile>
#include <QFileInfo>
#include <QProgressDialog>

#define BLOCK_SIZE      1024

ChattingForm::ChattingForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChattingForm), isSent(false)
{
    ui->setupUi(this);

    /* 포트번호, ip 입력란에 정규표현식 사용 */
    ui->serverIP->setText("127.0.0.1");
    //ui->serverAddress->setInputMask("999.999.999.999;_");
    QRegularExpression re("^(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\."
                          "(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\."
                          "(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\."
                          "(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$");
    QRegularExpressionValidator validator(re);
    ui->serverIP->setPlaceholderText("Server IP Address");
    ui->serverIP->setValidator(&validator);
    ui->serverPort->setText(QString::number(PORT_NUMBER));
    ui->serverPort->setInputMask("00000;_");
    ui->serverPort->setPlaceholderText("Server Port No");

    /* 서버로 보낼 메시지를 위한 connect */
    connect(ui->inputLine, SIGNAL(returnPressed( )), SLOT(sendData( )));
    connect(ui->inputLine, SIGNAL(returnPressed( )), ui->inputLine, SLOT(clear( )));
    connect(ui->sentButton, SIGNAL(clicked( )), SLOT(sendData( )));
    connect(ui->sentButton, SIGNAL(clicked( )), ui->inputLine, SLOT(clear( )));
    connect(ui->fileButton, SIGNAL(clicked( )), SLOT(sendFile( )));

    /* 버튼의 상태변경 */
    ui->message->setReadOnly(true);
    ui->inputLine->setEnabled(false);
    ui->sentButton->setEnabled(false);
    ui->fileButton->setDisabled(true);
    ui->logoutButton->setDisabled(true);

    /* 채팅을 위한 소켓 */
    clientSocket = new QTcpSocket(this);
    connect(clientSocket, &QAbstractSocket::errorOccurred,
            [=]{ qDebug( ) <<  clientSocket->errorString( ); });
    connect(clientSocket, SIGNAL(readyRead( )), SLOT(receiveData( )));
    connect(clientSocket, SIGNAL(disconnected( )), SLOT(disconnect( )));

    /* 파일 전송을 위한 소켓 */
    fileClient = new QTcpSocket(this);

    /* 파일 전송 시 나눠서 전송 */
    connect(fileClient, SIGNAL(bytesWritten(qint64)), SLOT(goOnSend(qint64)));

    /* progressDialog 객체 생성, 설정 */
    progressDialog = new QProgressDialog(0);
    progressDialog->setAutoClose(true);
    progressDialog->reset();

    connect(ui->connectButton, &QPushButton::clicked,
            [=]{
        if(ui->connectButton->text() == tr("Log In")) {               /* Log In 버튼 클릭 시 Log In */
            /* 입력받은 ip주소, port번호로 소켓에 연결 */
            clientSocket->connectToHost(ui->serverIP->text( ),
                                        ui->serverPort->text( ).toInt( ));
            clientSocket->waitForConnected();       //접속 끝날때 까지 대기

            /* 프로토콜 생성해서 서버로 전송 */
            sendProtocol(Chat_Login, (ui->nameLineEdit->text().toStdString()+","+
                                      ui->idLineEdit->text().toStdString()).data() );

            savedLoadData();
        }
        else if(ui->connectButton->text() == tr("Chat in"))  {        /* Chat in 버튼 클릭 시 Chat in */
            /* 프로토콜 생성해서 서버로 전송 */
            sendProtocol(Chat_In, (ui->nameLineEdit->text().toStdString()+","+
                                   ui->idLineEdit->text().toStdString()).data() );
            /* 버튼의 상태 변경 */
            ui->connectButton->setText(tr("Chat Out"));
            ui->inputLine->setEnabled(true);
            ui->sentButton->setEnabled(true);
            ui->fileButton->setEnabled(true);

            loadData();
        }
        else if(ui->connectButton->text() == tr("Chat Out"))  {       /* Chat Out 버튼 클릭 시 Chat Out */
            /* 프로토콜 생성해서 서버로 전송 */
            sendProtocol(Chat_Out, (ui->nameLineEdit->text().toStdString()+","+
                                    ui->idLineEdit->text().toStdString()).data() );
            /* 버튼의 상태 변경 */
            ui->connectButton->setText(tr("Chat in"));
            ui->inputLine->setDisabled(true);
            ui->sentButton->setDisabled(true);
            ui->fileButton->setDisabled(true);

            saveData();
            ui->message->clear();
        }
    } );

    setWindowTitle(tr("Chat Client"));


}

/* 소멸자에서 소켓 close */
ChattingForm::~ChattingForm()
{
    clientSocket->close( );

    delete ui;
}

/* 창이 닫힐 때 서버에 연결 접속 메시지를 보내고 종료 */
void ChattingForm::closeEvent(QCloseEvent*)
{
    saveData();
    qDebug()<< "종료했슴둥 ^~^";

    if(ui->connectButton->text() == tr("Log In"))   return; // 로그인 안된 상태에서는 소켓 connect이 안되있으므로 예외처리

    /* 프로토콜 생성해서 서버로 전송 */
    sendProtocol(Chat_LogOut, (ui->nameLineEdit->text().toStdString()+","+
                               ui->idLineEdit->text().toStdString()).data() );
    clientSocket->disconnectFromHost(); //소켓 닫기
    if(clientSocket->state() != QAbstractSocket::UnconnectedState)  /* 소켓 연결이 끊길때 까지 대기 */
        clientSocket->waitForDisconnected();
}

/* 서버에서 데이터가 올 때 */
void ChattingForm::receiveData( )
{
    QTcpSocket *clientSocket = dynamic_cast<QTcpSocket *>(sender( ));   //어느 소켓에서 데이터를 받아왔는지

    if (clientSocket->bytesAvailable( ) > BLOCK_SIZE) return;   // 예외처리
    QByteArray bytearray = clientSocket->read(BLOCK_SIZE);

    Chat_Status type;       // 채팅 프로토콜 타입
    char data[1020];        // 전송되는 메시지/데이터
    memset(data, 0, 1020);

    QDataStream in(&bytearray, QIODevice::ReadOnly);
    in.device()->seek(0);
    in >> type;         // 채팅 프로토콜 타입
    in.readRawData(data, 1020);     //전송되는 데이터 / 메시지

    switch(type) {
    case Chat_Login:    // 로그인(서버 접속)
        /* 로그인 성공 시 사용자의 상태 변경*/
        if(QString(data) == "true")
        {
            QMessageBox::information(this, tr("Success"), \
                                     tr("Successed Login"));
            ui->connectButton->setText(tr("Chat in"));
            ui->nameLineEdit->setReadOnly(true);
            ui->idLineEdit->setReadOnly(true);
            ui->logoutButton->setEnabled(true);
            ui->serverIP->setEnabled(false);
            ui->serverPort->setEnabled(false);
        }
        /* 로그인 실패 시 경고메시지 출력 후 종료 */
        else
        {
            clientSocket->blockSignals(true);   //로그인 실패 시 소켓 읽어오는 시그널 차단.
            QMessageBox::critical(this, tr("Error"), \
                                  tr("Failed Login"));
            clientSocket->blockSignals(false);  //로그인 실패 시 소켓 읽어오는 시그널 해제.
        }
        break;
    case Chat_Talk:     // 채팅
        ui->message->append(QString(data));     //메시지 화면에 표시

        ui->inputLine->setEnabled(true);        //버튼 상태 변경
        ui->sentButton->setEnabled(true);
        ui->fileButton->setEnabled(true);

        chattingData[ui->idLineEdit->text()].append(QString(data));
        break;
    case Chat_KickOut:  //강퇴 당한 경우
    {
        /* 프로토콜 생성해서 서버로 전송 */
        sendProtocol(Chat_Out, (ui->nameLineEdit->text().toStdString()+","+
                                ui->idLineEdit->text().toStdString()).data() );
        QMessageBox::critical(this, tr("Chatting Client"), \
                              tr("Kick out from Server"));
        ui->inputLine->setDisabled(true);        //버튼 상태 변경
        ui->sentButton->setDisabled(true);
        ui->fileButton->setDisabled(true);
        ui->connectButton->setText(tr("Chat in"));

        /* 프로토콜 생성해서 서버로 전송 */
        QString data ="";
        sendProtocol(Chat_List,data.toStdString().data());

        saveData();
        ui->message->clear();
        break;
    }
    case Chat_Invite:   //초대 받은 경우
    {
        /* 프로토콜 생성해서 서버로 전송 */
        sendProtocol(Chat_In, (ui->nameLineEdit->text().toStdString()+","+
                               ui->idLineEdit->text().toStdString()).data() );

        QMessageBox::information(this, tr("Chatting Client"), \
                                 tr("Invited from Server"));
        ui->inputLine->setEnabled(true);        //버튼 상태 변경
        ui->sentButton->setEnabled(true);
        ui->fileButton->setEnabled(true);
        ui->connectButton->setText(tr("Chat Out"));
        ui->nameLineEdit->setReadOnly(true);

        /* 프로토콜 생성해서 서버로 전송 */
        QString data ="";
        sendProtocol(Chat_List,data.toStdString().data());

        loadData();
        break;
    }
    case Chat_List: // 현재 채팅방 참여인원 전달 받아 상태변경
    {
        ui->stateTreeWidget->clear();

        QList<QString> row = QString(data).split(",");
        QString receiveData = QString::fromStdString(data);
        foreach (auto v,row) {
            QTreeWidgetItem* item = new QTreeWidgetItem(ui->stateTreeWidget);
            item->setText(0,v);
            ui->stateTreeWidget->addTopLevelItem(item);
        }
        ui->stateTreeWidget->takeTopLevelItem(0);   //공백 칸 제거

        break;
    }
    default:
        break;
    };
}

/* 서버로 메시지 전송 슬롯 */
void ChattingForm::sendData(  )
{
    QString str = ui->inputLine->text( );
    /* 전송할 데이터가 있으면 전송 */
    if(str.length( )) {
        QByteArray bytearray;
        bytearray = str.toUtf8( );
        ui->message->append(tr("Me : ") + str);
        sendProtocol(Chat_Talk, bytearray.data()); // 프로토콜 생성해서 서버로 전송
        chattingData[ui->idLineEdit->text()].append(tr("Me : ") + str);
    }
}

/* 서버로 파일 전송 슬롯 */
void ChattingForm::sendFile()
{
    loadSize = 0;
    byteToWrite = 0;
    totalSize = 0;
    outBlock.clear();
    /* 파일 보내기 파일을 열고 파일 이름(경로 포함)을 가져오는 슬롯 */
    QString filename = QFileDialog::getOpenFileName(this);
    if(filename.length()){  //파일이 있으면
        file = new QFile(filename);
        file->open(QFile::ReadOnly);    //파일 오픈

        qDebug() << QString(tr("file %1 is opened")).arg(filename);
        progressDialog->setValue(0); // 처음으로 전송되지 않음

        /* 처음 전송될 때만 연결이 연결 신호를 생성할 때 발생 */
        /* 처음으로 보낼 때 connectToHost는 연결 신호를 시작, 두 번째 후에는 send를 호출 */
        if (!isSent) {
            fileClient->connectToHost(ui->serverIP->text( ),
                                      ui->serverPort->text( ).toInt( ) + 1);
            isSent = true;
        }

        byteToWrite = totalSize = file->size(); // 나머지 데이터의 크기
        loadSize = 1024; // 매번 전송되는 데이터의 크기

        QDataStream out(&outBlock, QIODevice::WriteOnly);
        out << qint64(0) << qint64(0) << filename << ui->nameLineEdit->text()
            << ui->idLineEdit->text();

        totalSize += outBlock.size(); //전체 크기는 파일 크기 + 파일 이름 + 기타 정보의 크기
        byteToWrite += outBlock.size();

        /* 데이터스트림의 시작 부분에서 전체 크기와 파일 이름 및 기타 정보 크기인 qint64를 앞에 씀 */
        out.device()->seek(0);
        out << totalSize << qint64(outBlock.size());

        fileClient->write(outBlock); // 읽은 파일을 소켓으로 보냄

        /* progressDialog 상태 */
        progressDialog->setMaximum(totalSize);
        progressDialog->setValue(totalSize-byteToWrite);
        progressDialog->show();
    }
    qDebug() << QString(tr("Sending file %1")).arg(filename);

}

/* 파일 전송 시 여러번 나눠서 전송하는 슬롯 */
void ChattingForm::goOnSend(qint64 numBytes)
{
    byteToWrite -= numBytes; // 남은 데이터 크기만큼
    outBlock = file->read(qMin(byteToWrite, numBytes));
    fileClient->write(outBlock);

    /* progressDialog 설정 */
    progressDialog->setMaximum(totalSize);
    progressDialog->setValue(totalSize-byteToWrite);

    /* 전송 완료되면 progressDialog리셋 */
    if (byteToWrite == 0) {
        qDebug() << tr("File sending completed!");
        progressDialog->reset();
    }
}

/* 연결이 끊어졌을 때 상태 변경 */
void ChattingForm::disconnect( )
{
    QMessageBox::critical(this, tr("Chatting Client"), \
                          tr("Disconnect from Server"));
    ui->inputLine->setEnabled(false);
    ui->nameLineEdit->setReadOnly(false);
    ui->idLineEdit->setReadOnly(false);
    ui->sentButton->setEnabled(false);
    ui->connectButton->setText(tr("Log In"));
    ui->logoutButton->setDisabled(true);
    ui->serverIP->setEnabled(true);
    ui->serverPort->setEnabled(true);
}

/* 프로토콜 생성해서 서버로 전송 */
void ChattingForm::sendProtocol(Chat_Status type, char* data, int size)
{
    /* 바이너리구조의 데이터스트림 생성, 소켓에 보낼 데이터를 채워 전송 */
    QByteArray dataArray;
    QDataStream out(&dataArray, QIODevice::WriteOnly);
    out.device()->seek(0);
    out << type;
    out.writeRawData(data, size);
    clientSocket->write(dataArray);     // 서버로 전송
    clientSocket->flush();
    while(clientSocket->waitForBytesWritten()); //waitForBytesWritten 디폴트값 3초
}

/* 로그아웃 슬롯 */
void ChattingForm::on_logoutButton_clicked()
{
    sendProtocol(Chat_LogOut, ui->nameLineEdit->text().toStdString().data()); // 프로토콜 생성해서 서버로 전송
    ui->message->clear();
    clientSocket->disconnectFromHost(); //소켓 닫기
    /* 버튼의 상태변경 */
    ui->serverIP->setEnabled(true);
    ui->serverPort->setEnabled(true);
    ui->stateTreeWidget->clear();
}

/* 프로그램 종료 후 저장되었던 기존 메시지 로그 불러오기 */
void ChattingForm::savedLoadData()
{
    QString filename =ui->idLineEdit->text()+ "," + ui->nameLineEdit->text();

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text | QIODevice::Truncate))
        return;

    /* 메시지 로그의 데이터 입력 */
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        chattingData[ui->idLineEdit->text()].append(line);
    }
    file.close( );
}

/* 프로그램 실행 중, 입력된 메시지 로그 불러오기 */
void ChattingForm::loadData()
{    
    QString filename =ui->idLineEdit->text()+ "," + ui->nameLineEdit->text();

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    /* 메시지 로그의 데이터 입력 */
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        ui->message->append(line);
    }
    file.close( );
}

/* 메시지 로그 저장 */
void ChattingForm::saveData()
{
    QString filename =ui->idLineEdit->text()+ "," + ui->nameLineEdit->text();

    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text ))
        return;

    /* 메시지 로그의 데이터를 줄마다 저장 */
    QTextStream out(&file);
    for (const auto& v : qAsConst(chattingData[ui->idLineEdit->text()])) {
        out << v <<"\n";
    }

    file.close( );
}
