#ifndef CHATTINGFORM_H
#define CHATTINGFORM_H

#include <QWidget>
#include <QDataStream>

#include "chatserverform.h"     // Chat_Status 구조체 사용

class QTcpSocket;
class QFile;
class QProgressDialog;
class ChattingThread;

namespace Ui {
class ChattingForm;
}

/* 채팅프로그램 */
class ChattingForm : public QWidget
{
    Q_OBJECT

public:
    const int PORT_NUMBER = 8000;   //포트번호 8000번 고정

    explicit ChattingForm(QWidget *parent = nullptr);
    ~ChattingForm();

    void loadData();        /* 채팅방의 채팅 로그 불러오기 */

private:
    Ui::ChattingForm *ui;

    void closeEvent(QCloseEvent*) override; /* 창이 닫힐 때 서버에 연결 접속 메시지를 보내고 종료 */
    QTcpSocket *clientSocket;		// 클라이언트용 소켓
    QTcpSocket *fileClient;         // 파일용 소켓
    QProgressDialog* progressDialog;    // 파일 진행 확인
    QFile* file;            // 서버로 보내는 파일
    qint64 loadSize;        // 파일의 크기
    qint64 byteToWrite;     // 보낸 파일의 크기
    qint64 totalSize;       // 전체 파일의 크기
    QByteArray outBlock;    // 전송을 위한 데이터
    bool isSent;            // 파일 서버에 접속되었는지 확인

    ChattingThread* chattingLog;   // 채팅로그 저장을 위한 스레드
private slots:
    void receiveData( );		/* 서버에서 데이터가 올 때 */
    void sendData( );           /* 서버로 메시지 전송 슬롯 */
    void sendFile();            /* 서버로 파일 전송 슬롯 */
    void goOnSend(qint64);      /* 파일 전송 시 여러번 나눠서 전송하는 슬롯 */
    void disconnect( );         /* 연결이 끊어졌을 때 상태 변경 */
    void sendProtocol(Chat_Status, char*, int = 1020);  /* 프로토콜 생성해서 서버로 전송 */
    void on_logoutButton_clicked();     /* 로그아웃 슬롯 */

};

#endif // CHATTINGFORM_H

