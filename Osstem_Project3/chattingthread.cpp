#include "chattingthread.h"
#include "qdebug.h"

#include <QFile>
#include <QTextStream>

ChattingThread::ChattingThread(int id, QString name, QObject *parent)
    : QThread{parent}
{
    filename = QString::number(id)+ "_" + name;

}

void ChattingThread::run()
{
    Q_FOREVER {
        saveData();
        sleep(60);      // 1분마다 저장
    }
}
/* 로그 입력 */
void ChattingThread::appendData(QString str)
{
   clientChattingLog.append(str);
}

/* 로그 데이터 저장 */
void ChattingThread::saveData()
{
    if(clientChattingLog.count()> 0){
    QFile file("../data/chattingDB/" + filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
        return;

    /* 채팅 로그의 데이터를 \n 마다 저장 */
    QTextStream out(&file);
    foreach(auto v, clientChattingLog){
        out << v <<"\n";
    }
    file.close( );
    clientChattingLog.clear();
    }
}

