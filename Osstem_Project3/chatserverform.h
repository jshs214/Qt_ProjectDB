#ifndef CHATSERVERFORM_H
#define CHATSERVERFORM_H

#include "qsqltablemodel.h"
#include <QWidget>
#include <QList>
#include <QHash>

class QLabel;
class QTcpServer;
class QTcpSocket;
class QFile;
class QProgressDialog;
class LogThread;
class QSqlTableModel;
class QStandardItemModel;
class Delegate;
/* 프로토콜 */
typedef enum {
    Chat_Login,             // 로그인(서버 접속)   --> 채팅방 입장 위한 정보 저장
    Chat_In,                // 채팅방 입장
    Chat_Talk,              // 채팅
    Chat_Out,               // 채팅방 퇴장         --> 초대 가능
    Chat_LogOut,            // 로그 아웃(서버 단절) --> 초대 불가능
    Chat_Invite,             // 초대
    Chat_KickOut,           // 강퇴
    Chat_List,              // 현재 채팅방 참여목록
} Chat_Status;

namespace Ui {
class ChatServerForm;
}

/* 서버클래스 */
class ChatServerForm : public QWidget
{
    Q_OBJECT

public:
    explicit ChatServerForm(QWidget *parent = nullptr);
    ~ChatServerForm();

private:
    const int BLOCK_SIZE = 1024;    // 블록 크기
    const int PORT_NUMBER = 8000;   // 포트번호 8000고정

    Ui::ChatServerForm *ui;

    QTcpServer *chatServer; // 채팅서버
    QTcpServer *fileServer; // 파일서버
    QHash<quint16, QString> clientNameHash;      // port, name
    QHash<quint16, QString> clientPortIDHash;    // port, id
    QHash<QString, QTcpSocket*> clientSocketHash;// id, socket

    QMenu* menu;            //ContextMenu
    QFile* file;            //QFile 객체
    QProgressDialog* progressDialog;    //progressDialog
    qint64 totalSize;       // 데이터의 총 크기
    qint64 byteReceived;    // 소켓에서 읽어온 데이터의 바이트의 크기
    QByteArray inBlock;     // 소켓에서 읽어온 데이터의 바이트
    LogThread* logThread;   // 로그 저장을 위한 스레드

    void sendLogInOut(QTcpSocket* sock , const char* data);     /* 로그인 성공 유무를 보내는 메서드 */
    void sendChatList();    /* Chat_List로 현재 채팅방 참여인원 전달 */

    QSqlTableModel *serverClientModel;  //채팅사용자의 모델
    QStandardItemModel *chattingModel;  //채팅중인 사용자 모델

    Delegate *m_delegate = nullptr;
    void setBackground();   /* 모든 사용자가 Off 일 때 배경색 설정하는 메서드. */

    QList<int> grayList;    //delegate로 배경색을 변경할 행을 저장하는 멤버변수
    QList<int> greenList;   //delegate로 배경색을 변경할 행을 저장하는 멤버변수

private slots:
    void addClient(int, QString);          /* 고객의 데이터가 추가되면 서버에도 추가하는 슬롯 */
    void modClient(int, QString);          /* 고객의 데이터가 변경되면 서버에도 추가하는 슬롯 */
    void remClient(int);                   /* 고객의 데이터가 삭제되면 서버에도 삭제하는 슬롯 */

    void clientConnect();                  /* 채팅 서버 */
    void receiveData();                    /* 데이터를 받을 때 */
    void removeClient();                   /* 연결이 끊어지면 데이터 제거 */
    void on_svClientTableView_customContextMenuRequested(const QPoint &pos);    /* ContextMenu 슬롯 */
    void inviteClient();                   /* 클라이언트 초대하기 */
    void kickOut();                        /* 클라이언트 강퇴하기 */
    void acceptConnection();               /* 파일 서버 */
    void readClient();                     /* 파일 전송 */

    void on_sendButton_clicked();   /* 서버 관리자의 채팅을 위한 슬롯 */


};

#endif // CHATSERVERFORM_H
